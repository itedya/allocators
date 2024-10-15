#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "allocators.h"

// #arena_allocator #create_arena_allocator
void test_if_arena_allocator_creates_itself_successfully();

// #arena_allocator #allocate_memory_with_arena_allocator
void test_if_arena_allocator_allocates_memory();
void test_if_arena_allocator_resizes_allocated_items_array_when_necessary_and_executing_allocate_function();
void test_if_arena_allocator_does_not_allocate_memory_when_passed_0_bytes_as_a_size();

// #arena_allocator #deallocate_memory_with_arena_allocator
void test_if_arena_allocator_does_not_deallocate_memory();

// #normal_allocator #create_normal_allocator
void test_if_normal_allocator_creates_itself_successfully();

// #normal_allocator #deallocate_memory_with_normal_allocator
void test_if_normal_allocator_deallocates_memory_successfully();
void test_if_normal_allocator_does_not_deallocate_memory_that_is_not_owned_by_this_allocator();
void test_if_normal_allocator_does_not_deallocate_memory_when_passed_null_ptr();

int main() {
	test_if_arena_allocator_creates_itself_successfully();
	test_if_arena_allocator_allocates_memory();
	test_if_arena_allocator_resizes_allocated_items_array_when_necessary_and_executing_allocate_function();
	test_if_arena_allocator_does_not_deallocate_memory();
	test_if_arena_allocator_does_not_allocate_memory_when_passed_0_bytes_as_a_size();
	test_if_normal_allocator_creates_itself_successfully();
	test_if_normal_allocator_deallocates_memory_successfully();
	test_if_normal_allocator_does_not_deallocate_memory_that_is_not_owned_by_this_allocator();
	test_if_normal_allocator_does_not_deallocate_memory_when_passed_null_ptr();
}

void fail_if(char* filename, int line, bool result, char* fmt, ...) {
	if (!result) return;

	va_list args;
	va_start(args, fmt);

	bool is_formatting = false;
	bool printing_formatted = false;

	printf("[FAIL] ");

	while (*fmt != '\0') {
		if (!is_formatting && *fmt == '%') {
			is_formatting = true;
		} else if (is_formatting) {
			if (*fmt == 's') {
				char* s = va_arg(args, char*);
	            printf("%s\n", s);
	            printing_formatted = true;
			} else if (*fmt == 'd') {
	            int i = va_arg(args, int);
	            printf("%d\n", i);
	            printing_formatted = true;
	        } else if (*fmt == 'c') {
	            int c = va_arg(args, int);
	            printf("%c\n", c);
	        	printing_formatted = true;
	        } else if (*fmt == 'f') {
	            double d = va_arg(args, double);
	            printf("%f\n", d);
	            printing_formatted = true;
	        } else if (*fmt == 'p') {
				size_t p = va_arg(args, size_t);
	            printf("%p\n", (void*) p);
	            printing_formatted = true;
	        }

	        is_formatting = false;
	    }
        
	    if (!printing_formatted && !is_formatting) {
        	printf("%c", *fmt);
	    }

        printing_formatted = false;
        ++fmt;
    }

    printf("(%s:%d)\n", filename, line);

    va_end(args);

	exit(1);
}

// #arena_allocator #create_arena_allocator
void test_if_arena_allocator_creates_itself_successfully() {
	printf("[IN PROGRESS] test if arena allocator creates istelf successfully\n");

	arena_allocator* arena = create_arena_allocator();
	
	if (arena->allocated_items == 0) {
		printf("[FAIL] Expected arena->allocated_items to not be equal to zero. Received %p. (%s:%d)\n", arena->allocated_items, __FILE__, __LINE__);
		exit(1);
	}
	
	if (arena->allocated_items_len != 0) {
		printf("[FAIL] Expected arena->allocated_items_len to be equal to zero. Received %d. (%s:%d)\n", arena->allocated_items_len, __FILE__, __LINE__);
		exit(1);
	}
	
	if (arena->allocated_items_cap != 2) {
		printf("[FAIL] Expected arena->allocated_items_cap to be equal to 2. Received %d. (%s:%d)\n", arena->allocated_items_cap, __FILE__, __LINE__);
		exit(1);
	}
	
	destroy_arena_allocator(arena);
	
	printf("[PASS] test if arena allocator creates istelf successfully\n");
}

