#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pair.h"

void delete_ds(void *data)
{
  free(((pair*)data)->first);
  free(((pair*)data)->second);
}

int main(int argc, char const *argv[])
{
  pair a;
  int x = 0;
  int y = 12;
  pair_new(&a, &x, &y);
  assert(*(int*)(a.first) == x);
  assert(*(int*)(a.second) == y);
  pair_delete(&a, NULL);

  int *first = malloc(sizeof(int));
  int *second = malloc(sizeof(int));
  *first = 8;
  *second = 7;
  pair_new(&a, first, second);
  assert(*(int*)pair_first(&a) == *first);
  assert(*(int*)pair_second(&a) == *second);
  dsconf conf = {NULL, delete_ds, NULL};
  pair_delete(&a, &conf);
  return 0;
}