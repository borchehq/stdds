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

void test_new_vector()
{
  dsconf conf = {clone_ds, delete_ds};
  vector v; new_vector(&v, sizeof(size_t*), 1, conf);
  size_t *elem = malloc(sizeof(size_t));
  *elem = 21;
  push(&v, &elem);
  assert(**(size_t**)vec_at(&v, 0) == 21);
  delete_vector(&v);
}

void test_push()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  assert(num_push == size(&v));
  delete_vector(&v);
}

void test_vec_at()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  for(size_t i = 0; i < num_push; i++)
  {
    assert(i == *(size_t*)vec_at(&v, i));
  }
  delete_vector(&v);
}


void test_get()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  for(size_t i = 0; i < num_push; i++)
  {
    void *res = get(&v, i);
    assert(i == *(size_t*)res);
    free(res);
  }
  delete_vector(&v);
}

void test_rem()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  size_t index_remove = num_push - 1000;
  size_t successor = *(size_t*)vec_at(&v, index_remove + 1);
  for(size_t i = index_remove; i < size(&v); i++)
  {
    int status = rem(&v, index_remove);
    assert(*(size_t*)vec_at(&v, index_remove) == successor);
    successor = *(size_t*)vec_at(&v, index_remove + 1);
  }
  delete_vector(&v);
}

void test_set()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  size_t val = 7819;
  for(size_t i = 0; i < 10000; i++)
  {
    set(&v, &val, i);
    assert(*(size_t*)vec_at(&v, i) == val);
  }
  delete_vector(&v);
}

void test_to_array()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  size_t *array = (size_t*)to_array(&v);
  for(size_t i = 0; i < size(&v); i++)
  {
    assert(*(size_t*)vec_at(&v, i) == array[i]);
  }
  free(array);
  delete_vector(&v);
}

void test_insert()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    push(&v, &i);
  }
  size_t element = 91909;
  size_t index = size(&v) - 102;
  size_t *immediate = malloc(sizeof(size_t) * (size(&v) - index));
  for(size_t i = 0; i < size(&v) - index; i++)
  {
    immediate[i] = *(size_t*)vec_at(&v, index + i);
  }
  insert(&v, &element, index);
  assert(size(&v) == 10001);
  assert(*(size_t*)vec_at(&v, index) == element);
  for(size_t i = 0; i < size(&v) - 1 - index; i++){
    assert(immediate[i] == *(size_t*)vec_at(&v, index + i + 1));
  }
  free(immediate);
  delete_vector(&v);
}

void test_sortv()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  vector v2; new_vector(&v2, sizeof(size_t), 1, conf);
  for(size_t i = 20; i > 0; i--)
  {
    push(&v, &i);
    push(&v2, &i);
  }
  sortv(&v, compare);
  for(size_t i = 0; i < 20 ; i++)
  {
    assert(*(size_t*)vec_at(&v, i) == *(size_t*)vec_at(&v2, 19 - i));
  }
  delete_vector(&v);
  delete_vector(&v2);
}

void test_merge()
{
  dsconf conf = {clone_ds, delete_ds};
  vector v; new_vector(&v, sizeof(size_t*), 1, conf);
  vector v2; new_vector(&v2, sizeof(size_t*), 1, conf);
  for(size_t i = 20; i > 0; i--)
  {
    size_t *elem = malloc(sizeof(size_t));
    size_t *elem2 = malloc(sizeof(size_t));
    *elem = *elem2 = i;
    push(&v, &elem);
    push(&v2, &elem2);
  }
    merge(&v, &v2);
    assert(size(&v) == 40);
    for(size_t i = 0; i < 20; i++)
    {
      assert(**(size_t**)vec_at(&v, i) == **(size_t**)vec_at(&v, 20 + i));
    }
    delete_vector(&v);
}

void test_split()
{
  dsconf conf = {clone_ds, delete_ds};
  vector v; new_vector(&v, sizeof(size_t*), 1, conf);
  vector v2;
  for(size_t i = 0; i < 20; i++)
  {
    size_t *elem = malloc(sizeof(size_t));
    *elem = i;
    push(&v, &elem);
  }
  split(&v, &v2, 2);
  assert(size(&v) == 2);
  assert(size(&v2) == 18);
  for(size_t i = 0; i < 2; i++)
  {
    assert(**(size_t**)vec_at(&v, i) == i);
  }
  for(size_t i = 2; i < 20; i++)
  {
    assert(**(size_t**)vec_at(&v2, i - 2) == i);
  }
  delete_vector(&v);
  delete_vector(&v2);
}

void test_is_empty()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  assert(is_empty(&v) == true);
  size_t val = 1;
  push(&v, &val);
  assert(is_empty(&v) == false);
  delete_vector(&v);
}

void test_index_valid()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 1, conf);
  size_t val = 1;
  push(&v, &val);
  assert(index_valid(&v, 0) == true);
  assert(index_valid(&v, 1) == false);
  delete_vector(&v);
}

void test_clone()
{
  dsconf conf = {NULL, NULL};
  vector v; new_vector(&v, sizeof(size_t), 0, conf);
  vector v2;
  for(size_t i = 0; i < 20; i++)
  {
    push(&v, &i);
  }
  clone(&v, &v2);
  assert(size(&v) == size(&v2));
  for(size_t i = 0; i < 20; i++)
  {
    assert(*(size_t*)vec_at(&v, i) == *(size_t*)vec_at(&v2, i));
  }
  delete_vector(&v);
  delete_vector(&v2);

  conf = (dsconf){clone_ds, delete_ds};
  new_vector(&v, sizeof(size_t), 0, conf);
  for(size_t i = 0; i < 20; i++)
  {
    size_t *elem = malloc(sizeof(size_t));
    *elem = i;
    push(&v, &elem);
  }
  clone(&v, &v2);
  assert(size(&v) == size(&v2));
  for(size_t i = 0; i < 20; i++)
  {
    assert(**(size_t**)vec_at(&v, i) == **(size_t**)vec_at(&v2, i));
  }
  delete_vector(&v);
  delete_vector(&v2);
}

int main(int argc, char const *argv[])
{
  printf("[i] Testing new_vector()...\n");
  test_new_vector();
  printf("[i] Testing push()...\n");
  test_push();
  printf("[i] Testing vec_at()...\n");
  test_vec_at();
  printf("[i] Testing get()...\n");
  test_get();
  printf("[i] Testing rem()...\n");
  test_rem();
  printf("[i] Testing set()...\n");
  test_set();
  printf("[i] Testing to_array()...\n");
  test_to_array();
  printf("[i] Testing insert()...\n");
  test_insert();
  printf("[i] Testing sortv()...\n");
  test_sortv();
  printf("[i] Testing merge()...\n");
  test_merge();
  printf("[i] Testing split()...\n");
  test_split();
  printf("[i] Testing is_empty()...\n");
  test_is_empty();
  printf("[i] Testing index_valid()...\n");
  test_index_valid();
  printf("[i] Testing clone()...\n");
  test_clone();
  return 0;
}
