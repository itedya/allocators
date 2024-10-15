#include <stdlib.h>
#include <stdio.h>
#include "allocators.h"

arena_allocator* create_arena_allocator() {
	arena_allocator* arena = malloc(sizeof(arena_allocator));
	
	arena->allocated_items_len = 0;
	arena->allocated_items_cap = 2;
	arena->allocated_items = malloc(sizeof(void*) * arena->allocated_items_cap);
	
	return arena;
}

void* allocate_memory_with_arena_allocator(arena_allocator* arena, size_t size) {
	if (size == 0) {
		return 0;
	}

	int new_capacity = arena->allocated_items_cap;

	while (arena->allocated_items_len + 1 > new_capacity) {
		new_capacity *= 2;
	}
	
	if (new_capacity != arena->allocated_items_cap) {
		arena->allocated_items = realloc(arena->allocated_items, sizeof(void*) * new_capacity);
		arena->allocated_items_cap = new_capacity;
	}
		
	arena->allocated_items[arena->allocated_items_len] = malloc(size);	
	arena->allocated_items_len++;
		
	return arena->allocated_items[arena->allocated_items_len - 1];
}

void deallocate_memory_with_arena_allocator(arena_allocator* arena, void* ptr) {
	// noop
}

void destroy_arena_allocator(arena_allocator* arena) {
	for (int i = 0; i < arena->allocated_items_len; i++) {
		free(arena->allocated_items[i]);
	}
	
	free(arena->allocated_items);
	free(arena);
}

normal_allocator* create_normal_allocator() {
	normal_allocator* allocator = malloc(sizeof(normal_allocator));
	
	allocator->arena_allocator = create_arena_allocator();
	
	return allocator;
}

void* allocate_memory_with_normal_allocator(normal_allocator* allocator, size_t size) {
	return allocate_memory_with_arena_allocator(allocator->arena_allocator, size);
}

void deallocate_memory_with_normal_allocator(normal_allocator* allocator, void* ptr) {
	void** allocated_items = allocator->arena_allocator->allocated_items;
	int* allocated_items_len = &allocator->arena_allocator->allocated_items_len;
	
	if (*allocated_items_len == 0) {
		return;
	}
	
	if (*allocated_items_len == 1) {
		if (allocated_items[0] == ptr) {
			free(allocated_items[0]);
			*allocated_items_len = *allocated_items_len - 1;
		}
		
		return;
	}
	
	for (int i = 0; i < *allocated_items_len; i++) {
		if (allocated_items[i] == ptr) {
			free(allocated_items[i]);

			for (int j = i + 1; j < *allocated_items_len; j++) {
				allocated_items[j - 1] = allocated_items[j];
			}
			
			*allocated_items_len = *allocated_items_len - 1;
		}
	}
}

void destroy_normal_allocator(normal_allocator* allocator) {
	destroy_arena_allocator(allocator->arena_allocator);
	free(allocator);
}