#ifndef HMALLOC_H
#define HMALLOC_H
#include <pthread.h>
// Husky Malloc Interface
// cs3650 Starter Code

typedef struct hm_stats
{
    long pages_mapped;
    long pages_unmapped;
    long chunks_allocated;
    long chunks_freed;
    long free_length;
} hm_stats;

hm_stats *hgetstats();
void hprintstats();

void *hmalloc(size_t size, pthread_mutex_t lock);
void hfree(void *item, pthread_mutex_t lock);
void *hrealloc(void *item, size_t size, pthread_mutex_t lock);

#endif
