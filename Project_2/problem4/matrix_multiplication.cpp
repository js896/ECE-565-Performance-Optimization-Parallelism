#include <stdlib.h>
#include <time.h>

#define N 1024

double A[N][N];
double B[N][N];
double C[N][N];
double elapsed_ns;

double calc_time(struct timespec start, struct timespec end)
{
    double start_sec = (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
    double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

    if (end_sec < start_sec)
    {
        return 0;
    }
    else
    {
        return end_sec - start_sec;
    }
}

void i_j_k() {}

void j_k_i() {}

void i_k_j() {}

int main(int argc, char **argv)
{
    struct timespec start_time, end_time;
    int ordering = atoi(argv[1]);

    switch (ordering)
    {
    case 1:
        i_j_k();
        break;
    case 2:
        j_k_i();
    default:
        i_k_j();
        break;
    }

    return EXIT_SUCCESS;
}
