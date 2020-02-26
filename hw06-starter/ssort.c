// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/mman.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <math.h>
// #include <float.h>
// #include <assert.h>
// #include "float_vec.h"
// #include "barrier.h"
// #include "utils.h"

// int comp(const void *a, const void *b)
// {
//     float fa = *(const float *)a;
//     float fb = *(const float *)b;
//     if(fa == 99.999001)
//     return 1;
//     if(fb == 99.999001)
//     return -1;
//     return (fa > fb) - (fa < fb);
// }

// void qsort_floats(floats *xs)
// {
//     qsort(xs->data, xs->size, sizeof(float), comp);
// }

// floats *
// sample(float *data, long size, int P)
// {
//     floats *xs = make_floats(0);

//     floats_push(xs, FLT_MIN);

//     int range = 3 * (P - 1);
//     float tmp[range];
//     for (int ii = 0; ii < range; ++ii)
//     {
//         tmp[ii] = data[rand() % size];
//     }
//     qsort(tmp, range, sizeof(float), comp);
//     for (int ii = 1; ii < range; ii += 3)
//     {
//         floats_push(xs, tmp[ii]);
//     }

//     floats_push(xs, FLT_MAX);

//     return xs;
// }

// void sort_worker(int pnum, float *data, long size, int P, floats *samps, long *sizes, barrier *bb)
// {
//     floats *xs = make_floats(0);
//     for (int ii = 0; ii < size; ii++)
//     {
//         if (data[ii] > samps->data[pnum] && data[ii] <= samps->data[pnum + 1])
//         {
//             floats_push(xs, data[ii]);
//         }
//     }

//     for (int ii = 0; ii < xs->size; ii++)
//     {
//         assert(xs->data[ii] > samps->data[pnum] && xs->data[ii] <= samps->data[pnum + 1]);
//     }

//     printf("%d: start %.04f, count %ld\n", pnum, samps->data[pnum], xs->size);
//     sizes[pnum] = xs->size;

//     barrier_wait(bb);

//     long start = 0, end = 0;
//     for (int ii = 0; ii < pnum; ii++)
//     {
//         start += sizes[ii];
//     }
//     for (int ii = 0; ii <= pnum; ii++)
//     {
//         end += sizes[ii];
//     }
//     // printf("process %d\n", pnum);
//     qsort_floats(xs);
//     // floats_print(xs);
//     barrier_wait(bb);
//     for (int ii = start; ii < end; ii++)
//     {
//         // printf("process %d, stores float %F at %d\n", pnum, xs->data[ii], start + ii);
//         assert(xs->data[ii - start] > samps->data[pnum] && xs->data[ii - start] <= samps->data[pnum + 1]);
//         data[ii] = xs->data[ii - start];
//     }
//     free_floats(xs);
// }

// void run_sort_workers(float *data, long size, int P, floats *samps, long *sizes, barrier *bb)
// {
//     pid_t kids[P];

//     for (int pp = 0; pp < P; ++pp)
//     {
//         if ((kids[pp] = fork()))
//         {

//         }
//         else
//         {
//             sort_worker(pp, data, size, P, samps, sizes, bb);
//             exit(0);
//         }
//     }

//     for (int ii = 0; ii < P; ++ii)
//     {
//         int rv = waitpid(kids[ii], 0, 0);
//         check_rv(rv);
//     }
// }

// void sample_sort(float *data, long size, int P, long *sizes, barrier *bb)
// {
//     floats *samps = sample(data, size, P);
//     run_sort_workers(data, size, P, samps, sizes, bb);
//     free_floats(samps);
// }

// int main(int argc, char *argv[])
// {
//     alarm(120);

//     //Usage error is args are not equal to three
//     if (argc != 3)
//     {
//         printf("Usage:\n");
//         printf("\t%s P data.dat\n", argv[0]);
//         return 1;
//     }

//     //Get inputs
//     const int P = atoi(argv[1]);
//     const char *fname = argv[2];

//     //Set seed for the next few random calls
//     seed_rng();

//     int rv;
//     struct stat st;
//     rv = stat(fname, &st);
//     check_rv(rv);

//     const int fsize = st.st_size;
//     if (fsize < 8)
//     {
//         printf("File too small.\n");
//         return 1;
//     }

//     //Open file
//     int fd = open(fname, O_RDWR);
//     check_rv(fd);

//     //Open mmaps
//     long *fileCount = mmap(0, sizeof(long), PROT_READ,
//                            MAP_FILE | MAP_SHARED, fd, 0);
//     //First 8 bits of the file is a long type
//     long count = fileCount[0];

//     float *fileArray = mmap(0, count * sizeof(float), PROT_READ | PROT_WRITE,
//                             MAP_FILE | MAP_SHARED, fd, 0);
//     //Next few bits are for the array data
//     float *data = &fileArray[2];

