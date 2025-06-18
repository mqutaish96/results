#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE_DEFAULT 256
#define STRIDE_DEFAULT 8

int main(int argc, char* argv[])
{
    printf("\n ---------------------------------- Start of program -------------------------");

    int length = SIZE_DEFAULT, width = SIZE_DEFAULT;
    int stride = STRIDE_DEFAULT;
    int i, j, iteration = length * width / stride;
    int expect = (stride < sizeof(int64_t)) ? (length * width / sizeof(int64_t)) : length * width;

    int64_t* matrix;
    int64_t* matrix_fill;
    int64_t temp = 0;
    int s;

    printf("\n\n");
    printf("width:%d, length: %d, stride:%d\n\n", width, length, stride);

    // Print addresses of variables
    printf("Matrix addr (before allocation): %p \n", &matrix);
    printf("Matrix_fill addr (before allocation): %p \n", &matrix_fill);
    printf("stride addr:%p \n", &stride);
    printf("temp addr:%p \n", &temp);
    printf("i:%p \n", &i);
    printf("s:%p \n", &s);
    printf("iteration:%p \n", &iteration);
    printf("length:%p \n", &length);
    printf("width:%p \n", &width);

    // Allocate memory for matrix and matrix_fill
    matrix = (int64_t*)aligned_alloc(64, sizeof(int64_t) * length * width);
    matrix_fill = (int64_t*)aligned_alloc(64, sizeof(int64_t) * length * width);

    // Calculate and print the address ranges for matrix and matrix_fill
    printf("\nMatrix Base Address: %p\n", matrix);
    printf("Matrix End Address: %p\n", (void*)((char*)matrix + (length * width * sizeof(int64_t)) - 1));

    printf("\nMatrix_fill Base Address: %p\n", matrix_fill);
    printf("Matrix_fill End Address: %p\n", (void*)((char*)matrix_fill + (length * width * sizeof(int64_t)) - 1));

    // Initialization
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            matrix[i * width + j] = i * j;
        }
    }

    // Clear cache
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            matrix_fill[i * width + j] = i * j;
        }
    }

    // Access matrix data
    for (s = 0; s < stride; s++) {
        for (i = 0; i < iteration; i++) {
            temp = +matrix[i * stride + s];
        }
    }

    printf("\n\n");
    printf("Summation Result: %ld Number of Iteration: %d \n", temp, iteration);
    printf("Expected Cache Misses:%d\n", expect);

    // Free allocated memory
    if (matrix != NULL) free(matrix);
    if (matrix_fill != NULL) free(matrix_fill);

    printf("\n --------------------------------- End of program -------------------------\n\n");
    return 0;
}
