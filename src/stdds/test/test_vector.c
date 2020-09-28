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

int construct_ds(void *ds)
{
  *(size_t**)ds = malloc(sizeof(size_t));
  size_t x = 27;
  **(size_t**)ds = (size_t)27;
  return 0;
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

void test_vector_empty()
{
  dsconf conf = {NULL, NULL};
  vector v; vector_new(&v, sizeof(size_t), 1, &conf);
  assert(vector_empty(&v) == true);
  size_t val = 1;
  vector_push_back(&v, &val);
  assert(vector_empty(&v) == false);
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
  assert(v.allocated == MIN_CAP);
  vector_delete(&v);
}

void test_vector_resize()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  double a = 9;

  for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  vector_resize(&v, 7);
  assert(v.occupied == 7);
  assert(v.allocated == 8);
  for(double i = 0; i < 7; i++)
  {
    assert(*(double*)vector_at(&v, (size_t)i) == i);
  }
  vector_delete(&v);

  vector_new(&v, sizeof(double), 0, NULL);
  for(double i = 0; i < 35; i++)
  {
    vector_push_back(&v, &i);
  }
  vector_resize(&v, 200);
  assert(v.occupied == 200);
  assert(v.allocated == 200);
  vector_assign(&v, &a, 199);
  assert(*(double*)vector_at(&v, 199) == a);
  vector_delete(&v);

  dsconf conf = {copy_ds, delete_ds, construct_ds};
  vector_new(&v, sizeof(size_t*), 0, &conf);

  for(size_t i = 0; i < 35; i++)
  {
    size_t *x = malloc(sizeof(size_t));
    *x = i;
    vector_push_back(&v, &x);
    free(x);
  }
  vector_resize(&v, 200);
  assert(v.occupied == 200);
  assert(v.allocated == 200);
  for(size_t i = 0; i < 35; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == i);
  }
 
  for(size_t i = 35; i < 200 ; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == 27);
  }
  vector_resize(&v, 5);
  assert(v.occupied == 5);
  assert(v.allocated == 8);
  for(size_t i = 0; i < 5; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == i);
  }
  vector_delete(&v);
}

void test_vector_assign_range()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  double a[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  vector_assign_range(&v, a, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(double*)vector_at(&v, i) == a[i]);
  }
  vector_delete(&v);

  vector_new(&v, sizeof(double), 16, NULL);
  vector_assign_range(&v, a, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(double*)vector_at(&v, i) == a[i]);
  }
  vector_delete(&v);

  vector_new(&v, sizeof(double), 73, NULL);
  vector_assign_range(&v, a, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 18);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(double*)vector_at(&v, i) == a[i]);
  }
  vector_delete(&v);

  size_t *b[16];
  size_t *c[16];
  for(size_t i = 0; i < 16; i++)
  {
    b[i] = malloc(sizeof(size_t));
    c[i] = malloc(sizeof(size_t));
    *b[i] = i;
    *c[i] = 15 - i;
  }

  dsconf conf = {copy_ds, delete_ds};
  vector_new(&v, sizeof(size_t*), 73, &conf);
  vector_assign_range(&v, b, 16);
  vector_assign_range(&v, c, 16);
  vector_assign_range(&v, b, 16);
  vector_assign_range(&v, b, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 18);
  for(size_t i = 0; i < 16; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *b[i]);
    assert(*(size_t**)vector_at(&v, i) != b[i]);
    free(b[i]);
    free(c[i]);
  }
  vector_delete(&v);
}

void test_vector_assign_fill()
{
  vector v; 
  vector_new(&v, sizeof(double), 0, NULL);
  double a[16] = {12,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  vector_assign_fill(&v, a, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(double*)vector_at(&v, i) == a[0]);
  }
  vector_delete(&v);

  vector_new(&v, sizeof(double), 16, NULL);
  vector_assign_fill(&v, a, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(double*)vector_at(&v, i) == a[0]);
  }
  vector_delete(&v);

  vector_new(&v, sizeof(double), 73, NULL);
  vector_assign_fill(&v, a, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 18);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(double*)vector_at(&v, i) == a[0]);
  }
  vector_delete(&v);

  size_t *b[16];
  size_t *c[16];
  for(size_t i = 0; i < 16; i++)
  {
    b[i] = malloc(sizeof(size_t));
    c[i] = malloc(sizeof(size_t));
    *b[i] = i;
    *c[i] = 15 - i;
  }

  dsconf conf = {copy_ds, delete_ds};
  vector_new(&v, sizeof(size_t*), 73, &conf);
  vector_assign_fill(&v, &b[0], 16);
  vector_assign_fill(&v, c, 16);
  vector_assign_fill(&v, b, 16);
  vector_assign_fill(&v, b, 16);
  assert(v.occupied == 16);
  assert(v.allocated == 18);
  for(size_t i = 0; i < 16; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *b[0]);
    assert(*(size_t**)vector_at(&v, i) != b[0]);
  }
  for(size_t i = 0; i < 16; i++)
  {
    free(b[i]);
    free(c[i]);
  }
  vector_delete(&v);
}

