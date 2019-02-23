#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "vector.h"

struct test
{
    size_t size;
};

#define SIZE 100000000
#define SMALL 100000

double time_elapsed(void *(*function)(void))
{
  struct timeval tv_begin, tv_end;
  gettimeofday(&tv_begin, NULL);
  function();
  gettimeofday(&tv_end, NULL);
  return ((tv_end.tv_sec * 1e6 + tv_end.tv_usec) - (tv_begin.tv_sec * 1e6 + tv_begin.tv_usec)) / (double)1e6;
}

void *fill_array()
{
  size_t *vs = malloc(sizeof(size_t) * SIZE);
  for(size_t i = 0; i < SIZE; i++){
    vs[i] = i;
  }
  size_t tmp = vs[SIZE - 1];
  free(vs);
  return (void*)tmp;
}

void *fill_vector()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), SIZE, conf);
  for(size_t i = 0; i < SIZE; i++)
  {
    push(&v, &i);
  }
  delete_vector(&v);
  return NULL;
}

void *benchmark_insert()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), SIZE, conf);
  for(size_t i = 0; i < SMALL; i++)
  {
    insert(&v, &i, SMALL);
  }
  delete_vector(&v);
  return NULL;
}

int main(int argc, char const *argv[])
{
  double time_array = time_elapsed(fill_array);
  double time_vector = time_elapsed(fill_vector);

  printf("Time array: %.16f sec\n", time_array);
  printf("Time vector: %.16f sec\n", time_vector);
  printf("Overhead vector: %f\n", time_vector / time_array);

  printf("Time insert(): %.16f sec\n", time_elapsed(benchmark_insert));
  return 0;
}
