#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "malloc.c"

#define MAX_ALLOCATION_SIZE 1024
#define NUM_ALLOCATIONS 10000

void benchmark_malloc() {
    clock_t start, end;
    double cpu_time_used;

    printf("Benchmarking malloc():\n");

    start = clock();

    // Perform allocations
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) {
        size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
        void *ptr = malloc(size);
        if (ptr == NULL) {
            fprintf(stderr, "Allocation failed!\n");
            return;
        }
    }

   
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) {
        free(ptr);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);
}

void benchmark_custom_allocator() {
    clock_t start, end;
    double cpu_time_used;

    printf("Benchmarking custom allocator:\n");

    start = clock();

    // Perform allocations
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) {
        size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
        void *ptr = custom_malloc(size);  
        if (ptr == NULL) {
            fprintf(stderr, "Allocation failed!\n");
            return;
        }
    }

    // Perform deallocations (in reverse order)
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) {
        custom_free(ptr);  
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Run benchmarks
    benchmark_malloc();
    benchmark_custom_allocator();  
    return 0;
}
