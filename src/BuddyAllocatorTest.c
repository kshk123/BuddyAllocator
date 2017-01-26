#include "BuddyAllocator.h"
#include "string.h"
#include "stdio.h"

int main()
{
    int size = 500;

    buddy_allocator_t * buddyT = buddy_allocator_create(size);
    if(buddyT)
    {
        uint8_t order = 0;
        uint8_t oldOrder = 0;
        void * ptr = buddy_allocator_alloc(buddyT, 30, &order);
        void * oldPtr = NULL;
        if(ptr)
        {
            memcpy(ptr, "Test Case 1", 11);
            printf("%s\n", (char *)ptr); 
            buddy_allocator_free(buddyT, ptr, order);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }

        ptr = buddy_allocator_alloc(buddyT, 300, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 2", 11);
            printf("%s\n", (char *)ptr);
            buddy_allocator_free(buddyT, ptr, order);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }

        ptr = buddy_allocator_alloc(buddyT, 300, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 3", 11);
            printf("%s\n", (char *)ptr);
            oldPtr = ptr;
            oldOrder = order;
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }

        ptr = buddy_allocator_alloc(buddyT, 250, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 4", 11);
            printf("%s\n", (char *)ptr);
            buddy_allocator_free(buddyT, ptr, order);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }
        ptr = buddy_allocator_alloc(buddyT, 150, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 5", 11);
            printf("%s\n", (char *)ptr);
            buddy_allocator_free(buddyT, ptr, order);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }
        buddy_allocator_free(buddyT, oldPtr, oldOrder);
        ptr = buddy_allocator_alloc(buddyT, 100, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 6", 11);
            printf("%s\n", (char *)ptr);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        } 
        ptr = buddy_allocator_alloc(buddyT, 100, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 7", 11);
            printf("%s\n", (char *)ptr);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }
        ptr = buddy_allocator_alloc(buddyT, 100, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 8", 11);
            printf("%s\n", (char *)ptr);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }
        ptr = buddy_allocator_alloc(buddyT, 100, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 9", 11);
            printf("%s\n", (char *)ptr);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }
        ptr = buddy_allocator_alloc(buddyT, 100, &order);
        if(ptr)
        {
            memcpy(ptr, "Test Case 10", 12);
            printf("%s\n", (char *)ptr);
        }
        else
        {
            printf("Cannot allocate memory from buddy allocator\n");
        }    
        buddy_allocator_destroy(buddyT);
    }
    else
    {
        printf("Buddy Creation failed\n");
    }

    return 0;
}