// #arena_allocator #allocate_memory_with_arena_allocator
void test_if_arena_allocator_allocates_memory() {
	printf("[IN PROGRESS] test if arena allocator allocates memory\n");
	
	arena_allocator* arena = create_arena_allocator();
	
	void* ptr = allocate_memory_with_arena_allocator(arena, 8);
	
	if (ptr == 0) {
		printf("[FAIL] Expected returned pointer to not be equal to 0. (%s:%d)\n", __FILE__, __LINE__);
		exit(1);
	}
	
	if (arena->allocated_items_len != 1) {
		printf("[FAIL] Expected length of allocated items array to be equal to 1. Received: %d. (%s:%d)\n", arena->allocated_items_len, __FILE__, __LINE__);
		exit(1);
	}
	
	if (arena->allocated_items_cap != 2) {
		printf("[FAIL] Expected capacity of allocated items array to be equal to 2. Received %d. (%s:%d)\n", arena->allocated_items_cap, __FILE__, __LINE__);
		exit(1);
	}
	
	if (arena->allocated_items[0] != ptr) {
		printf("[FAIL] Expected first item of allocated items array to be equal to %p, received %p. (%s:%d)\n", ptr, arena->allocated_items[0], __FILE__, __LINE__);
		exit(1);
	}
	
	destroy_arena_allocator(arena);
	
	printf("[PASS] test if arena allocator allocates memory\n");
}

// #arena_allocator #allocate_memory_with_arena_allocator
void test_if_arena_allocator_resizes_allocated_items_array_when_necessary_and_executing_allocate_function() {
	printf("[IN PROGRESS] test if arena allocator resizes allocated items array when necessary and executing allocate function\n");
	
	arena_allocator* arena = create_arena_allocator();
	
	for (int i = 0; i < 100; i++) {
		void* ptr = allocate_memory_with_arena_allocator(arena, 8);
		
		if (arena->allocated_items[i] != ptr) {
			printf("[FAIL] Expected last entry of allocated items to be equal to %p, received %p. (%s:%d)\n", ptr, arena->allocated_items[i], __FILE__, __LINE__);
			exit(1);
		}
		
		if (arena->allocated_items_len != i + 1) {
			printf("[FAIL] Expected length of allocated items to be equal to %d, received %d. (%s:%d)\n", i + 1, arena->allocated_items_len, __FILE__, __LINE__);
			exit(1);
		}
		
		int cap = 2;
		
		while (i + 1 > cap) {
			cap *= 2;
		}
		
		if (arena->allocated_items_cap != cap) {
			printf("[FAIL] Expected capacity of allocated items to be equal to %d, received %d. (%s:%d)\n", cap, arena->allocated_items_cap, __FILE__, __LINE__);
			exit(1);
		}
	}
	
	destroy_arena_allocator(arena);
	
	printf("[PASS] test if arena allocator resizes allocated items array when necessary and executing allocate function\n");
}

// #arena_allocator #deallocate_memory_with_arena_allocator
void test_if_arena_allocator_does_not_deallocate_memory() {
	printf("[IN PROGRESS] test if arena allocator does not deallocate memory\n");
	
	arena_allocator* arena = create_arena_allocator();
	
	void* allocated_ptrs[100];
	
	for (int i = 0; i < 100; i++) {
		allocated_ptrs[i] = allocate_memory_with_arena_allocator(arena, 8);
	}
	
	for (int i = 99; i >= 0; i--) {
		deallocate_memory_with_arena_allocator(arena, allocated_ptrs[i]);
		
		if (arena->allocated_items_cap != 128) {
			printf("[FAIL] capacity of allocated items array is not equal to 128, received %d. (%s:%d)", arena->allocated_items_cap, __FILE__, __LINE__);
			exit(1);
		}
		
		if (arena->allocated_items_len != 100) {
			printf("[FAIL] allocated items array length is not equal to 100, received %d. (%s:%d)\n", arena->allocated_items_len, __FILE__, __LINE__);
			exit(1);
		}
		
		char exists_in_internal_arr = 0;
		
		for (int j = 0; j < arena->allocated_items_len; j++) {
			if (arena->allocated_items[j] == allocated_ptrs[i]) {
				exists_in_internal_arr = 1;
				break;
			}
		}
		
		if (exists_in_internal_arr == 0) {
			printf("[FAIL] allocated pointer does not exist in allocated_items array. (%s:%d)\n", __FILE__, __LINE__);
			exit(1);
		}
	}

	destroy_arena_allocator(arena);
	
	printf("[PASS] test if arena allocator does not deallocate memory\n");
}

