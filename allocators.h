#ifndef allocators_h_
#define allocators_h_

// shared interface (stubs for callbacks usage)
typedef void* (*allocate_memory)(void* allocator, size_t size);
typedef void (*deallocate_memory)(void *allocator, void* ptr);
typedef void (*destroy_allocator)(void *allocator);

// arena allocator interface
struct arena_allocator {
	void** allocated_items;
	int allocated_items_len;
	int allocated_items_cap;
};

typedef struct arena_allocator arena_allocator;

arena_allocator* create_arena_allocator();
void* allocate_memory_with_arena_allocator(arena_allocator* allocator, size_t size);
void deallocate_memory_with_arena_allocator(arena_allocator* allocator, void* ptr); // noop
void destroy_arena_allocator(arena_allocator* allocator);

// normal allocator interface
struct normal_allocator {
	arena_allocator* arena_allocator;
};

typedef struct normal_allocator normal_allocator;

normal_allocator* create_normal_allocator();
void* allocate_memory_with_normal_allocator(normal_allocator* allocator, size_t size);
void deallocate_memory_with_normal_allocator(normal_allocator* allocator, void* ptr);
void destroy_normal_allocator(normal_allocator* allocator);

typedef void* (*allocate_memory_callback)(void* allocator, size_t size);
typedef void (*deallocate_memory_callback)(void* allocator, void* ptr);

struct allocator_dependency_package {
	void* allocator,
	allocate_memory_callback allocate,
	deallocate_memory_callback deallocate
};

typedef struct allocator_dependency_package allocator_dependency_package;

#endif