void test_vector_insert_range()
{
  vector v; 
  vector_new(&v, sizeof(size_t), 0, NULL);
  size_t b = 12;
  vector_assign_fill(&v, &b, 16);
  size_t a[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  vector_insert_range(&v, a, 0, 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == a[i]);
  }
  for(size_t i = 16; i < 32; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);

  vector_new(&v, sizeof(size_t), 0, NULL);
  vector_assign_fill(&v, &b, 16);
  vector_insert_range(&v, a, 5, 16);
  for(size_t i = 0; i < 5; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  for(size_t i = 5; i < 21; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == a[i - 5]);
  }
  for(size_t i = 21; i < 32; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);

  vector_new(&v, sizeof(size_t), 0, NULL);
  vector_assign_fill(&v, &b, 16);
  vector_insert_range(&v, a, 16, 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  for(size_t i = 16; i < 32; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == a[i - 16]);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);

  dsconf conf = {copy_ds, delete_ds, NULL};
  vector_new(&v, sizeof(size_t), 0, &conf);
  size_t *x[16];
  size_t *y[16];
  for(size_t i = 0; i < 16; i++)
  {
    x[i] = malloc(sizeof(size_t));
    y[i] = malloc(sizeof(size_t));
    *x[i] = i;
    *y[i] = 15 - i;
  }

  vector_assign_fill(&v, &y[15], 16);
  vector_insert_range(&v, x, 5, 16);
  for(size_t i = 0; i < 5; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *y[15]);
    assert(*(size_t**)vector_at(&v, i) != y[15]);
  }
  for(size_t i = 5; i < 21; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *x[i - 5]);
    assert(*(size_t**)vector_at(&v, i) != x[i - 5]);
  }
  for(size_t i = 21; i < 32; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *y[15]);
    assert(*(size_t**)vector_at(&v, i) != y[15]);
  }
  for(size_t i = 0; i < 16; i++)
  {
   free(x[i]);
   free(y[i]);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);
}

void test_vector_insert_fill()
{
  vector v; 
  vector_new(&v, sizeof(size_t), 0, NULL);
  size_t b = 12;
  vector_assign_fill(&v, &b, 16);
  size_t a[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  vector_insert_fill(&v, &a[5], 0, 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == a[5]);
  }
  for(size_t i = 16; i < 32; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);

  vector_new(&v, sizeof(size_t), 0, NULL);
  vector_assign_fill(&v, &b, 16);
  vector_insert_fill(&v, &a[5], 5, 16);
  for(size_t i = 0; i < 5; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  for(size_t i = 5; i < 21; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == a[5]);
  }
  for(size_t i = 21; i < 32; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);

  vector_new(&v, sizeof(size_t), 0, NULL);
  vector_assign_fill(&v, &b, 16);
  vector_insert_fill(&v, &a[5], 16, 16);
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == 12);
  }
  for(size_t i = 16; i < 32; i++)
  {
    assert(*(size_t*)vector_at(&v, i) == a[5]);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
  vector_delete(&v);

  dsconf conf = {copy_ds, delete_ds, NULL};
  vector_new(&v, sizeof(size_t), 0, &conf);
  size_t *x[16];
  size_t *y[16];
  for(size_t i = 0; i < 16; i++)
  {
    x[i] = malloc(sizeof(size_t));
    y[i] = malloc(sizeof(size_t));
    *x[i] = i;
    *y[i] = 15 - i;
  }

  vector_assign_fill(&v, &y[15], 16);
  vector_insert_fill(&v, &x[11], 5, 16);
  for(size_t i = 0; i < 5; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *y[15]);
    assert(*(size_t**)vector_at(&v, i) != y[15]);
  }
  for(size_t i = 5; i < 21; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *x[11]);
    assert(*(size_t**)vector_at(&v, i) != x[11]);
  }
  for(size_t i = 21; i < 32; i++)
  {
    assert(**(size_t**)vector_at(&v, i) == *y[15]);
    assert(*(size_t**)vector_at(&v, i) != y[15]);
  }
  for(size_t i = 0; i < 16; i++)
  {
   free(x[i]);
   free(y[i]);
  }
  assert(v.occupied == 32);
  assert(v.allocated == 32);
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
  printf("[i] Testing vector_empty()...\n");
  test_vector_empty();
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
  printf("[i] Testing vector_resize()...\n");
  test_vector_resize();
  printf("[i] Testing vector_assign_range()...\n");
  test_vector_assign_range();
  printf("[i] Testing vector_assign_fill()...\n");
  test_vector_assign_fill();
  printf("[i] Testing vector_insert_range()...\n");
  test_vector_insert_range();
  printf("[i] Testing vector_insert_fill()...\n");
  test_vector_insert_fill();
  return 0;
}
