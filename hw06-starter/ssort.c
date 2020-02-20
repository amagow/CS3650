#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#include "float_vec.h"
#include "barrier.h"
#include "utils.h"

int comp(const void * a, const void * b){
    return ( *(long*)a - *(long*)b );
}

    void qsort_floats(floats *xs)
{
    // TODO: call qsort to sort the array
    // see "man 3 qsort" for details
    //FIX : length of the array
    qsort(xs, 0, sizeof(long), comp);
}

floats *
sample(float *data, long size, int P)
{
    // TODO: sample the input data, per the algorithm decription
    return make_floats(10);
}

void sort_worker(int pnum, float *data, long size, int P, floats *samps, long *sizes, barrier *bb)
{
    floats *xs = make_floats(10);
    // TODO: select the floats to be sorted by this worker

    printf("%d: start %.04f, count %ld\n", pnum, samps->data[pnum], xs->size);

    // TODO: some other stuff

    qsort_floats(xs);

    // TODO: probably more stuff

    free_floats(xs);
}

void run_sort_workers(float *data, long size, int P, floats *samps, long *sizes, barrier *bb)
{
    pid_t kids[P];
    (void)kids; // suppress unused warning

    // TODO: spawn P processes, each running sort_worker

    for (int ii = 0; ii < P; ++ii)
    {
        //int rv = waitpid(kids[ii], 0, 0);
        //check_rv(rv);
    }
}

void sample_sort(float *data, long size, int P, long *sizes, barrier *bb)
{
    floats *samps = sample(data, size, P);
    run_sort_workers(data, size, P, samps, sizes, bb);
    free_floats(samps);
}

int main(int argc, char *argv[])
{
    alarm(120);

    //Usage error is args are not equal to three
    if (argc != 3)
    {
        printf("Usage:\n");
        printf("\t%s P data.dat\n", argv[0]);
        return 1;
    }

    //Get inputs
    const int P = atoi(argv[1]);
    const char *fname = argv[2];

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
    int fd = open(fname, O_RDWR);
    check_rv(fd);

    //FIX: unknown size??
    int size = 1024 * sizeof(long);

    float *file = mmap(0, size, PROT_READ,
                      MAP_FILE | MAP_PRIVATE, fd, 0); // TODO: load the file with mmap.

    // TODO: These should probably be from the input file.
    long count = 0;
    while(file[count]){
        count ++;
    }
    float *data = file;

    printf("%ld\n", count);
    printf("%f\n", data[0]);

    long sizes_bytes = P * sizeof(long);
    long *sizes = mmap(0, sizes_bytes, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0); // TODO: This should be shared

    barrier *bb = make_barrier(P);
    (void)sizes;
    // sample_sort(data, count, P, sizes, bb);

    free_barrier(bb);

    // TODO: munmap your mmaps
    munmap(file, size);
    return 0;
}
