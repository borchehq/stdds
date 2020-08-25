#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "vector.h"

void delete_ds(void *data)
{
  free(*(size_t**)data);
}

int clone_ds(void *ds, void *clone)
{
  *(size_t**)clone = malloc(sizeof(size_t));
  memcpy(*(size_t**)clone, *(size_t**)ds, sizeof(size_t));
  return 0;
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

void test_vector_new()
{
  dsconf conf = {clone_ds, delete_ds};
  vector v; vector_new(&v, sizeof(size_t*), 1, &conf);
  size_t *elem = malloc(sizeof(size_t));
  *elem = 21;
  vector_push(&v, &elem);
  assert(**(size_t**)vector_at(&v, 0) == 21);
  vector_delete(&v);
}

void test_vector_push()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  assert(num_push == vector_size(&v));
  vector_delete(&v);
}

void test_vector_at()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  for(size_t i = 0; i < num_push; i++)
  {
    assert(i == *(size_t*)vector_at(&v, i));
  }
  vector_delete(&v);
}


void test_vector_get()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  for(size_t i = 0; i < num_push; i++)
  {
    void *res = vector_get(&v, i);
    assert(i == *(size_t*)res);
    free(res);
  }
  vector_delete(&v);
}

void test_vector_remove()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  size_t index_remove = num_push - 1000;
  size_t successor = *(size_t*)vector_at(&v, index_remove + 1);
  for(size_t i = index_remove; i < vector_size(&v); i++)
  {
    int status = vector_remove(&v, index_remove);
    assert(*(size_t*)vector_at(&v, index_remove) == successor);
    successor = *(size_t*)vector_at(&v, index_remove + 1);
  }
  vector_delete(&v);
}

void test_vector_set()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  size_t val = 7819;
  for(size_t i = 0; i < 10000; i++)
  {
    vector_set(&v, &val, i);
    assert(*(size_t*)vector_at(&v, i) == val);
  }
 vector_delete(&v);
}

void test_vector_to_array()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  size_t *array = (size_t*)vector_to_array(&v);
  for(size_t i = 0; i < vector_size(&v); i++)
  {
    assert(*(size_t*)vector_at(&v, i) == array[i]);
  }
  free(array);
  vector_delete(&v);
}

void test_vector_insert()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push(&v, &i);
  }
  size_t element = 91909;
  size_t index = vector_size(&v) - 102;
  size_t *immediate = malloc(sizeof(size_t) * (vector_size(&v) - index));
  for(size_t i = 0; i < vector_size(&v) - index; i++)
  {
    immediate[i] = *(size_t*)vector_at(&v, index + i);
  }
  vector_insert(&v, &element, index);
  assert(vector_size(&v) == 10001);
  assert(*(size_t*)vector_at(&v, index) == element);
  for(size_t i = 0; i < vector_size(&v) - 1 - index; i++){
    assert(immediate[i] == *(size_t*)vector_at(&v, index + i + 1));
  }
  free(immediate);
  vector_delete(&v);
}

void test_vector_sort()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  vector v2; vector_new(&v2, sizeof(size_t), 1, &conf);
  for(size_t i = 20; i > 0; i--)
  {
    vector_push(&v, &i);
    vector_push(&v2, &i);
  }
  vector_sort(&v, compare);
  for(size_t i = 0; i < 20 ; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == *(size_t*)vector_at(&v2, 19 - i));
  }
  vector_delete(&v);
  vector_delete(&v2);
}

void test_vector_merge()
{
  dsconf conf = {clone_ds, delete_ds};
  vector v; vector_new(&v, sizeof(size_t*), 1, &conf);
  vector v2; vector_new(&v2, sizeof(size_t*), 1, &conf);
  for(size_t i = 20; i > 0; i--)
  {
    size_t *elem = malloc(sizeof(size_t));
    size_t *elem2 = malloc(sizeof(size_t));
    *elem = *elem2 = i;
    vector_push(&v, &elem);
    vector_push(&v2, &elem2);
  }
    vector_merge(&v, &v2);
    assert(vector_size(&v) == 40);
    for(size_t i = 0; i < 20; i++)
    {
      assert(**(size_t**)vector_at(&v, i) == **(size_t**)vector_at(&v, 20 + i));
    }
    vector_delete(&v);
}

void test_vector_split()
{
  dsconf conf = {clone_ds, delete_ds};
  vector v; vector_new(&v, sizeof(size_t*), 1, &conf);
  vector v2;
  for(size_t i = 0; i < 20; i++)
  {
    size_t *elem = malloc(sizeof(size_t));
    *elem = i;
    vector_push(&v, &elem);
  }
  vector_split(&v, &v2, 2);
  assert(vector_size(&v) == 2);
  assert(vector_size(&v2) == 18);
  for(size_t i = 0; i < 2; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == i);
  }
  for(size_t i = 2; i < 20; i++)
  {
    assert(**(size_t**)vector_at(&v2, i - 2) == i);
  }
  vector_delete(&v);
  vector_delete(&v2);
}

void test_vector_is_empty()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  assert(vector_is_empty(&v) == true);
  size_t val = 1;
  vector_push(&v, &val);
  assert(vector_is_empty(&v) == false);
  vector_delete(&v);
}

void test_vector_index_valid()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t val = 1;
  vector_push(&v, &val);
  assert(vector_index_valid(&v, 0) == true);
  assert(vector_index_valid(&v, 1) == false);
  vector_delete(&v);
}

void test_vector_clone()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 0, &conf);
  vector v2;
  for(size_t i = 0; i < 20; i++)
  {
    vector_push(&v, &i);
  }
  vector_clone(&v, &v2);
  assert(vector_size(&v) == vector_size(&v2));
  for(size_t i = 0; i < 20; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == *(size_t*)vector_at(&v2, i));
  }
  vector_delete(&v);
  vector_delete(&v2);

  conf = (dsconf){clone_ds, delete_ds};
  vector_new(&v, sizeof(size_t), 0, &conf);
  for(size_t i = 0; i < 20; i++)
  {
    size_t *elem = malloc(sizeof(size_t));
    *elem = i;
    vector_push(&v, &elem);
  }
  vector_clone(&v, &v2);
  assert(vector_size(&v) == vector_size(&v2));
  for(size_t i = 0; i < 20; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == **(size_t**)vector_at(&v2, i));
  }
  vector_delete(&v);
  vector_delete(&v2);
}

int main(int argc, char const *argv[])
{
  printf("[i] Testing vector_new()...\n");
  test_vector_new();
  printf("[i] Testing vector_push()...\n");
  test_vector_push();
  printf("[i] Testing vector_at()...\n");
  test_vector_at();
  printf("[i] Testing vector_get()...\n");
  test_vector_get();
  printf("[i] Testing vector_remove()...\n");
  test_vector_remove();
  printf("[i] Testing vector_set()...\n");
  test_vector_set();
  printf("[i] Testing vector_to_array()...\n");
  test_vector_to_array();
  printf("[i] Testing vector_insert()...\n");
  test_vector_insert();
  printf("[i] Testing vector_sort()...\n");
  test_vector_sort();
  printf("[i] Testing merge()...\n");
  test_vector_merge();
  printf("[i] Testing split()...\n");
  test_vector_split();
  printf("[i] Testing is_empty()...\n");
  test_vector_is_empty();
  printf("[i] Testing vector_index_valid()...\n");
  test_vector_index_valid();
  printf("[i] Testing clone()...\n");
  test_vector_clone();
  return 0;
}
