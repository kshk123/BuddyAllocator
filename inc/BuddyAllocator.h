#ifndef BUDDY_ALLOCATOR_H_
#define BUDDY_ALLOCATOR_H_

#include "stdlib.h"
#include "stdint.h"

#define SUCCESS 1
#define FAIL    0

#define MIN_ORDER 4
#define MAX_ORDER 34
#define ORDERTOSIZE(i) (1 << (i)) /* Macro to convert order to size. order(i) = 2 ** i in size */

struct buddy_allocator_t_
{
    size_t memSize; /* size of the memory for this buddy allocator*/
    uint8_t maxOrder; /* maximum order for this buddy alloator */
    void * memStart; /* start of the memory pool for this buddy allocator */
    void * memPool[MAX_ORDER + 1]; /* pool of ordered size memory blocks */
};

typedef struct buddy_allocator_t_ buddy_allocator_t;

struct node_t /* Data structure to maintain list of buddy allocators */
{
    buddy_allocator_t * buddy_alloc;
    struct node_t * next;   
};

typedef struct node_t node;

node  * buddyList; /* The list of buddy allocators currently present */

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
void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr, uint8_t order);

int initBuddyAlloc(buddy_allocator_t *buddy_allocator);
void * alloc_buddy(buddy_allocator_t *buddy_allocator, size_t size, uint8_t * order);
void * findBuddy(buddy_allocator_t * buddyMem, void * block, uint8_t order);

#endif

