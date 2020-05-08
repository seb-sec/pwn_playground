#include "include/pwn_playground.h"
#include "include/heap_module.h"
#include <stdio.h>
#include <stdlib.h>

void init_heap_module() {
	for (int i = 0; i < MAX_CHUNKS; i++) {
		allocated_chunks[i] = NULL;
	}
	alloc_idx = 0;
}

void new_chunk() {
	/* case max allocations made */
	if (alloc_idx >= MAX_CHUNKS) {
		puts("Max allocations reached!");
		return;
	}
	puts("Enter size of chunk:");
	size_t len;
	scanf("%zu", &len);
	flush();
	void *ptr = malloc(len);
	if (ptr == NULL) {
		puts("Malloc failed!");
		exit(1);
	}
	allocated_chunks[alloc_idx] = ptr;
	printf("New chunk allocated at index %u\n", alloc_idx);
	alloc_idx++;
}

void write_chunk() {
	puts("Enter a chunk idx to write to:");
	unsigned int choice;
	scanf("%u", &choice);
	flush();
	if (choice >= alloc_idx) {
		puts("idx out of range or not yet allocated!");
		return;
	}
	printf("Enter input to chunk %u:\n", choice);
	gets((char *) allocated_chunks[choice]);
}

void free_chunk() {
	puts("Enter a chunk idx to free:");
	unsigned int choice;
	scanf("%u", &choice);
	flush();
	if (choice >= alloc_idx) {
		puts("idx out of range or not yet allocated!");
		return;
	}
	free(allocated_chunks[choice]);
	printf("Free'd chunk %u\n", choice);
}

void print_chunk() {
	puts("Enter a chunk idx to print:");
	unsigned int choice;
	scanf("%u", &choice);
	flush();
	if (choice >= alloc_idx) {
		puts("idx out of range or not yet allocated!");
		return;
	}
	printf("Chunk contents: %s\n", (char *) allocated_chunks[choice]);
}

void print_heap_menu() {
	puts("\n--- Heap module ---");
	puts("1. Allocate chunk");
	puts("2. Write to chunk");
	puts("3. Read from chunk");
	puts("4. Free chunk");
	puts("5. Exit heap module");
	puts(">");
}

void heap_menu() {
	print_heap_menu();
	int choice;
	scanf("%d", &choice);
	flush();
	switch (choice) {
		case 1:
			new_chunk();
			break;
		case 2:
			write_chunk();
			break;
		case 3:
			print_chunk();
			break;
		case 4:
			free_chunk();
			break;
		case 5:
			return;
		default:
			puts("Invalid option");
	}
}

void leak_heap() {
	puts("Enter a chunk idx to leak (must be allocated):");
	unsigned int choice;
	scanf("%u", &choice);
	flush();
	if (choice >= alloc_idx) {
		puts("idx out of range or not yet allocated!");
		return;
	}
	printf("Chunk address: %p\n", allocated_chunks[choice]);
}
