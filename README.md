# BuddyAllocator
Implemention of a buddy allocator for memory handling.

There is one basic limitation in the implementation, the order has to be passed
by the user when returning to the buddy allocator, and no error checking is present
if the order is not correct. This has to be enhanced so that the order is maintained
by the allocator itself, without having the user to pass it.

The Makefile provides option to build a shared library (libBuddy.so) using the command
'make release', which can then be linked to an executable.
The Makefile also provides an option to build a test executable (buddyTest) using the
command 'make test', which can be used to perform various tests.

The various API's provided are

/**
* Create a buddy allocator
* @param memory_size Backing memory size
* @return the new buddy allocator
*/
buddy_allocator_t *buddy_allocator_create(size_t raw_memory_size);

/**
* Destroy a buddy allocator
* @param buddy_allocator
*/
void buddy_allocator_destroy(buddy_allocator_t *buddy_allocator);

/**
* Allocate memory
* @param buddy_allocator The buddy allocator
* @param size Size of memory to allocate
* @param order The order in the buddy allocator. This has to be provided at the time of freeing.
* @return pointer to the newly allocated memory, or @a NULL if out of memory
*/
void *buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size, uint8_t * order);

/**
* Deallocates a perviously allocated memory area.
* If @a ptr is @a NULL, it simply returns
* @param buddy_allocator The buddy allocator
* @param ptr The memory area to deallocate
* @param order The order of memory area in the buddy allocator
*/
void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr);



