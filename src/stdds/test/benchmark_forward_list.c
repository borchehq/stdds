#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#include "forward_list.h"

double time_elapsed(void *(*function)(forward_list *list), forward_list *list)
{
  struct timeval tv_begin, tv_end;
  gettimeofday(&tv_begin, NULL);
  function(list);
  gettimeofday(&tv_end, NULL);
  return ((tv_end.tv_sec * 1e6 + tv_end.tv_usec) - (tv_begin.tv_sec * 1e6 + tv_begin.tv_usec)) / (double)1e6;
}

void *time_forward_list_new(forward_list *list)
{ 
  forward_list list2;
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    forward_list_new(&list2, sizeof(size_t), NULL);
  }
}

void *time_forward_list_empty(forward_list *list)
{
  forward_list list2;
  forward_list_new(&list2, sizeof(size_t), NULL);
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    forward_list_empty(&list2);
  }
}

void *time_forward_list_size(forward_list *list)
{
  forward_list list2;
  forward_list_new(&list2, sizeof(size_t), NULL);
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    forward_list_size(&list2);
  }
}

void *time_forward_list_front(forward_list *list)
{
  forward_list list2;
  forward_list_new(&list2, sizeof(size_t), NULL);
  size_t a = 0;
  forward_list_push_front(&list2, &a);
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
    forward_list_front(&list2);
  }
  forward_list_delete(&list2);
}

void *time_forward_list_push_front(forward_list *list)
{
  forward_list list2;
  forward_list_new(&list2, sizeof(size_t), NULL);
  volatile size_t iterations = 50000000;

  for(size_t i = 0; i < iterations; i++)
  {
     forward_list_push_front(&list2, &i);
  }
  forward_list_delete(&list2);
}

void *time_forward_list_pop_front(forward_list *list)
{
  volatile size_t iterations = 50000000;
  for(size_t i = 0; i < iterations; i++)
  {
     forward_list_pop_front(list);
  }
}

void *time_forward_list_insert_after(forward_list *list)
{
  forward_list list2;
  forward_list_new(&list2, sizeof(size_t), NULL);
  volatile size_t iterations = 50000000;
  double a = 0;

  forward_list_push_front(&list2, &a);
  for(size_t i = 0; i < iterations; i++)
  {
    forward_list_insert_after(&list2, 0, &i);
  }
  forward_list_delete(&list2);
}

void *time_forward_list_erase_after(forward_list *list)
{
  volatile size_t iterations = 50000000;

  for(size_t i = 0; i < iterations; i++)
  { 
    forward_list_erase_after(list, 0);
  }
}

void *time_forward_list_clear(forward_list *list)
{
  volatile size_t iterations = 50000000;

  for(size_t i = 0; i < iterations; i++)
  { 
    forward_list_clear(list);
  }
}

int main(int argc, char const *argv[])
{
  double time = 0;
  forward_list list;
  size_t iterations = 50000000;

  time = time_elapsed(time_forward_list_new, NULL);
  printf("Time time_forward_list_new: %.16f sec\n", time);
  time = time_elapsed(time_forward_list_empty, NULL);
  printf("Time time_forward_list_empty: %.16f sec\n", time);
  time = time_elapsed(time_forward_list_size, NULL);
  printf("Time time_forward_list_size: %.16f sec\n", time);
  time = time_elapsed(time_forward_list_front, NULL);
  printf("Time time_forward_list_front: %.16f sec\n", time);
  time = time_elapsed(time_forward_list_push_front, NULL);
  printf("Time time_forward_list_push_front: %.16f sec\n", time);
  forward_list_new(&list, sizeof(size_t), NULL);
  for(size_t i = 0; i < iterations; i++)
  {
     forward_list_push_front(&list, &i);
  }
  time = time_elapsed(time_forward_list_pop_front, &list);
  forward_list_delete(&list);
  printf("Time time_forward_list_pop_front: %.16f sec\n", time);
  time = time_elapsed(time_forward_list_insert_after, NULL);
  printf("Time time_forward_list_insert_after: %.16f sec\n", time);
  forward_list_new(&list, sizeof(size_t), NULL);
  for(size_t i = 0; i < iterations; i++)
  {
    forward_list_push_front(&list, &i);
  }
  time = time_elapsed(time_forward_list_erase_after, &list);
  forward_list_delete(&list);
  printf("Time time_forward_list_erase_after: %.16f sec\n", time);
  forward_list_new(&list, sizeof(size_t), NULL);
  for(size_t i = 0; i < iterations; i++)
  {
     forward_list_push_front(&list, &i);
  }
  time = time_elapsed(time_forward_list_clear, &list);
  forward_list_delete(&list);
  printf("Time time_forward_list_clear: %.16f sec\n", time);
  return 0;
}