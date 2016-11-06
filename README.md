# BuddyAllocator
Implemention of a buddy allocator with the following interface:

/**
* Create a buddy allocator
* @param raw_memory Backing memory
* @param memory_size Backing memory size
* @return the new buddy allocator
*/
buddy_allocator_t *buddy_allocator_create(void *raw_memory, size_t raw_memory_size);

/**
* Destroy a buddy allocator
* @param buddy_allocator
* /
void buddy_allocator_destroy(buddy_allocator_t *buddy_allocator);

/**
* Allocate memory
* @param buddy_allocator The buddy allocator
* @param size Size of memory to allocate
* @return pointer to the newly allocated memory, or @a NULL if out of memory
* /
void *buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size);

/**
* Deallocates a perviously allocated memory area.
* If @a ptr is @a NULL, it simply returns
* @param buddy_allocator The buddy allocator
* @param ptr The memory area to deallocate
*/
void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr);



