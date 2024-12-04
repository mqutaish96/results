/*
 * This program shows data race between concurrent threads running on numa nodes.  
 *
 * The usage is: 
 *     ./data_race.exe -t <worker_type: w(writer only)/m(half readers and half writers)> -n <number of threads per node> -a <data sharing: s/n> -s <sychronize: y/n> -p <enable PAPI>
 *
 * The program creats a number of concurrent threads, half of which are writer and the other half are reader.
 * It will pin these threads in round-robin fashion to different CPU cores across numa nodes in the system.
 *
 * For example, on a system with 2 numa nodes:
 * ./data_race.exe -n 2 -a s -s n 
 * 193 mticks, Reader (thread 3), on node 1 (cpu 19).
 * 215 mticks, Writer (thread 1), on node 1 (cpu 17).
 * 221 mticks, Reader (thread 2), on node 0 (cpu 18).
 * 231 mticks, Writer (thread 0), on node 0 (cpu 16).
 *
 * ./data_race.exe -n 2 -a n -s n
 * 25 mticks, Reader (thread 2), on node 0 (cpu 18).
 * 25 mticks, Writer (thread 0), on node 0 (cpu 16).
 * 27 mticks, Writer (thread 1), on node 1 (cpu 17).
 * 43 mticks, Reader (thread 3), on node 1 (cpu 19).
 */

#define _MULTI_THREADED
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <numa.h>
#include <sys/types.h>
#include <stdbool.h>

#ifdef _USE_PAPI
#include <papi.h>
#endif

/*
 * A thread on each numa node seems to provoke cache misses
 */
#define 	LOOP_CNT     	(1 * 1024 * 1024) 

void wait_500ms() {
    clock_t start, current;
    start = clock();  // Get the current clock time

    // Busy wait until 500 milliseconds have passed
    do {
        current = clock();  // Get the current clock time
    } while ((double)(current - start) / CLOCKS_PER_SEC < 0.5);  // Check if 500ms have passed

    printf("500ms elapsed\n");
}


enum ACCESS_PATTERN{
  ACCESS_SHARING,
  ACCESS_NOSHARING
};
typedef enum ACCESS_PATTERN access_pattern;

enum WORKER_TYPE{
  WORKER_WRITERONLY,
  WORKER_MIXED
};
typedef enum WORKER_TYPE worker_type;

#if defined(__x86_64__) || defined(__i386__) 
static __inline__ uint64_t rdtsc() {
    unsigned hi, lo;
    __asm__ __volatile__ ( "rdtsc" : "=a"(lo), "=d"(hi));
    return ( (uint64_t)lo) | ( ((uint64_t)hi) << 32);
}

#elif defined(__aarch64__)
static __inline__ uint64_t rdtsc(void)
{
    uint64_t val;

    /*
     * According to ARM DDI 0487F.c, from Armv8.0 to Armv8.5 inclusive, the
     * system counter is at least 56 bits wide; from Armv8.6, the counter
     * must be 64 bits wide.  So the system counter could be less than 64
     * bits wide and it is attributed with the flag 'cap_user_time_short'
     * is true.
     */
    asm volatile("mrs %0, cntvct_el0" : "=r" (val));

    return val;
}
#endif


/* 
 * Create a data struct which occupies a whole cacheline.
 */
typedef struct _buf {
  long data[8]; 
} buf __attribute__((aligned (64)));

buf *access_buf;

worker_type worker = WORKER_MIXED;
access_pattern pattern = ACCESS_SHARING;
volatile int wait_to_begin = 1;
struct thread_data *thread;
int num_nodes;
int num_cores;
int num_threads; 
bool sync_flag = true;
long global_lock;
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
long global_data = 0;
char * writer_name = "Writer";
char * reader_name = "Reader";
#ifdef _USE_PAPI
bool enable_papi = false;
#endif

#define checkResults(string, val) {             \
 if (val) {                                     \
   printf("Failed with %d at %s", val, string); \
   exit(1);                                     \
 }                                              \
}
 
struct thread_data {
    pthread_t tid;
    long tix;
    long node;
    char *name;
};

