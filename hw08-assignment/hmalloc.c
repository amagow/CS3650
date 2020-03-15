
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <assert.h>

#include "hmalloc.h"

/*
  typedef struct hm_stats {
  long pages_mapped;
  long pages_unmapped;
  long chunks_allocated;
  long chunks_freed;
  long free_length;
  } hm_stats;
*/

typedef struct free_block
{
    size_t size;
    struct free_block *next;
} free_block;

const size_t PAGE_SIZE = 4096;
static hm_stats stats; // This initializes the stats to 0.
static free_block *free_head;

long free_list_length()
{
    // TODO: Calculate the length of the free list.
    long len = 0;
    free_block *curr = free_head;
    while (curr)
    {
        curr = curr->next;
        ++len;
    }
    return len;
}

hm_stats *
hgetstats()
{
    stats.free_length = free_list_length();
    return &stats;
}

void hprintstats()
{
    stats.free_length = free_list_length();
    fprintf(stderr, "\n== husky malloc stats ==\n");
    fprintf(stderr, "Mapped:   %ld\n", stats.pages_mapped);
    fprintf(stderr, "Unmapped: %ld\n", stats.pages_unmapped);
    fprintf(stderr, "Allocs:   %ld\n", stats.chunks_allocated);
    fprintf(stderr, "Frees:    %ld\n", stats.chunks_freed);
    fprintf(stderr, "Freelen:  %ld\n", stats.free_length);
}

static size_t
div_up(size_t xx, size_t yy)
{
    // This is useful to calculate # of pages
    // for large allocations.
    size_t zz = xx / yy;

    if (zz * yy == xx)
    {
        return zz;
    }
    else
    {
        return zz + 1;
    }
}

//Sort list by address to remove bottleneck of O(n^2)
void insert_free_node(free_block *node)
{
    // if (!free_head)
    // {
    //     free_head->next = block;
    //     return;
    // }
    // free_block *cur = free_head;
    // free_block *prev = 0;
    // while (cur)
    // {
    //     if ((void *)cur > (void *)block)
    //     {
    //         if (((void *)prev + prev->size == (void *)block) && ((void *)block + block->size == (void *)cur))
    //         {
    //             prev->size = prev->size + block->size + cur->size;
    //             prev->next = cur->next;
    //         }
    //         else if ((void *)prev + prev->size == (void *)block)
    //         {
    //             prev->size = prev->size + block->size;
    //         }
    //         else if ((void *)block + block->size == (void *)cur)
    //         {
    //             block->size = block->size + cur->size;
    //             block->next = cur->next;
    //             if (prev)
    //             {
    //                 prev->next = block;
    //             }
    //             else
    //             {
    //                 free_head = block;
    //             }
    //         }
    //         else
    //         {
    //             block->next = cur;
    //             if (prev)
    //             {
    //                 prev->next = block;
    //             }
    //             else
    //             {
    //                 free_head = block;
    //             }
    //         }
    //         break;
    //     }
    //     prev = cur;
    //     cur = cur->next;
    // }
     // if the list is empty, just insert it
  if (free_head == 0) {
    free_head= node;
    return;
  }

  free_block* cur = free_head;
  free_block* prev = 0;
  while (cur != 0) {
    // if the current node is greater than the given address,
    // insert the given node after the previous node
    if ((void*) cur > (void*) node) {
      //TODO: tring to coalesce on insertion... could be wrong.
      // commented out old stuff so if all fails, just uncomment that and delete whatever else here

      // Get the size of the previous node, unless it is null
      size_t prev_size = 0;
      if (prev != 0) {
        prev_size = prev->size;
      }

      // The given node is adjacent to the nodes on both sides of it (prev and cur)
      // Need to coalesce on both sides
      if (((void*) prev + prev_size == (void*) node) && ((void*) node + node->size == (void*) cur)) {
        // basically, discard the given node and the current node
        // just keep the previous node, and adjust size accordingly
        prev->size = prev->size + node->size + cur->size;
        prev->next = cur->next;
      }

      // The given node is adjacent to the end of the previous node
      // Need to coalesce
      else if ((void*) prev + prev_size == (void*) node) {
        // just increase the size of the previous node to include the given node
        // basically, discard the given node b/c it is now part of the prev node
        prev->size = prev->size + node->size;
      }

      // The given node is adjacent to the beginning of the current node
      // Need to coalesce
      else if ((void*) node + node->size == (void*) cur) {
        // increase the size of the given node to include the current node size
        node->size = node->size + cur->size;

        // basically, discard the current node and replace it with the given node
        if (prev != 0) {
          prev->next = node; //point the previous node to the given node if prevous is not null
        }
        node->next = cur->next; //point the given node to the current's next node
      }

      // No coalescing necessary because the given node is not adjacent to any existing blocks
      // Just insert the node
      else {
        // set the previous' next node to the given node if previous is not null
        if (prev != 0) {
          prev->next = node;
        }
        // set the given node's next to the current node
        node->next = cur;
      }

      // If the previous node was null, then we are inserting at the beginning of the free list
      // So we need to update free_list_head
      if (prev == 0) {
        free_list_head = node;
      }

      // break because the node has been inserted or coalesced
      break;

      //TODO delete below
      /*
      // set the previous' next node to the given node
      prev->next = node;
      // set the given node's next to the current node
      node->next = cur;
      break;
      */
    }
    prev = cur; // set the prev node to the current node
    cur = cur->next; // set the current node to the next node
  }
}

