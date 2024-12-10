#define _MULTI_THREADED
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdbool.h>

/*
 * A thread on each core seems to provoke cache misses
 */
#define LOOP_CNT (1 * 1024 * 1024)
void busy_wait(int microseconds) {
  volatile int i;
  for (i = 0; i < microseconds * 100; i++); // Adjust the multiplier as needed
}
enum ACCESS_PATTERN {
  ACCESS_SHARING,
  ACCESS_NOSHARING
};
typedef enum ACCESS_PATTERN access_pattern;

enum WORKER_TYPE {
  WORKER_WRITERONLY,
  WORKER_MIXED
};
typedef enum WORKER_TYPE worker_type;

#if defined(__x86_64__) || defined(__i386__)
static __inline__ uint64_t rdtsc() {
  unsigned hi, lo;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}
#endif

/*
 * Create a data struct which occupies a whole cacheline.
 */
typedef struct _buf {
  long data[8];
} buf __attribute__((aligned(64)));

buf *access_buf;

worker_type worker = WORKER_MIXED;
access_pattern pattern = ACCESS_SHARING;
volatile int wait_to_begin = 1;
struct thread_data *thread;
int num_threads;
bool sync_flag = true;
long global_lock;
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
long global_data = 0;
char *writer_name = "Writer";
char *reader_name = "Reader";

#define checkResults(string, val) {             \
 if (val) {                                     \
   printf("Failed with %d at %s", val, string); \
   exit(1);                                     \
 }                                              \
}

struct thread_data {
  pthread_t tid;
  long tix;
  char *name;
};

/*
 * Bind a thread to the specified CPU core.
 */
void setAffinity(void *parm) {
  int tix = ((struct thread_data *)parm)->tix;
  char *func_name = ((struct thread_data *)parm)->name;

  /* set thread affinity */
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(tix, &cpuset);

  pthread_t pid = pthread_self();
  int set_result = pthread_setaffinity_np(pid, sizeof(cpu_set_t), &cpuset);
  if (set_result != 0) {
    printf("pthread_setaffinity_np return error[%d]\n", set_result);
  }

  pthread_setname_np(pthread_self(), func_name);
}

/*
 * Thread function to simulate the false sharing.
 * The "lock" threads will test-n-set the lock field,
 * while the reader threads will just read the other fields
 * in the struct.
 */
extern void *read_write_func(void *parm) {
  int tix = ((struct thread_data *)parm)->tix;
  uint64_t start, stop, j;
  char *thd_name = ((struct thread_data *)parm)->name;
  int buf_id, data_id;
  uint64_t temp = 0;

  // Pin each thread to a CPU core.
  setAffinity(parm);

  // Wait for all threads to get created before starting.
  while (wait_to_begin);

  start = rdtsc();
  if (pattern == ACCESS_NOSHARING) {
    for (j = 0; j < LOOP_CNT; j++) {
      buf_id = tix;
      data_id = tix % 8;
      if (*thd_name == *writer_name) { // Writer threads, each thread works on a separate buf
        access_buf[buf_id].data[data_id]++;
      } else { // Reader threads.
        temp += access_buf[buf_id].data[data_id];
      }
    }
  }
  if (pattern == ACCESS_SHARING) { // all threads access buf[0]
    buf_id = 0;
    if (worker == WORKER_WRITERONLY)
      data_id = 0;
    else
      data_id = tix % 8;
    for (j = 0; j < LOOP_CNT; j++) {
      if (*thd_name == *writer_name) {
        if (sync_flag)
          while (__sync_lock_test_and_set(&global_lock, 1) != 0);
        access_buf[buf_id].data[data_id]++;
        if (sync_flag)
          __sync_lock_release(&global_lock);
      } else {
        temp += access_buf[buf_id].data[data_id];
      }
    }
  }

  // Print out stats
  stop = rdtsc();

  int cpu = sched_getcpu();

  printf("%ld mticks, %s (thread %d), on cpu %d. temp: %ld, data: %ld. [%d:%d]\n", (stop - start) / 1000000, thd_name, tix, cpu, temp, access_buf[buf_id].data[data_id], buf_id, data_id);

  return NULL;
}

void print_usage(char *argv[]) {
  printf("Usage: %s -t <worker_type: [w/m]> -n <num_threads> -a <sharing_data: [s/n]> -s <synchronize: [y/n]>\n", argv[0]);
}

int main(int argc, char *argv[]) {
  int i, j, rc = 0;
  int nts;
  char access, worker_char, sync;

  char cha;
  while ((cha = getopt(argc, argv, "t:n:a:s:h")) != -1) {
    switch (cha) {
      case 't':
        worker_char = optarg[0];
        break;
      case 'n':
        nts = atol(optarg);
        break;
      case 'a':
        access = optarg[0];
        break;
      case 's':
        sync = optarg[0];
        break;
      case 'h':
      default:
        print_usage(argv);
        exit(0);
    }
  }

  if (pthread_mutex_init(&global_mutex, NULL) != 0)
    printf("mutex init failed\n");

  if (access == 'n' || access == 'N')
    pattern = ACCESS_NOSHARING;

  if (worker_char == 'w' || worker_char == 'W')
    worker = WORKER_WRITERONLY;

  if (sync == 'y' || sync == 'Y')
    sync_flag = true;
  else
    sync_flag = false;

  num_threads = nts;

  access_buf = (buf *)aligned_alloc(64, sizeof(buf) * num_threads);
  thread = malloc(sizeof(struct thread_data) * num_threads);
  if (access_buf == NULL || thread == NULL) {
    fprintf(stderr, "Cannot allocate memory: access_buf[%p] and thread[%p]\n", access_buf, thread);
    return 1;
  }

  for (i = 0; i < num_threads; i++) {
    for (j = 0; j < 8; j++) {
      access_buf[i].data[j] = j;
    }
  }

  printf("data_race running: %d threads, data_sharing: %d, synchronization: %d[%c]\n", num_threads, pattern, sync_flag, sync);

  if (worker == WORKER_MIXED) {
    // Create the first half of threads as writer threads.
    for (i = 0; i <= (num_threads / 2 - 1); i++) {
      thread[i].tix = i;
      thread[i].name = writer_name;
      rc = pthread_create(&thread[i].tid, NULL, read_write_func, &thread[i]);
      checkResults("pthread_create()\n", rc);
      busy_wait(500);
    }

    // Create the second half of threads as reader threads.
    for (i = ((num_threads / 2)); i < (num_threads); i++) {
      thread[i].tix = i;
      thread[i].name = reader_name;
      rc = pthread_create(&thread[i].tid, NULL, read_write_func, &thread[i]);
      checkResults("pthread_create()\n", rc);
     busy_wait(500);
    }
  }

  if (worker == WORKER_WRITERONLY) {
    for (i = 0; i < num_threads; i++) {
      thread[i].tix = i;
      thread[i].name = writer_name;
      rc = pthread_create(&thread[i].tid, NULL, read_write_func, &thread[i]);
      checkResults("pthread_create()\n", rc);
      busy_wait(500);
    }
  }

  // Sync to let threads start together
  busy_wait(500);
  wait_to_begin = 0;

  for (i = 0; i < num_threads; i++) {
    rc = pthread_join(thread[i].tid, NULL);
    checkResults("pthread_join()\n", rc);
  }

  if (worker == WORKER_WRITERONLY) {
    printf("buf[0]: ");
    for (j = 0; j < 8; j++) {
      printf(" %ld", access_buf[0].data[j]);
    }
    printf("\n");
  }

  return 0;
}