/*
 * Bind a thread to the specified numa node.
*/
void setAffinity(void *parm) {
  volatile uint64_t rc, j;
  int tix        = ((struct thread_data *)parm)->tix;
  int node        = ((struct thread_data *)parm)->node;
  char *func_name = ((struct thread_data *)parm)->name;
  int ret;

//   ret = numa_run_on_node(node); //by no reason, this function doesn't work on Wiener
//   if(ret != 0)
//     printf("setAffinity: i %d, node: %d failed!\n", tix, node);

  /* set thread affinity */
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(tix+16, &cpuset);

   pthread_t pid = pthread_self();
   int set_result = pthread_setaffinity_np(pid, sizeof(cpu_set_t), &cpuset);
   if (set_result != 0) {
//     printf("pthread_setaffinity_np return error[%d]\n", set_result);
   }

  pthread_setname_np(pthread_self(),func_name);
}

#ifdef _USE_PAPI
void handle_error (int retval)
{
    /* print error to stderr and exit */
    PAPI_strerror(retval);
    printf("PAPI error: %d\n", retval);
    exit(1);
}

void start_counters(int EventSet)
{
    if (PAPI_start(EventSet) != PAPI_OK)
        handle_error(1);
}

void stop_counters(int EventSet, long_long *values)
{
    /* Stop the counting of events in the Event Set */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);
}
#endif

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

#ifdef _USE_PAPI
   int EventSet=PAPI_NULL;
   long_long values[2];

  if(enable_papi)
  {
    /* Create the Event Set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        handle_error(1);
  
    if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK)
        handle_error(1);
  
    if (PAPI_add_event(EventSet, PAPI_L2_DCM) != PAPI_OK)
        handle_error(1);
  }
#endif

   // Pin each thread to a numa node.
   setAffinity(parm);

   // Wait for all threads to get created before starting.
   while(wait_to_begin) ;

#ifdef _USE_PAPI
  if(enable_papi)
  {
    start_counters(EventSet);
  }
#endif

   start = rdtsc();
   if(pattern == ACCESS_NOSHARING)
   {
     for(j=0; j<LOOP_CNT; j++) {
     buf_id = tix;
     data_id = tix % 8;
     if (*thd_name == *writer_name) { // Writer threads, each thread works on a separate buf
        access_buf[buf_id].data[data_id]++; 
     } else { // Reader threads.
        temp += access_buf[buf_id].data[data_id]; 
     }; 
    } 
  }
  if(pattern == ACCESS_SHARING) //all threads access buf[0]
  {
    buf_id = 0;
    if(worker == WORKER_WRITERONLY)
      data_id = 0;
    else
      data_id = tix % 8;
    for(j=0; j<LOOP_CNT; j++)
    {
      if(*thd_name == *writer_name)
      {
        if(sync_flag)
        while(__sync_lock_test_and_set(&global_lock, 1 ) != 0);
        //pthread_mutex_lock(&global_mutex);
        access_buf[buf_id].data[data_id]++; 
        //pthread_mutex_unlock(&global_mutex);
        if(sync_flag)
        __sync_lock_release(&global_lock);
      } else {
        temp += access_buf[buf_id].data[data_id]; 
      }
    }
  }

  // Print out stats
  //
  stop = rdtsc();

#ifdef _USE_PAPI
  if(enable_papi)
  {
    stop_counters(EventSet, values);
  }
#endif

  int cpu = sched_getcpu();
  int node = numa_node_of_cpu(cpu);

#ifdef _USE_PAPI
  printf("%ld mticks, %s (thread %d), on node %d (cpu %d). L1_DCM %lld, L2_DCM %lld. temp: %ld, data: %ld. [%d:%d]\n", (stop-start)/1000000, thd_name, tix, node, cpu, values[0], values[1], temp, access_buf[buf_id].data[data_id], buf_id, data_id);
#else
  printf("%ld mticks, %s (thread %d), on node %d (cpu %d). temp: %ld, data: %ld. [%d:%d]\n", (stop-start)/1000000, thd_name, tix, node, cpu, temp, access_buf[buf_id].data[data_id], buf_id, data_id);
#endif

  return NULL;
}
 
void print_usage(char *argv[])
{
    printf("Usage: %s -t <worker_type: [w/m]> -n <num_threads_per_node> -a <sharing_data: [s/n]> -s <sychronize: [y/n]> -p enable_papi\n", argv[0]);
}

int main ( int argc, char *argv[] )
{
  int     i, j, n, rc=0;
  int nts_per_node;
  char access, worker_char, sync;

  char cha;
  while ((cha = getopt (argc, argv, "t:n:a:s:ph")) != -1)
  {
    switch (cha)
    {
      case 't':
        worker_char = optarg[0];
        break;
      case 'n':
        nts_per_node  = atol(optarg);
        break;
      case 'a':
        access = optarg[0];
        break;
      case 's':
        sync = optarg[0];
        break;
      case 'p':
#ifdef _USE_PAPI
        enable_papi = true;
#endif
        break;
      case 'h':
      default:
        print_usage(argv);
        exit(0);
    }
  }

#ifdef _USE_PAPI
  /* Initialize the PAPI library */
  int retval;
  if(enable_papi)
  {
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
      fprintf(stderr, "PAPI library init error!\n");
      exit(1);
    }
  }
