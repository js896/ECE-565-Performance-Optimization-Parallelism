#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>


double calc_time(struct timeval start, struct timeval end) {
  double start_sec = (double)start.tv_sec*1000000.0 + (double)start.tv_usec;
  double end_sec = (double)end.tv_sec*1000000.0 + (double)end.tv_usec;

  if (end_sec < start_sec) {
    return 0;
  } else {
    return end_sec - start_sec;
  }
};

/*
//original
void do_loops(int *a, int *b, int *c, int N)
{
  int i;
  for (i=N-1; i>=1; i--) {
    a[i] = a[i] + 1;
  }
  for (i=1; i<N; i++) {
    b[i] = a[i+1] + 3;
  }
  for (i=1; i<N; i++) {
    c[i] = b[i-1] + 2;
  }
}
*/

/*
//loop unrolling
void do_loops(int *a, int *b, int *c, int N)
{
  int i;
  for (i=N-1; i>=1; i-=4) {
    a[i] = a[i] + 1;
    a[i-1] = a[i-1] + 1;
    a[i-2] = a[i-2] + 1;
    a[i-3] = a[i-3] + 1; 
  }
  for (i=1; i<N; i+=4) {
    b[i] = a[i+1] + 3;
    b[i+1] = a[i+2] + 3;
    b[i+2] = a[i+3] + 3;
    b[i+3] = a[i+4] + 3;
  }
  for (i=1; i<N; i+=4) {
    c[i] = b[i-1] + 2;
    c[i+1] = b[i] + 2;
    c[i+2] = b[i+1] + 2;
    c[i+3] = b[i+2] + 2;
  }
}
*/

/*
//loop fusion
void do_loops(int *a, int *b, int *c, int N)
{
  int i;
  for (i=N-1; i>=1; i--) {
    a[i] = a[i] + 1;
  }
  for (i=1; i<N; i++) {
    b[i] = a[i+1] + 3;
    c[i] = b[i-1] + 2;
  }
}
*/

/*
//Loop Reversal
void do_loops(int *a, int *b, int *c, int N)
{
  int i;
  for (i=1; i<=N-1; i++) {
    a[i] = a[i] + 1;
  }
  for (i=N-1; i>=1; i--) {
    b[i] = a[i+1] + 3;
  }
  for (i=N-1; i>=1; i--) {
    c[i] = b[i-1] + 2;
  }
}
*/

/*
//loop peeling
void do_loops(int *a, int *b, int *c, int N)
 {
   int i;
   for (i=N-1; i>=1; i--) {
     a[i] = a[i] + 1;
   }
   if (N > 1) {
     b[1] = a[2] + 3;
   }
   for (i=2; i<N; i++) {
     b[i] = a[i+1] + 3;
   }
   if (N > 1) {
     c[1] = b[0] + 2;
   }
   for (i=2; i<N; i++) {
     c[i] = b[i-1] + 2;
   }
 }
*/

//loop strip mining
void do_loops(int *a, int *b, int *c, int N)
{
  int i;
  int j;
  for (i=N-1; i>=1; i-= 50) {
    for (j = i; j >= (i-49); --j) {
    a[j] = a[j] + 1;
    }
  }
  for (i=1; i<N; i+=50) {
    for (j = i; j < (i+50); ++j) {
      b[j] = a[j+1] + 3;
    }
  }
  for (i=1; i<N; i+=50) {
    for (j = i; j < (i+50); ++j) {
      c[j] = b[j-1] + 2;
    }
  }
}

int main(int argc, char *argv[])
{
  int i;
  unsigned long long sum = 0;
  int N;

  if (argc < 2) {
    printf("Usage: ./loop_performance <array_size>\n");
    return -1;
  }

  N = atoi(argv[1]);

  if ((N != 10000000) && (N != 100000000)) {
    printf("Program argument must be 10,000,000 or 100,000,000.\n");
    return -1;
  }

  int *a = malloc((N+1)*sizeof(int));
  int *b = malloc((N+1)*sizeof(int));
  int *c = malloc((N+1)*sizeof(int));

  for (i=0; i<N+1; i++) {
    a[i] = 1;
    b[i] = 0;
    c[i] = 0;
  }

  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL);
  do_loops(a, b, c, N);
  gettimeofday(&end_time, NULL);

  for (i=0; i<N; i++) {
    sum += c[i];
  }

  double elapsed_us = calc_time(start_time, end_time);
  double elapsed_ms = elapsed_us / 1000.0;
  printf("sum=%llu\n", sum);
  printf("Time=%f milliseconds\n", elapsed_ms);

  return 0;
}