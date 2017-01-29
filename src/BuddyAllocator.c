#include <string.h>
#include <stdio.h>
#include "BuddyAllocator.h"

buddy_allocator_t *buddy_allocator_create(size_t raw_memory_size)
{
    buddy_allocator_t * newBuddy = (buddy_allocator_t *)malloc(sizeof(buddy_allocator_t));
    if(NULL == newBuddy)
    {
        printf("Failed to allocate memory.\n");
        return NULL;
    }
    memset(newBuddy, 0, sizeof(buddy_allocator_t));
    newBuddy->memStart = malloc(raw_memory_size);
    if(NULL == newBuddy->memStart)
    {
        printf("Failed to allocate memory.\n");
        free(newBuddy);
        return NULL;
    }
    memset(newBuddy->memStart, 0, raw_memory_size);
    newBuddy->memSize  = raw_memory_size;
    if(!initBuddyAlloc(newBuddy))
    {
        free(newBuddy->memStart);
        free(newBuddy);
        return NULL;
    }

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
        free(buddy_allocator->memStart);
        buddy_allocator->memStart = NULL;
        free(buddy_allocator);
        buddy_allocator = NULL;
        free(ptr);
        ptr = NULL;
    }
}

int initBuddyAlloc(buddy_allocator_t *buddy_allocator)
{
    uint8_t order = 0;
    while(ORDERTOSIZE(order) < buddy_allocator->memSize)
    {
        order++;
    }

    if(order > MAX_ORDER)
    {
        printf("Cannot create buddy allocator as backup memory is more than can be supported\n");
        return FAIL;
    }

    buddy_allocator->maxOrder = order;
    memset(buddy_allocator->memPool, 0, MAX_ORDER + 1);
    buddy_allocator->memPool[order] = buddy_allocator->memStart;
    return SUCCESS;
}

void * buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size, uint8_t * order)
{
    node * ptr = buddyList;
    while(ptr != NULL && ptr->buddy_alloc != buddy_allocator)
    {
        ptr = ptr->next;
    }

    if(NULL == ptr)
        return NULL;

    return alloc_buddy(buddy_allocator, size, order);
}

void * alloc_buddy(buddy_allocator_t *buddy_allocator, size_t size, uint8_t * order)
{
    uint8_t curr = 0;
    void * block = NULL;
    void * buddy = NULL;

    while (ORDERTOSIZE(curr) < size + 1)
    { 
        curr++;
    }

    if(curr < MIN_ORDER)
    {
        *order = MIN_ORDER;
        curr  = MIN_ORDER;
    }
    else
    {
        *order = curr;
    }

    while(curr <= buddy_allocator->maxOrder)
    {
        if (buddy_allocator->memPool[curr])
            break;
        curr++;
    }

    if (curr > buddy_allocator->maxOrder)
    {
        printf("Memory allocation fail!!! No free memory available.\n");
        *order = 0;
        return NULL;
    }

    block = buddy_allocator->memPool[curr];
    buddy_allocator->memPool[curr] = *(void**)buddy_allocator->memPool[curr];

    while (curr-- > *order) 
    {
        buddy = findBuddy(buddy_allocator, block, curr);
        buddy_allocator->memPool[curr] = buddy;
    }

    return block;
}

void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr, uint8_t order)
{
    if(NULL == ptr)
        return;

    uint8_t curr = 0;
    void *  buddy= NULL;
    void ** memPtr = NULL;

    curr = order;

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

