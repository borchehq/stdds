#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "vector.h"

void delete_ds(void *data)
{
  free(*(size_t**)data);
}

int copy_ds(void *ds, void *clone)
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
  dsconf conf = {copy_ds, delete_ds};
  vector v; vector_new(&v, sizeof(size_t*), 1, &conf);
  size_t *elem = malloc(sizeof(size_t));
  *elem = 21;
  vector_push_back(&v, &elem);
  assert(**(size_t**)vector_at(&v, 0) == 21);
  vector_delete(&v);
}

void test_vector_push_back()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push_back(&v, &i);
  }
  assert(num_push == vector_size(&v));
  vector_delete(&v);

  conf = (dsconf){copy_ds, delete_ds};
  vector_new(&v, sizeof(size_t), 1, &conf);
  size_t *x[num_push];
  for(size_t i = 0; i < num_push; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    vector_push_back(&v, &x[i]);
  }
   for(size_t i = 0; i < num_push; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *x[i]);
    assert(*(size_t**)vector_at(&v, i) != x[i]);
    free(x[i]);
    assert(**(size_t**)vector_at(&v, i) == i);
  }
  vector_delete(&v);
}

void test_vector_at()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push_back(&v, &i);
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
    vector_push_back(&v, &i);
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
    vector_push_back(&v, &i);
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

void test_vector_assign()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push_back(&v, &i);
  }
  size_t val = 7819;
  for(size_t i = 0; i < 10000; i++)
  {
    vector_assign(&v, &val, i);
    assert(*(size_t*)vector_at(&v, i) == val);
  }
 vector_delete(&v);

 conf = (dsconf){copy_ds, delete_ds};
 vector_new(&v, sizeof(size_t), 1, &conf);
 size_t *x[num_push];
 size_t *y[num_push];
  for(size_t i = 0; i < num_push; i++)
  {
    x[i] = malloc(sizeof(size_t));
    y[num_push - 1 - i] = malloc(sizeof(size_t));
    *x[i] = i;
    *y[num_push - 1 - i] = i;
    vector_push_back(&v, &x[i]);
    free(x[i]);
  }
  for(size_t i = 0; i < num_push; i++)
  {
    vector_assign(&v, &y[i], i);
  }
  for(size_t i = 0; i < num_push; i++)
  { 
    assert(**(size_t**)vector_at(&v, i) == *y[i]);
    free(y[i]);
    assert(**(size_t**)vector_at(&v, i) == num_push - 1 - i);   
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
    vector_push_back(&v, &i);
  }
  size_t *array = (size_t*)vector_to_array(&v);
  for(size_t i = 0; i < vector_size(&v); i++)
  {
    assert(*(size_t*)vector_at(&v, i) == array[i]);
  }
  free(array);
  vector_delete(&v);

  conf = (dsconf){copy_ds, delete_ds};
  vector_new(&v, sizeof(size_t), 1, &conf);
  size_t *x[num_push];

  for(size_t i = 0; i < num_push; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    vector_push_back(&v, &x[i]);
    free(x[i]);
  }
  size_t **array2 = (size_t**)vector_to_array(&v);
  for(size_t i = 0; i < vector_size(&v); i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *array2[i]);
    free(array2[i]);
  }
  free(array2);
  vector_delete(&v);
}

void test_vector_insert()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    vector_push_back(&v, &i);
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

  conf = (dsconf){copy_ds, delete_ds};
  vector_new(&v, sizeof(size_t), 1, &conf);
  size_t *x[num_push];
  for(size_t i = 0; i < num_push; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    vector_insert(&v, &x[i], 0);
  }
   for(size_t i = 0; i < num_push; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *x[num_push - 1 - i]);
    assert(*(size_t**)vector_at(&v, i) != x[num_push - 1 - i]);
    free(x[num_push - 1 - i]);
    assert(**(size_t**)vector_at(&v, i) == num_push - 1 - i);
  }
  vector_delete(&v);
}

