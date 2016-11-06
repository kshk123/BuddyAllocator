#ifndef _BUDDY_ALLOCATOR_H__
#define _BUDDY_ALLOCATOR_H__

#include "stdlib.h"
#include "stdint.h"

#define MIN_ORDER 1
#define MAX_ORDER 20
#define ORDERTOSIZE(i) (1 << (i)) /* Macro to convert order to size. order(i) = 2 ** i in size */

struct buddy_allocator_t_
{
	size_t memSize; /* size of the memory for this buddy allocator*/
	uint8_t maxOrder; /* maximum order for this buddy alloator */
	void * memStart; /* start of the memory pool for this buddy allocator */
	void *  memPool[MAX_ORDER + 1]; /* pool of ordered size memory blocks */
};

typedef struct buddy_allocator_t_ buddy_allocator_t;

struct node_t /* Data structure to maintain list of buddy allocators */
{
	buddy_allocator_t * buddy_alloc;
	struct node_t * next;	
};

typedef struct node_t node;

node  * buddyList; /* The list of buddy allocators currently present */

buddy_allocator_t *buddy_allocator_create(void *raw_memory, size_t raw_memory_size);
void buddy_allocator_destroy(buddy_allocator_t *buddy_allocator);
void *buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size);
void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr);
void initBuddyAlloc(buddy_allocator_t *buddy_allocator);
void * alloc_buddy(buddy_allocator_t *buddy_allocator, size_t size);
void * findBuddy(buddy_allocator_t * buddyMem, void * block, uint8_t order);

#endif