// #arena_allocator #allocate_memory_with_arena_allocator
void test_if_arena_allocator_does_not_allocate_memory_when_passed_0_bytes_as_a_size() {
	printf("[IN PROGRESS] test if arena allocator does not allocate memory when passed 0 bytes as a size\n");

	arena_allocator* arena = create_arena_allocator();

	void* ptr = allocate_memory_with_arena_allocator(arena, 0);

	if (ptr != 0) {
		printf("[FAIL] Expected returned pointer to be equal to 0, received %p. (%s:%d)\n", ptr, __FILE__, __LINE__);
		exit(1);
	}

	if (arena->allocated_items_cap != 2) {
		printf("[FAIL] Expected capacity of allocated items to be equal to 2, received %d. (%s:%d)\n", arena->allocated_items_cap, __FILE__, __LINE__);
		exit(1);
	}

	if (arena->allocated_items_len != 0) {
		printf("[FAIL] Expected length of allocated items to be equal to 0, received %d. (%s:%d)\n", arena->allocated_items_len, __FILE__, __LINE__);
		exit(1);
	}

	destroy_arena_allocator(arena);

	printf("[PASS] test if arena allocator does not allocate memory when passed 0 bytes as a size\n");
}

// #normal_allocator #create_normal_allocator
void test_if_normal_allocator_creates_itself_successfully() {
	printf("[IN PROGRESS] test if normal allocator creates itself successfully\n");

	normal_allocator* allocator = create_normal_allocator();

	if (allocator == 0) {
		printf("[FAIL] Expected returned allocator to not be equal to 0, received %p. (%s:%d)\n", 
			allocator, __FILE__, __LINE__);
		exit(1);
	}

	if (allocator->arena_allocator == 0) {
		printf("[FAIL] Expected arena allocator inside normal allocator to not be equal to 0, received %p. (%s:%d)\n", 
			allocator->arena_allocator, __FILE__, __LINE__);
		exit(1);
	}

	destroy_normal_allocator(allocator);

	printf("[PASS] test if normal allocator creates itself successfully\n");
}

// #normal_allocator #deallocate_memory_with_normal_allocator
void test_if_normal_allocator_deallocates_memory_successfully() {
	printf("[IN PROGRESS] test if normal allocator deallocates memory successfully\n");

	normal_allocator* allocator = create_normal_allocator();

	void* ptr1 = allocate_memory_with_normal_allocator(allocator, 8);
	void* ptr2 = allocate_memory_with_normal_allocator(allocator, 8);
	void* ptr3 = allocate_memory_with_normal_allocator(allocator, 8);
	
	fail_if(__FILE__, __LINE__, ptr1 == NULL, "Expected ptr1 to not be null.");
	fail_if(__FILE__, __LINE__, ptr2 == NULL, "Expected ptr2 to not be null.");
	fail_if(__FILE__, __LINE__, ptr3 == NULL, "Expected ptr3 to not be null.");

	deallocate_memory_with_normal_allocator(allocator, ptr1);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items_len != 2, 
		"Expected length of allocated items array to be equal to 2, received %d", 
		allocator->arena_allocator->allocated_items_len);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items_cap != 4, 
		"Expected capacity of allocated items array to be equal to 4, received %d", 
		allocator->arena_allocator->allocated_items_cap);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items[0] != ptr2,
		"Expected first allocated item to be equal to %p, received %p.", 
		ptr2, allocator->arena_allocator->allocated_items[0]);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items[1] != ptr3,
		"Expected first allocated item to be equal to %p, received %p.", 
		ptr3, allocator->arena_allocator->allocated_items[1]);

	deallocate_memory_with_normal_allocator(allocator, ptr2);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items_len != 1, 
		"Expected length of allocated items array to be equal to 1, received %d", 
		allocator->arena_allocator->allocated_items_len);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items_cap != 4, 
		"Expected capacity of allocated items array to be equal to 4, received %d", 
		allocator->arena_allocator->allocated_items_cap);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items[0] != ptr3,
		"Expected first allocated item to be equal to %p, received %p.", 
		ptr3, allocator->arena_allocator->allocated_items[0]);

	deallocate_memory_with_normal_allocator(allocator, ptr3);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items_len != 0, 
		"Expected length of allocated items array to be equal to 0, received %d", 
		allocator->arena_allocator->allocated_items_len);

	fail_if(__FILE__, __LINE__, allocator->arena_allocator->allocated_items_cap != 4, 
		"Expected capacity of allocated items array to be equal to 4, received %d", 
		allocator->arena_allocator->allocated_items_cap);

	destroy_normal_allocator(allocator);

	printf("[PASS] test if normal allocator deallocates memory successfully\n");
}

