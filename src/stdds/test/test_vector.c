#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "vector.h"

typedef struct test_s
{
  double *data;
  size_t size;
}test_s;

void delete_datatype(void *data)
{
  test_s test = *(test_s*)data;
  free(test.data);
}

int compare(const void *a, const void *b)
{
    if(*(size_t*)a > *(size_t*)b){
      return 1;
    }
    if(*(size_t*)a < *(size_t*)b){
      return -1;
    }
    return -1;
}

int main(int argc, char const *argv[])
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 100000000;
  printf("[i] Testing push()...\n");
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  assert(num_push == size(&v));

  printf("[i] Testing vec_at()...\n");
  for(size_t i = 0; i < num_push; i++)
  {
    assert(i == *(size_t*)vec_at(&v, i));
  }

  printf("[i] Testing get()...\n");
  for(size_t i = 0; i < num_push; i++)
  {
    void *res = get(&v, i);
    assert(i == *(size_t*)res);
    free(res);
  }

  printf("[i] Testing rem()...\n");
  size_t index_remove = num_push - 1000;
  size_t successor = *(size_t*)vec_at(&v, index_remove + 1);
  for(size_t i = index_remove; i < size(&v); i++)
  {
    rem(&v, index_remove);
    assert(*(size_t*)vec_at(&v, index_remove) == successor);
    successor = *(size_t*)vec_at(&v, index_remove + 1);
  }

  printf("[i] Testing set()...\n");
  size_t val = 7819;
  for(size_t i = 0; i < 10000; i++)
  {
    set(&v, &val, i);
    assert(*(size_t*)vec_at(&v, i) == val);
  }

  printf("[i] Testing to_array()...\n");
  size_t *array = (size_t*)to_array(&v);
  for(size_t i = 0; i < size(&v); i++)
  {
    assert(*(size_t*)vec_at(&v, i) == array[i]);
  }
  free(array);

  printf("[i] Testing insert()...\n");
  size_t element = 9102901;
  size_t index = size(&v) - 12552;
  size_t *immediate = malloc(sizeof(size_t) * (size(&v) - index));
  for(size_t i = 0; i < size(&v) - index; i++)
  {
    immediate[i] = *(size_t*)vec_at(&v, index + i);
  }
  printf("Size before: %li\n", size(&v));
  insert(&v, &element, index);
  printf("Size afterwards: %li\n", size(&v));
  if(*(size_t*)vec_at(&v, index) != element){
  }
  for(size_t i = 0; i < size(&v) - 1 - index; i++){
    if(immediate[i] != *(size_t*)vec_at(&v, index + i + 1)){
      break;
    }
  }
  free(immediate);
  delete_vector(&v);

  printf("[INFO] testing sortv()\n");
  new_vector(&v, sizeof(size_t), 1, conf);
  vector v2;
  new_vector(&v2, sizeof(size_t), 1, conf);
  for(size_t i = 20; i > 0; i--){
    push(&v, &i);
    push(&v2, &i);
  }
  printf("[ ");
  for(size_t i = 0; i < 20; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  sortv(&v, compare);
  printf("[ ");
  for(size_t i = 0; i < 20; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  //delete_vector(&v);
  //delete_vector(&v2);
  merge(&v, &v2);
  printf("%lu\n", v.occupied);
  printf("[");
  for(size_t i = 0; i < 40; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  split(&v, &v2, 2);
  printf("[");
  for(size_t i = 0; i < v.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  printf("[");
  for(size_t i = 0; i < v2.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v2, i));
  }
  printf("]\n");
  //delete_vector(&v);
  //delete_vector(&v2);
  index_valid(&v, 1) == 0 ? printf("false\n") :  printf("true\n");
  index_valid(&v, 2) == 0 ? printf("false\n") :  printf("true\n");
  index_valid(&v2, 37) == 0 ? printf("false\n") :  printf("true\n");
  index_valid(&v2, 38) == 0 ? printf("false\n") :  printf("true\n");
  bool empty = is_empty(&v);
  empty == 0 ? printf("false\n") :  printf("true\n");
  delete_vector(&v);
  new_vector(&v, sizeof(size_t), 4, conf);
  empty = is_empty(&v);
  empty == 0 ? printf("false\n") :  printf("true\n");
  for(size_t i = 20; i > 0; i--){
    push(&v, &i);
  }
  printf("[");
  for(size_t i = 0; i < v.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  delete_vector(&v2);
  clone(&v, &v2);
  printf("[");
  for(size_t i = 0; i < v2.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v2, i));
  }
  printf("]\n");
  delete_vector(&v);
  delete_vector(&v2);
  return 0;
}