/*
To allocate memory B bytes of memory, you should first add sizeof(size_t) to B
to make space to track the size of the block, and then:
For requests with (B < 1 page = 4096 bytes):
- See if there's a big enough block on the free list. If so, select the first one and remove it from the list.
- If you don't have a block, mmap a new block (1 page = 4096).
- If the block is bigger than the request, and the leftover is big enough to store a free list cell, return the extra to the free list.
- Use the start of the block to store its size.
- Return a pointer to the block *after* the size field.
For requests with (B >= 1 page = 4096 bytes):
- Calculate the number of pages needed for this block.
- Allocate that many pages with mmap.
- Fill in the size of the block as (# of pages * 4096).
- Return a pointer to the block *after* the size field.
*/

void *
hmalloc(size_t size)
{
    stats.chunks_allocated += 1;
    size += sizeof(size_t);
    size < sizeof(free_block) ? sizeof(free_block) : size;

    // TODO: Actually allocate memory with mmap and a free list.
    if (size < PAGE_SIZE)
    {
        free_block *block = 0;
        free_block *cur = free_head;
        free_block *prev = 0;
        while (cur)
        {
            if (cur->size >= size)
            {
                block = cur;

                if (prev)
                {
                    prev->next = cur->next;
                }
                else
                {
                    free_head = cur->next;
                }
                break;
            }
            prev = cur;
            cur = cur->next;
        }

        if (!block)
        {
            block = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            assert(block != MAP_FAILED);
            stats.pages_mapped += 1;
            block->size = PAGE_SIZE;
        }

        if (block->size > size && (block->size - size >= sizeof(free_block)))
        {
            void *free_address = (void *)block + size;

            free_block *to_free = (free_block *)free_address;
            to_free->size = block->size - size;
            block->size = size;
            insert_free_node(to_free);
        }

        return (void *)block + sizeof(size_t);
    }
    else
    {
        int n_pages = div_up(size, PAGE_SIZE);
        free_block *block = mmap(0, n_pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        assert(block != MAP_FAILED);
        stats.pages_mapped += n_pages;
        block->size = n_pages * PAGE_SIZE;
        return (void *)block + sizeof(size_t);
    }
}

void hfree(void *item)
{
    stats.chunks_freed += 1;

    free_block *block = (free_block *)(item - sizeof(free_block));

    if (block->size < PAGE_SIZE)
    {
        //TODO put the free block back into the free list.
        insert_free_node(block);
    }
    else
    {
        int n_pages = div_up(block->size, PAGE_SIZE);
        int rv = munmap((void *)block, block->size);
        assert(rv != -1);
        stats.pages_unmapped += n_pages;
    }
}