// #normal_allocator #deallocate_memory_with_normal_allocator
void test_if_normal_allocator_does_not_deallocate_memory_that_is_not_owned_by_this_allocator() {
	printf("[IN PROGRESS] test if normal allocator does not deallocate memory that is not owned by this allocator\n");

	normal_allocator* allocator = create_normal_allocator();

	void* ptr1 = allocate_memory_with_normal_allocator(allocator, 8);
	void* ptr2 = allocate_memory_with_normal_allocator(allocator, 8);
	void* ptr3 = allocate_memory_with_normal_allocator(allocator, 8);

	normal_allocator* second_allocator = create_normal_allocator();

	void* ptr4 = allocate_memory_with_normal_allocator(second_allocator, 8);
	void* ptr5 = allocate_memory_with_normal_allocator(second_allocator, 8);

	deallocate_memory_with_normal_allocator(second_allocator, ptr1);
	deallocate_memory_with_normal_allocator(second_allocator, ptr2);
	deallocate_memory_with_normal_allocator(second_allocator, ptr3);

	if (second_allocator->arena_allocator->allocated_items_len != 2) {
		printf("[FAIL] Expected length of allocated items array to be equal to 2, received %d. (%s:%d)\n", 
			second_allocator->arena_allocator->allocated_items_len, __FILE__, __LINE__);
		exit(1);
	}

	if (second_allocator->arena_allocator->allocated_items_cap != 2) {
		printf("[FAIL] Expected capacity of allocated items array to be equal to 2, received %d. (%s:%d)\n", 
			second_allocator->arena_allocator->allocated_items_cap, __FILE__, __LINE__);
		exit(1);
	}

	int ptr4_exists = 0;
	int ptr5_exists = 0;

	for (int i = 0; i < second_allocator->arena_allocator->allocated_items_len; i++) {
		if (second_allocator->arena_allocator->allocated_items[i] == ptr4) {
			ptr4_exists = 1;
		}

		if (second_allocator->arena_allocator->allocated_items[i] == ptr5) {
			ptr5_exists = 1;
		}
	}

	if (ptr4_exists == 0) {
		printf("[FAIL] Expected ptr4 to exist in allocated items array in second allocator.\n");
		exit(1);
	}

	if (ptr5_exists == 0) {
		printf("[FAIL] Expected ptr5 to exist in allocated items array in second allocator.\n");
		exit(1);
	}

	destroy_normal_allocator(allocator);
	destroy_normal_allocator(second_allocator);

	printf("[PASS] test if normal allocator does not deallocate memory that is not owned by this allocator\n");
}

// #normal_allocator #deallocate_memory_with_normal_allocator
void test_if_normal_allocator_does_not_deallocate_memory_when_passed_null_ptr() {
	printf("[IN PROGRESS] test if normal allocator does not deallocate memory when passed null ptr\n");

	normal_allocator* allocator = create_normal_allocator();

	void* ptr = allocate_memory_with_normal_allocator(allocator, 8);

	deallocate_memory_with_normal_allocator(allocator, 0);

	if (allocator->arena_allocator->allocated_items_cap != 2) {
		printf("[FAIL] Expected allowed items capacity to be equal to 2, received %d. (%s:%d)\n",
			allocator->arena_allocator->allocated_items_cap, __FILE__, __LINE__);
		exit(1);
	}

	if (allocator->arena_allocator->allocated_items_len != 1) {
		printf("[FAIL] Expected allowed items length to be equal to 1, received %d. (%s:%d)\n", 
			allocator->arena_allocator->allocated_items_len, __FILE__, __LINE__);
		exit(1);
	}

	if (allocator->arena_allocator->allocated_items[0] != ptr) {
		printf("[FAIL] Expected first item of allowed items to be equal to %p, received %p. (%s:%d)\n", 
			ptr, allocator->arena_allocator->allocated_items[0], __FILE__, __LINE__);
		exit(1);
	}

	destroy_normal_allocator(allocator);

	printf("[PASS] test if normal allocator does not deallocate memory when passed null ptr\n");
}