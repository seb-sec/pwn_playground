#pragma once

#define MAX_CHUNKS 100

void *allocated_chunks[MAX_CHUNKS];
unsigned int alloc_idx;

/* initialises allocated_chunks array and alloc_idx
 * this should be called before calling other functions below
 */
void init_heap_module();

/* allocates a new chunk in the heap according to a user supplied size
 * fails if the maximum allocations defined by MAX_CHUNKS has been reached
 */
void new_chunk();

/* make an unbounded write to a chunk in the array */
void write_chunk();

/* calls free on a chunk in the array */
void free_chunk();

/* prints at the given chunk address with a %s format specifier */
void print_chunk();

void print_heap_menu();

void heap_menu();

/* leak the address of a heap chunk from the array- it must be allocated */
void leak_heap();
