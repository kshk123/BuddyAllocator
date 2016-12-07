#include "BuddyAllocator.h"
#include "string.h"
#include "stdio.h"

int main()
{
	int size = 500;
	void * mem = (void *)malloc(size);

	buddy_allocator_t * buddyT = buddy_allocator_create(mem, size);
	void * ptr = buddy_allocator_alloc(buddyT, 30);
	memcpy(ptr, "abcdef", 6);
	printf("%s\n", (char *)ptr);
	buddy_allocator_free(buddyT, ptr);
	buddy_allocator_destroy(buddyT);

	return 0;
}

buddy_allocator_t *buddy_allocator_create(void *raw_memory, size_t raw_memory_size)
{
	buddy_allocator_t * newBuddy = (buddy_allocator_t *)malloc(sizeof(buddy_allocator_t));
	newBuddy->memStart = raw_memory;
	newBuddy->memSize  = raw_memory_size;
	initBuddyAlloc(newBuddy);

	node * newNode = (node *)malloc(sizeof(node));
	newNode->buddy_alloc = newBuddy;
	newNode->next = NULL;
	
	if(NULL == buddyList)
		buddyList = newNode;
	else
	{
		node * ptr = buddyList;
		while(ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = newNode;
	}

	return newBuddy;
}

void buddy_allocator_destroy(buddy_allocator_t *buddy_allocator)
{
	node * ptr = buddyList;
	node * prev = NULL;

	while(ptr != NULL && ptr->buddy_alloc != buddy_allocator)
	{
		prev = ptr;
		ptr = ptr->next;
	}

	if(ptr != NULL)
	{
		if(NULL == prev) /* deleting at the head */
		{
			buddyList = ptr->next;
		}
		else
		{
			prev->next = ptr->next;
		}
		free(buddy_allocator);
		buddy_allocator = NULL;
		free(ptr);
	}
}

void initBuddyAlloc(buddy_allocator_t *buddy_allocator)
{
	uint8_t order = 0;
	while(ORDERTOSIZE(order) < buddy_allocator->memSize)
	{
		order++;
	}

	buddy_allocator->maxOrder = order;
	memset(buddy_allocator->memPool, 0, MAX_ORDER);
	buddy_allocator->memPool[order] = buddy_allocator->memStart;
}

void * buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size)
{
	node * ptr = buddyList;
	while(ptr != NULL && ptr->buddy_alloc != buddy_allocator)
	{
		ptr = ptr->next;
	}

	if(NULL == ptr)
		return NULL;

	return alloc_buddy(buddy_allocator, size);
}

void * alloc_buddy(buddy_allocator_t *buddy_allocator, size_t size)
{
	uint8_t curr = 0;
	uint8_t order = 0;
  	void * block = NULL;
	void * buddy = NULL;

  	while (ORDERTOSIZE(curr) < size + 1)
	{ 
    	curr++;
	}

	if(curr < MIN_ORDER)
	{
		order = MIN_ORDER;
		curr  = MIN_ORDER;
	}
	else
	{
		order = curr;
	}

	while(curr <= buddy_allocator->maxOrder)
	{
		if (buddy_allocator->memPool[curr])
            break;
		curr++;
	}

  	if (curr > buddy_allocator->maxOrder)
		return NULL;

  	block = buddy_allocator->memPool[curr];
  	buddy_allocator->memPool[curr] = *(void**)buddy_allocator->memPool[curr];

  	while (curr-- > order) 
	{
    	buddy = findBuddy(buddy_allocator, block, curr);
    	buddy_allocator->memPool[curr] = buddy;
  	}

	*((uint8_t*) (block - 1)) = order;
  	return block;
}

void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr)
{
	if(NULL == ptr)
		return;

	uint8_t curr = 0;
  	void *  buddy= NULL;
  	void ** memPtr = NULL;

  	curr = *((uint8_t*) (ptr - 1));

  	for (;; curr++) 
	{
    	buddy = findBuddy(buddy_allocator, ptr, curr);
    	memPtr = &(buddy_allocator->memPool[curr]);

    	while ((*memPtr != NULL) && (*memPtr != buddy))
      		memPtr = (void **) *memPtr;

    	if (*memPtr != buddy) 
		{
      		*(void**) ptr = buddy_allocator->memPool[curr];
      		buddy_allocator->memPool[curr] = ptr;
      		return;
    	}
    	ptr = (ptr < buddy) ? ptr : buddy;
    	*memPtr = *(void**) *memPtr;
  	}
}

void * findBuddy(buddy_allocator_t * buddyMem, void * block, uint8_t order)
{
	int offset = block - buddyMem->memStart;
	return (void *)((offset ^ (1 << order)) + buddyMem->memStart); 
}