void test_vector_sort()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  vector v2; vector_new(&v2, sizeof(size_t), 1, &conf);
  for(size_t i = 20; i > 0; i--)
  {
    vector_push_back(&v, &i);
    vector_push_back(&v2, &i);
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
  dsconf conf = {copy_ds, delete_ds};
  vector v; vector_new(&v, sizeof(size_t*), 1, &conf);
  vector v2; vector_new(&v2, sizeof(size_t*), 1, &conf);
  for(size_t i = 20; i > 0; i--)
  {
    size_t *elem = malloc(sizeof(size_t));
    size_t *elem2 = malloc(sizeof(size_t));
    *elem = *elem2 = i;
    vector_push_back(&v, &elem);
    vector_push_back(&v2, &elem2);
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
  dsconf conf = {copy_ds, delete_ds};
  vector v; vector_new(&v, sizeof(size_t*), 1, &conf);
  vector v2;
  for(size_t i = 0; i < 20; i++)
  {
    size_t *elem = malloc(sizeof(size_t));
    *elem = i;
    vector_push_back(&v, &elem);
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
  vector_push_back(&v, &val);
  assert(vector_is_empty(&v) == false);
  vector_delete(&v);
}

void test_vector_index_valid()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  size_t val = 1;
  vector_push_back(&v, &val);
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
    vector_push_back(&v, &i);
  }
  vector_clone(&v, &v2);
  assert(vector_size(&v) == vector_size(&v2));
  for(size_t i = 0; i < 20; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == *(size_t*)vector_at(&v2, i));
  }
  vector_delete(&v);
  vector_delete(&v2);

  conf = (dsconf){copy_ds, delete_ds};
  vector_new(&v, sizeof(size_t), 0, &conf);
  for(size_t i = 0; i < 20; i++)
  {
    size_t *elem = malloc(sizeof(size_t));
    *elem = i;
    vector_push_back(&v, &elem);
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

void test_vector_reserve()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  vector_reserve(&v, 35);
  for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  for(double i = 0; i < 35; i++)
  {
    assert(*(double*)vector_at(&v, i) == i);
  }
  vector_reserve(&v, 21);
  assert(vector_size(&v) == 21);
  for(double i = 0; i < 21; i++)
  {
    assert(*(double*)vector_at(&v, i) == i);
  }
  vector_delete(&v);
}

void test_vector_capacity()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
   for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  assert(vector_capacity(&v) == 64);
  vector_delete(&v);
}

void test_vector_front()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  assert(*(double*)vector_front(&v) == 0.0);
  vector_delete(&v);
}

void test_vector_back()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  assert(*(double*)vector_back(&v) == 34.0);
  vector_delete(&v);
}

void test_vector_pop_back()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  assert(v.occupied == 35);
  for(double i = 34; i >= 0; i--)
  {
    double *tmp = vector_pop_back(&v);
    assert(*tmp == i);
    assert(v.occupied == i);
    free(tmp);
  }
  vector_delete(&v);
}

int main(int argc, char const *argv[])
{
  printf("[i] Testing vector_new()...\n");
  test_vector_new();
  printf("[i] Testing vector_push_back()...\n");
  test_vector_push_back();
  printf("[i] Testing vector_at()...\n");
  test_vector_at();
  printf("[i] Testing vector_get()...\n");
  test_vector_get();
  printf("[i] Testing vector_remove()...\n");
  test_vector_remove();
  printf("[i] Testing vector_assign()...\n");
  test_vector_assign();
  printf("[i] Testing vector_to_array()...\n");
  test_vector_to_array();
  printf("[i] Testing vector_insert()...\n");
  test_vector_insert();
  printf("[i] Testing vector_sort()...\n");
  test_vector_sort();
  printf("[i] Testing vector_merge()...\n");
  test_vector_merge();
  printf("[i] Testing vector_split()...\n");
  test_vector_split();
  printf("[i] Testing vector_is_empty()...\n");
  test_vector_is_empty();
  printf("[i] Testing vector_index_valid()...\n");
  test_vector_index_valid();
  printf("[i] Testing vector_clone()...\n");
  test_vector_clone();
  printf("[i] Testing vector_reserve()...\n");
  test_vector_reserve();
  printf("[i] Testing vector_capacity()...\n");
  test_vector_capacity();
  printf("[i] Testing vector_front()...\n");
  test_vector_front();
  printf("[i] Testing vector_back()...\n");
  test_vector_back();
  printf("[i] Testing vector_pop_back()...\n");
  test_vector_pop_back();
  return 0;
}
