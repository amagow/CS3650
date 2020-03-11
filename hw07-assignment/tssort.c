#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "float_vec.h"
#include "barrier.h"
#include "utils.h"

typedef struct sort_job
{
    int pnum;
    float *readData;
    float *writeData;
    long size;
    int P;
    floats *samps;
    long *sizes;
    barrier *bb;
} sort_job;

int comp(const void *a, const void *b)
{
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb);
}

void qsort_floats(floats *xs)
{
    qsort(xs->data, xs->size, sizeof(float), comp);
}

floats *
sample(float *data, long size, int P)
{
    floats *xs = make_floats(0);

    floats_push(xs, FLT_MIN);

    int range = 3 * (P - 1);
    float tmp[range];
    for (int ii = 0; ii < range; ++ii)
    {
        tmp[ii] = data[rand() % size];
    }
    qsort(tmp, range, sizeof(float), comp);
    for (int ii = 1; ii < range; ii += 3)
    {
        floats_push(xs, tmp[ii]);
    }

    floats_push(xs, FLT_MAX);

    return xs;
}

void *sort_worker(void *arg)
{
    sort_job *t_arg = ((sort_job *)arg);

    floats *xs = make_floats(0);
    for (int ii = 0; ii < t_arg->size; ii++)
    {
        if (t_arg->readData[ii] > t_arg->samps->data[t_arg->pnum] && t_arg->readData[ii] <= t_arg->samps->data[t_arg->pnum + 1])
        {
            floats_push(xs, t_arg->readData[ii]);
        }
    }

    for (int ii = 0; ii < xs->size; ii++)
    {
        assert(xs->data[ii] > t_arg->samps->data[t_arg->pnum] && xs->data[ii] <= t_arg->samps->data[t_arg->pnum + 1]);
    }

    printf("%d: start %.04f, count %ld\n", t_arg->pnum, t_arg->samps->data[t_arg->pnum], xs->size);
    t_arg->sizes[t_arg->pnum] = xs->size;

    printf("reached 0 %d\n", t_arg->pnum);
    barrier_wait(t_arg->bb);

    printf("reached 1 %d\n", t_arg->pnum);
    long start = 0, end = 0;
    for (int ii = 0; ii < t_arg->pnum; ii++)
    {
        start += t_arg->sizes[ii];
    }
    for (int ii = 0; ii <= t_arg->pnum; ii++)
    {
        end += t_arg->sizes[ii];
    }
    // printf("process %d\n", pnum);
    qsort_floats(xs);
    // floats_print(xs);
    barrier_wait(t_arg->bb);
    printf("reached 2 %d \nstart %ld and end %ld\n", t_arg->pnum, start, end);
    floats_print(xs);
    for (int ii = start; ii < end; ii++)
    {
        // printf("process %d, stores float %F at %d\n", pnum, xs->data[ii], start + ii);
        assert(xs->data[ii - start] > t_arg->samps->data[t_arg->pnum] && xs->data[ii - start] <= t_arg->samps->data[t_arg->pnum + 1]);
        t_arg->writeData[ii] = xs->data[ii - start];
    }
    free_floats(xs);
    free(t_arg);
    return 0;
}

void run_sort_workers(float *readData, float *writeData, long size, int P, floats *samps, long *sizes, barrier *bb)
{
    pthread_t threads[P];

    for (int pp = 0; pp < P; ++pp)
    {
        sort_job *arg = malloc(sizeof(sort_job));
        arg->pnum = pp;
        arg->readData = readData;
        arg->writeData = writeData;
        arg->size = size;
        arg->P = P;
        arg->samps = samps;
        arg->sizes = sizes;
        arg->bb = bb;

        int rv = pthread_create(&(threads[pp]), 0, sort_worker, (void *)arg);
        check_rv(rv);
    }

    for (int pp = 0; pp < P; ++pp)
    {
        printf("loop");
        int rv = pthread_join(threads[pp], 0);
        check_rv(rv);
    }
}

void sample_sort(float *readData, float *writeData, long size, int P, long *sizes, barrier *bb)
{
    floats *samps = sample(readData, size, P);
    run_sort_workers(readData, writeData, size, P, samps, sizes, bb);
    free_floats(samps);
}

int main(int argc, char *argv[])
{
    alarm(120);

    //Usage error is args are not equal to three
    if (argc != 4)
    {
        printf("Usage:\n");
        printf("\t%s P input.dat output.dat\n", argv[0]);
        return 1;
    }

    //Get inputs
    const int P = atoi(argv[1]);
    const char *fname = argv[2];
    const char *fname2 = argv[3];

    //Set seed for the next few random calls
    seed_rng();

    int rv;
    struct stat st;
    rv = stat(fname, &st);
    check_rv(rv);

    const int fsize = st.st_size;
    if (fsize < 8)
    {
        printf("File too small.\n");
        return 1;
    }

    //Open file
    int fd = open(fname, O_RDONLY);
    int fd2 = open(fname2, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    check_rv(fd);
    check_rv(fd2);

    //Open mmaps
    long count;
    read(fd, &count, 8);
    float *fileArray = mmap(0, (count + 2) * sizeof(float), PROT_READ,
                            MAP_FILE | MAP_SHARED, fd, 0);

    write(fd2, &count, 8);
    float *writeArray = mmap(0, (count + 2) * sizeof(float), PROT_WRITE,
                             MAP_FILE | MAP_SHARED, fd2, 0);
    //Next few bits are for the array data
    float *readData = &fileArray[2];
    float *writeData = &writeArray[2];

    long sizes_bytes = P * sizeof(long);
    long *sizes = mmap(0, sizes_bytes, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0); // TODO: This should be shared

    // Debugger output
    // for (size_t i = 0; i < count; i++)
    // {
    //     printf("%f\n", data[i]);
    // }

    barrier *bb = make_barrier(P);
    sample_sort(readData, writeData, count, P, sizes, bb);

    free_barrier(bb);

    for (size_t i = 0; i < count; i++)
    {
        writeData[i] = readData[i];
        printf("%f\n", writeData[i]);
    }

    munmap(sizes, sizes_bytes);
    munmap(fileArray, (count + 2) * sizeof(float));
    munmap(writeArray, (count + 2) * sizeof(float));

    //Close File descriptor
    close(fd2);
    close(fd);
    return 0;
}