//     long sizes_bytes = P * sizeof(long);
//     long *sizes = mmap(0, sizes_bytes, PROT_READ | PROT_WRITE,
//                        MAP_SHARED | MAP_ANONYMOUS, -1, 0); // TODO: This should be shared

//     // Debugger output
//     // for (size_t i = 0; i < count; i++)
//     // {
//     //     printf("%f\n", data[i]);
//     // }

//     barrier *bb = make_barrier(P);
//     sample_sort(data, count, P, sizes, bb);

//     free_barrier(bb);

//     for (size_t i = 0; i < count; i++)
//     {
//         printf("%f\n", data[i]);
//     }

//     munmap(sizes, sizes_bytes);
//     munmap(fileCount, sizeof(long));
//     munmap(fileArray, count * sizeof(float));
//     return 0;
// }

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

#include <float.h>

int
comparitive(const void*a, const void*b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}

void
qsort_floats(floats* xs)
{
    // TODO: call qsort to sort the array
    // see "man 3 qsort" for details
    qsort(xs->data, xs->size, sizeof(float), comparitive);
}

floats*
sample(float* data, long size, int P)
{
    // TODO: sample the input data, per the algorithm decription
    floats* make = make_floats(0);
    floats_push(make, FLT_MIN);
    long sample_size = 3*(P-1);
    floats* temp = make_floats(0);
    // float prearray[sample_size];
    for (int i=0;i<sample_size;i++){
        // make->data[i] = data[rand()%sample_size];
        floats_push(temp, data[rand()%sample_size]);
    }
    qsort_floats(make);
    for (int j=1;j<sample_size;j+=3){
        floats_push(make, temp->data[j]);
    }
    floats_push(make, FLT_MAX);
    // floats_print(make);
    free_floats(temp);
    return make;
    
}

void
sort_worker(int pnum, float* data, long size, int P, floats* samps, long* sizes, barrier* bb)
{
    floats* xs = make_floats(0);
    for (int i=0;i<size;i++){
        if (samps->data[pnum] <= data[i] && data[i] < samps->data[pnum+1]){
            floats_push(xs, data[i]);
        }
    }

    printf("%d: start %.04f, count %ld\n", pnum, samps->data[pnum], xs->size);

    sizes[pnum] = xs->size;
    barrier_wait(bb);
    long start = 0;
    for (int i=0; i<=pnum-1; i++){
        start = start + sizes[i];
    }
    long end = 0;
    for (int j=0; j<=pnum; j++){
        end = end + sizes[j];
    }
    qsort_floats(xs);
    barrier_wait(bb);

    for (int ii=start; ii<end; ii++){
        data[ii] = xs->data[ii - start];
    }

    free_floats(xs);
}

void
run_sort_workers(float* data, long size, int P, floats* samps, long* sizes, barrier* bb)
{
    pid_t kids[P];

    for (int pp = 0; pp < P; ++pp) {
        if ((kids[pp] = fork())) {
        }
        else{
            sort_worker(pp ,data, size, P, samps, sizes, bb);
            exit(0);
        }
    }

    // (void) kids; suppress unused warning
    // TODO: spawn P processes, each running sort_worker

    for (int ii = 0; ii < P; ++ii) {
        int rv = waitpid(kids[ii], 0, 0);
        check_rv(rv);
    }
}

void
sample_sort(float* data, long size, int P, long* sizes, barrier* bb)
{
    floats* samps = sample(data, size, P);
    run_sort_workers(data, size, P, samps, sizes, bb);
    free_floats(samps);
}

int
main(int argc, char* argv[])
{
    alarm(120);

    if (argc != 3) {
        printf("Usage:\n");
        printf("\t%s P data.dat\n", argv[0]);
        return 1;
    }

    const int P = atoi(argv[1]);
    const char* fname = argv[2];

    seed_rng();

    int rv;
    struct stat st;
    rv = stat(fname, &st);
    check_rv(rv);

    const int fsize = st.st_size;
    if (fsize < 8) {
        printf("File too small.\n");
        return 1;
    }

    int fd = open(fname, O_RDWR);
    check_rv(fd);

    long* xl = mmap(0, sizeof(long), PROT_READ, MAP_PRIVATE|MAP_FILE, fd, 0);
    long size = xl[0];
    float* xf = mmap(0, size*sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FILE, fd, 0);
    xf = &xf[2];


    // TODO: These should probably be from the input file.
    // long count = 100;
    // float* data = malloc(1024);

    long sizes_bytes = P * sizeof(long);
    long* sizes = mmap(0, sizes_bytes, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);

    barrier* bb = make_barrier(P);

    sample_sort(xf, size, P, sizes, bb);

    free_barrier(bb);

    // TODO: munmap your mmaps
    munmap(sizes,sizes_bytes);
    munmap(xl,sizeof(long));
    munmap(xf,size*sizeof(float));
    return 0;
}




