#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ALLOCS 1000
#define MAX_BLOCK_SIZE 1024

// Structure to track memory blocks
typedef struct {
    void* ptr;
    size_t size;
} MemoryBlock;

void* custom_alloc_1(size_t size) {
    return malloc(size);
}

void custom_free_1(void* ptr) {
    free(ptr);
}

void* custom_alloc_2(size_t size) {
    return malloc(size);
}

void custom_free_2(void* ptr) {
    free(ptr);
}

#define ARENA_SIZE 1000000 

typedef struct {
    char* arena; 
    size_t offset;
} ArenaAllocator;


ArenaAllocator* init_custom_alloc_3() {
    ArenaAllocator* allocator = malloc(sizeof(ArenaAllocator));
    if (allocator == NULL) {
        return NULL; // Allocation failed
    }
    
    allocator->arena = malloc(ARENA_SIZE);
    if (allocator->arena == NULL) {
        free(allocator);
        return NULL; // Allocation failed
    }
    
    allocator->offset = 0;
    
    return allocator;
}

// Function to allocate memory using the custom allocator
void* custom_alloc_3(size_t size, ArenaAllocator* allocator) {
    if (allocator == NULL) {
        return NULL; // Allocator not initialized
    }

    // Check if there's enough space in the arena
    if (allocator->offset + size > ARENA_SIZE) {
        return NULL; // Insufficient space in the arena
    }

    // Allocate memory from the arena
    void* ptr = allocator->arena + allocator->offset;
    allocator->offset += size;

    return ptr;
}

// Function to free memory allocated by the custom allocator
void custom_free_3(void* ptr) {
   
}

// Function to clean up and deallocate the custom allocator
void cleanup_custom_alloc_3(ArenaAllocator* allocator) {
    if (allocator != NULL) {
        free(allocator->arena);
        free(allocator);
    }
}

// Custom allocator 4: Your implementation 4
void* custom_alloc_4(size_t size) {
    return malloc(size);
}

void custom_free_4(void* ptr) {
    free(ptr);
}

// Function to perform allocations using given allocator
void perform_allocations(int num_allocs, void* (*alloc_func)(size_t), void (*free_func)(void*), int* num_splits, size_t* total_allocated) {
    MemoryBlock allocated_blocks[NUM_ALLOCS];

    // Allocate memory
    for (int i = 0; i < num_allocs; i++) {
        size_t size = rand() % MAX_BLOCK_SIZE + 1;
        void* ptr = alloc_func(size);
        if (ptr == NULL) {
            printf("Allocation failed at index %d\n", i);
            break;
        }
        allocated_blocks[i].ptr = ptr;
        allocated_blocks[i].size = size;

        // Checking for splits
        if (i > 0 && (char*)allocated_blocks[i].ptr - (char*)allocated_blocks[i-1].ptr - allocated_blocks[i-1].size > 0)
            (*num_splits)++;

        *total_allocated += size;
    }

    // Free allocated memory
    for (int i = 0; i < num_allocs; i++) {
        free_func(allocated_blocks[i].ptr);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    // Initialize variables to track metrics
    int num_splits = 0;
    size_t total_allocated = 0;

    printf("\nRelative comparison of number of splits and heap growth\n");

    // Benchmark custom allocator 1
    printf("Using Custom Allocator 1:\n");
    perform_allocations(NUM_ALLOCS, custom_alloc_1, custom_free_1, &num_splits, &total_allocated);
    printf("Number of splits: %d\n", num_splits);
    printf("Total allocated memory: %zu bytes\n\n", total_allocated);

    // Reset metrics for the next allocator
    num_splits = 0;
    total_allocated = 0;

    // Benchmark custom allocator 2
    printf("Using Custom Allocator 2:\n");
    perform_allocations(NUM_ALLOCS, custom_alloc_2, custom_free_2, &num_splits, &total_allocated);
    printf("Number of splits: %d\n", num_splits);
    printf("Total allocated memory: %zu bytes\n\n", total_allocated);

    // Reset metrics for the next allocator
    num_splits = 0;
    total_allocated = 0;

    // Benchmark custom allocator 3
    printf("Using Custom Allocator 3:\n");
    ArenaAllocator* allocator_3 = init_custom_alloc_3();
    if (allocator_3 == NULL) {
        printf("Failed to initialize Custom Allocator 3\n");
        return 1;
    }
    perform_allocations(NUM_ALLOCS, (void* (*)(size_t))custom_alloc_3, custom_free_3, &num_splits, &total_allocated);
    printf("Number of splits: %d\n", num_splits);
    printf("Total allocated memory: %zu bytes\n\n", total_allocated);
    cleanup_custom_alloc_3(allocator_3);

    // Reset metrics for the next allocator
    num_splits = 0;
    total_allocated = 0;

    // Benchmark custom allocator 4
    printf("Using Custom Allocator 4:\n");
    perform_allocations(NUM_ALLOCS, custom_alloc_4, custom_free_4, &num_splits, &total_allocated);
    printf("Number of splits: %d\n", num_splits);
    printf("Total allocated memory: %zu bytes\n\n", total_allocated);

    return 0;
}