#endif

  if ( pthread_mutex_init( &global_mutex, NULL) != 0 )
    printf( "mutex init failed\n" );

  if (numa_available() < 0)
  {
   printf( "NUMA not available\n" );
   exit(1);
  }

  if(access == 'n' || access == 'N')
    pattern = ACCESS_NOSHARING;

  if(worker_char == 'w' || worker_char == 'W')
    worker = WORKER_WRITERONLY;

  if(sync == 'y' || sync == 'Y')
    sync_flag = true;
   else
    sync_flag = false;

  num_nodes = numa_max_node();
  num_nodes++;

  num_threads = (num_nodes) * nts_per_node;

  num_cores = numa_num_configured_cpus();
  access_buf = (buf*)aligned_alloc(64, sizeof(buf)*num_threads);
  thread = malloc(sizeof(struct thread_data) * num_threads);
  if(access_buf == NULL || thread == NULL)
  {
    fprintf(stderr, "Cannot allocate memory: access_buf[%p] and thread[%p]\n", access_buf, thread);
    return 1;
  }
  
  for(i = 0; i < num_threads; i++)
  {
    for(j = 0; j < 8; j++) {
      access_buf[i].data[j] = j;
    }
  }
 
  printf("data_race running: %d threads, %d nodes, %d cores, data_sharing: %d, synchronization: %d[%c]\n", num_threads, num_nodes, num_cores, pattern, sync_flag, sync);

  if(worker == WORKER_MIXED)
  {
    // Create the first half of threads as writer threads.
    // Assign each thread a successive round robin node to 
    // be pinned to (later after it gets created.)
    //
    for (i=0; i<=(num_threads/2 - 1); i++) {
       thread[i].tix = i;
       thread[i].node = i%num_nodes;
       thread[i].name = writer_name;
       rc = pthread_create(&thread[i].tid, NULL, read_write_func, &thread[i]);
       checkResults("pthread_create()\n", rc);
       wait_500ms();
    }
  
    // Create the second half of threads as reader threads.
    // Assign each thread a successive round robin node to 
    // be pinned to (later after it gets created.)
    //
    for (i=((num_threads/2)); i<(num_threads); i++) {
       thread[i].tix = i;
       thread[i].node = i%num_nodes;
       thread[i].name = reader_name;
       rc = pthread_create(&thread[i].tid, NULL, read_write_func, &thread[i]);
       checkResults("pthread_create()\n", rc);
       wait_500ms();
    }
  }

  if(worker == WORKER_WRITERONLY)
  {
    for (i=0; i<num_threads; i++) {
       thread[i].tix = i;
       thread[i].node = i%num_nodes;
       thread[i].name = writer_name;
       rc = pthread_create(&thread[i].tid, NULL, read_write_func, &thread[i]);
       checkResults("pthread_create()\n", rc);
       wait_500ms();
    }
  }

  // Sync to let threads start together
  wait_500ms();
  wait_to_begin = 0;
 
  for (i=0; i <num_threads; i++) {
     rc = pthread_join(thread[i].tid, NULL);
     checkResults("pthread_join()\n", rc);
  }

  if(worker == WORKER_WRITERONLY)
  {
    printf("buf[0]: ");
    for(j = 0; j < 8; j++) {
      printf(" %ld", access_buf[0].data[j]);
    }
    printf("\n");
  }

  return 0;
}
