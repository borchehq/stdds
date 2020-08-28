#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "list.h"

double time_elapsed(void *(*function)(list *list), list *list)
{
  struct timeval tv_begin, tv_end;
  gettimeofday(&tv_begin, NULL);
  function(list);
  gettimeofday(&tv_end, NULL);
  return ((tv_end.tv_sec * 1e6 + tv_end.tv_usec) - (tv_begin.tv_sec * 1e6 + tv_begin.tv_usec)) / (double)1e6;
}


void *time_list_new(list *list)
{
  volatile size_t iterations = 1000000000;
  for(size_t i = 0; i < iterations; i++)
  {
    list_new(list, sizeof(size_t), NULL);
  }
}

void *time_list_push_front(list *list)
{
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    list_push_front(list, &i);
  }
}

void *time_list_pop_front(list *list)
{
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    list_pop_front(list);
  }
}

void *time_list_insert(list *list)
{
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    list_insert(list, 1, &i);
  }
}

void *time_list_erase(list *list)
{
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    list_erase(list, 0);
  }
}

void *time_list_clear(list *list)
{
  list_clear(list);
}

int main(int argc, char const *argv[])
{
  list list;
  double time = time_elapsed(time_list_new, &list);
  printf("Time time_list_new: %.16f sec\n", time);
  list_new(&list, sizeof(size_t), NULL);
  time = time_elapsed(time_list_push_front, &list);
  list_delete(&list);
  printf("Time time_list_push_front: %.16f sec\n", time);

  list_new(&list, sizeof(char), NULL);
  for(size_t i = 0; i < 50000000; i++)
  {
    list_push_front(&list, &i);
  }
  time = time_elapsed(time_list_pop_front, &list);
  list_delete(&list);
  printf("Time time_list_pop_front: %.16f sec\n", time);

  list_new(&list, sizeof(size_t), NULL);
  time = time_elapsed(time_list_insert, &list);
  list_delete(&list);
  printf("Time time_list_insert: %.16f sec\n", time);

  list_new(&list, sizeof(char), NULL);
  for(size_t i = 0; i < 50000000; i++)
  {
    list_push_front(&list, &i);
  }
  time = time_elapsed(time_list_erase, &list);
  list_delete(&list);
  printf("Time time_list_erase: %.16f sec\n", time);

  list_new(&list, sizeof(char), NULL);
  for(size_t i = 0; i < 50000000; i++)
  {
    list_push_front(&list, &i);
  }
  time = time_elapsed(time_list_clear, &list);
  list_delete(&list);
  printf("Time time_list_clear: %.16f sec\n", time);
  return 0;
}

