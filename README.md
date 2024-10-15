# Allocators

Two simple allocators: 
 - arena
 - normal - arena with classic deallocation, something like malloc, but you can deallocate everything with destroy function (like arena)

Documentation is in header file.

Function names are loooong, but they explain everything the function does (self-documentation).
If you want something shorter, you can do:
```c
void dealloc_normal(normal_allocator* normal, void* ptr) {
  deallocate_memory_with_normal_allocator(normal, ptr);
}
```
it should do the trick, but imo it's not necessary.

Arena has a deallocation function, but it's a noop. It's done for the cases when you must have allocator-agnostic type code and all the callbacks must be provided. 
