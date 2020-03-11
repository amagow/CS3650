// Author: Nat Tuck
// CS3650 starter code

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include "barrier.h"
#include "utils.h"

barrier *
make_barrier(int nn)
{
    barrier *bb = malloc(sizeof(barrier));
    assert(bb != 0);

    check_rv(pthread_mutex_init(&(bb->m), 0));
    check_rv(pthread_cond_init(&(bb->cv), 0));

    bb->count = nn;
    bb->seen = 0;
    return bb;
}

void barrier_wait(barrier *bb)
{
    pthread_mutex_lock(&(bb->m));
    bb->seen += 1;
    int seen = bb->seen;

    if (seen < bb->count)
    {
        while (seen < bb->count)
        {
            pthread_cond_wait(&(bb->cv), &(bb->m));
        }
    }
    else
    {
        printf("barrier done\n");
        pthread_cond_broadcast(&(bb->cv));
        bb->seen = 0;
    }

    pthread_mutex_unlock(&(bb->m));
}

void free_barrier(barrier *bb)
{
    pthread_mutex_destroy(&(bb->m));
    pthread_cond_destroy(&(bb->cv));
    free(bb);
}
