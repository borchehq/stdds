#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"


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

static int deque_reserve_front(deque *deque)
{
  ptrdiff_t offset = deque->front.block - deque->block_map_first;
  ptrdiff_t range_map = 1 + deque->block_map_last - deque->block_map_first;
  ptrdiff_t offset_back = deque->back.block - deque->front.block;
  byte_t **tmp = realloc(deque->block_map_first,
                         range_map * 2 * sizeof(byte_t*));
  if(tmp != NULL)
  {
    memmove(tmp + offset + range_map,
            tmp + offset, sizeof(byte_t*) * (offset_back + 1));
    deque->block_map_first = tmp;
    deque->block_map_last = tmp + 2 * range_map - 1;
    deque->front.block = tmp + offset + range_map;
    deque->back.block = deque->front.block + offset_back;
    return 0;
  }

  return -1;
}


static int deque_reserve_back(deque *deque)
{
  ptrdiff_t offset = deque->front.block - deque->block_map_first;
  ptrdiff_t range_map = 1 + deque->block_map_last - deque->block_map_first;
  ptrdiff_t offset_back = deque->back.block - deque->front.block;
  byte_t **tmp = realloc(deque->block_map_first,
                         range_map * 2 * sizeof(byte_t*));

  if(tmp != NULL)
  {
    deque->block_map_first = tmp;
    deque->block_map_last = tmp + 2 * range_map - 1;
    deque->front.block = tmp + offset;
    deque->back.block = deque->front.block + offset_back;
    return 0;
  }
  return -1;
}


void test_deque_push_back()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);

  for(size_t i = 0; i < 190; i++)
  {
    size_t k = i * i;
    assert(deque.size == i);
    deque_push_back(&deque, &k);
  }
  assert(deque.size == 190);
  for(size_t i = 0; i < 190; i++)
  {
    size_t val = *(size_t*)&deque.block_map_first
                 [i / 16][(i % 16) * (deque.size_type)];
    assert(val == i * i);
  }
  assert(*(size_t*)(deque.back.current) == 189 * 189);
  assert(*(size_t*)(deque.front.current) == 0);
  deque_delete(&deque);

  dsconf conf = (dsconf){copy_ds, delete_ds, construct_ds};
  deque_new(&deque, sizeof(size_t*), &conf);
  size_t *x[190];

  for(size_t i = 0; i < 190; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i * i;
  }

  for(size_t i = 0; i < 190; i++)
  {
    deque_push_back(&deque, &x[i]);
  }

  for(size_t i = 0; i < 190; i++)
  {
    size_t val = **(size_t**)&deque.block_map_first
                 [i / 16][(i % 16) * (deque.size_type)];
    //printf("%lu\n", val)                 ;
    assert(val == i * i);
    assert(val == *x[i]);
    free(x[i]);
  }
  deque_delete(&deque);
}

void test_deque_push_front()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;

  for(size_t i = 0; i < size; i++)
  {
    size_t k = i * i;
    assert(deque.size == i);
    deque_push_front(&deque, &k);
  }
  assert(deque.size == size);
  for(size_t i = 0; i < size; i++)
  {
    ptrdiff_t offset = (size_t*)deque.front.current - (size_t*)deque.front.first;
    size_t val = *(size_t*)&(deque.front.block
                 [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]);
    assert(val == (size - 1 - i) * (size - 1 - i));
    //printf("%lu:%lu\n", *(size_t*)&(deque.front.block
    //             [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]), (size - 1 - i) * (size - 1 - i));
  }
  assert(*(size_t*)(deque.back.current) == 0);
  assert(*(size_t*)(deque.front.current) == (size - 1) * (size - 1));
  deque_delete(&deque);

  dsconf conf = (dsconf){copy_ds, delete_ds, construct_ds};
  deque_new(&deque, sizeof(size_t*), &conf);
  size_t *x[size];

  for(size_t i = 0; i < size; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i * i;
  }

  for(size_t i = 0; i < size; i++)
  {
    deque_push_front(&deque, &x[i]);
    assert(**(size_t**)deque.front.current == *x[i]);
  }

  for(size_t i = 0; i < size; i++)
  {
    ptrdiff_t offset = (size_t*)deque.front.current - (size_t*)deque.front.first;
    size_t val = **(size_t**)&(deque.front.block
                 [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]);
    //printf("%lu\n", *(size_t*)x[i])                 ;
    assert(val == (size - 1 - i) * (size - 1 - i));
    assert(val == *x[size - 1 - i]);
    free(x[size - 1 - i]);
  }
  deque_delete(&deque);
}

void test_deque_push_interplay()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;

  for(size_t i = 0; i < size; i++)
  {
    size_t k = i * i;
    assert(deque.size == 2 * i);
    deque_push_front(&deque, &k);
    deque_push_back(&deque, &k);
  }
  assert(deque.size == 2 * size);

  for(size_t i = 0; i < size; i++)
  {
    ptrdiff_t offset = (size_t*)deque.front.current - (size_t*)deque.front.first;
    size_t val = *(size_t*)&(deque.front.block
                 [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]);
    assert(val == (size - 1 - i) * (size - 1 - i));
    //printf("%lu:%lu\n", *(size_t*)&(deque.front.block
    //             [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]), (size - 1 - i) * (size - 1 - i));
  }
  for(size_t i = size; i < 2 * size; i++)
  {
    ptrdiff_t offset = (size_t*)deque.front.current - (size_t*)deque.front.first;
    size_t val = *(size_t*)&(deque.front.block
                 [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]);
    assert(val == (i - size) * (i - size));
    //printf("%lu:%lu\n", *(size_t*)&(deque.front.block
    //             [(i + offset) / 16][((i + offset) % 16) * (deque.size_type)]), (size - 1 - i) * (size - 1 - i));
  }
  deque_delete(&deque);
}

void test_deque_pop_back()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;

  for(size_t i = 0; i < size; i++)
  {
    size_t k = i * i;
    deque_push_back(&deque, &k);
  }

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = deque_pop_back(&deque);
    assert(*k == (size - 1 - i) * (size - 1 - i));
    free(k);
  }
  deque_delete(&deque);

  dsconf conf = (dsconf){copy_ds, delete_ds, construct_ds};
  deque_new(&deque, sizeof(size_t*), &conf);

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = malloc(sizeof(size_t));
    *k = i * i;
    deque_push_back(&deque, &k);
    free(k);
  }

  for(size_t i = 0; i < size; i++)
  {
    size_t **k = deque_pop_back(&deque);
    assert(**k == (size - 1 - i) * (size - 1 - i));
    free(*k);
    free(k);
  }
  deque_delete(&deque);
}

void test_deque_pop_front()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;

  for(size_t i = 0; i < size; i++)
  {
    size_t k = i * i;
    deque_push_back(&deque, &k);
  }

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = deque_pop_front(&deque);
    assert(*k == i * i);
    free(k);
  }
  deque_delete(&deque);

  dsconf conf = (dsconf){copy_ds, delete_ds, construct_ds};
  deque_new(&deque, sizeof(size_t*), &conf);

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = malloc(sizeof(size_t));
    *k = i * i;
    deque_push_front(&deque, &k);
    free(k);
  }

  for(size_t i = 0; i < size; i++)
  {
    size_t **k = deque_pop_front(&deque);
    assert(**k == (size - 1 - i) * (size - 1 - i));
    free(*k);
    free(k);
  }
  deque_delete(&deque);
}

void test_deque_at()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;

  for(size_t i = 0; i < size; i++)
  {
    size_t k = i * i;
    deque_push_back(&deque, &k);
  }

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = deque_at(&deque, i);
    assert(*k == i * i);
  }
  assert(deque_at(&deque, size) == NULL);
  deque_delete(&deque);

  dsconf conf = (dsconf){copy_ds, delete_ds, construct_ds};
  deque_new(&deque, sizeof(size_t*), &conf);

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = malloc(sizeof(size_t));
    *k = i * i;
    deque_push_back(&deque, &k);
    free(k);
  }

  for(size_t i = 0; i < size; i++)
  {
    size_t **k = deque_at(&deque, i);
    assert(**k == i * i);
  }
  deque_delete(&deque);
}

void print_deque(deque *deque)
{
  // Print the first block.
  printf("[ ");
  for(byte_t *i = deque->front.current; i <= deque->front.last;
      i += sizeof(size_t))
  {
    printf("%lu ", *(size_t*)i);
  }
  printf(" ]\n");

  // Print the blocks in between.
  for(byte_t **j = deque->front.block + 1; j < deque->back.block; j++)
  {
    printf("[ ");
    for(size_t i = 0; i < SIZE_BLOCK; i++)
    {
       printf("%lu ", ((size_t*)j[0])[i]);
    }
    printf(" ]\n");
  }

  // Print the last block.
  printf("[ ");
  for(byte_t *i = deque->back.first; i <= deque->back.current;
      i += sizeof(size_t))
  {
    printf("%lu ", *(size_t*)i);
  }
  printf(" ]\n");
}

void test_deque_insert()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;
  size_t magic_number_1 = 0;
  size_t magic_number_2 = 1;
  size_t magic_number_3 = 2;

  for(size_t i = 0; i < size; i++)
  {
    size_t k = 10;
    deque_push_back(&deque, &k);
  }

  deque_insert(&deque, &magic_number_1, 0);
  deque_insert(&deque, &magic_number_1, 191);
  assert(*(size_t*)deque_at(&deque, 0) == magic_number_1);
  assert(*(size_t*)deque_at(&deque, 191) == magic_number_1);
  deque_insert(&deque, &magic_number_1, 170);
  assert(*(size_t*)deque_at(&deque, 170) == magic_number_1);
  
  deque_delete(&deque);
  deque_new(&deque, sizeof(size_t), NULL);
  for(size_t i = 0; i < 16; i++)
  {
    deque_push_back(&deque, &magic_number_1);
  }
  for(size_t i = 0; i < 16; i++)
  {
    deque_push_back(&deque, &magic_number_2);
  }
  for(size_t i = 0; i < 16; i++)
  {
    deque_push_back(&deque, &magic_number_3);
  }
  
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_1);
  }
  for(size_t i = 16; i < 32; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_2);
  }
  for(size_t i = 32; i < 48; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_3);
  }
  //print_deque(&deque);
  //printf("--------------------------------\n");
  deque_insert(&deque, &magic_number_1, 0);
  assert(*(size_t*)deque_at(&deque, 0) == magic_number_1);
  deque_insert(&deque, &magic_number_1, 49);
  assert(*(size_t*)deque_at(&deque, 49) == magic_number_1);
  //print_deque(&deque);
  //printf("--------------------------------\n");
  deque_insert(&deque, &magic_number_1, 32);
  assert(*(size_t*)deque_at(&deque, 32) == magic_number_1);
  //print_deque(&deque);
  //printf("--------------------------------\n");

  deque_delete(&deque);
  deque_new(&deque, sizeof(size_t), NULL);
  for(size_t i = 0; i < 16; i++)
  {
    deque_push_back(&deque, &magic_number_1);
  }
  for(size_t i = 0; i < 500; i++)
  {
    deque_insert(&deque, &magic_number_2, 0);
  }
  for(size_t i = 0; i < 500; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_2);
  }
   for(size_t i = 500; i < 516; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_1);
  }
  deque_delete(&deque);
  deque_new(&deque, sizeof(size_t), NULL);
  for(size_t i = 0; i < 16; i++)
  {
    deque_push_back(&deque, &magic_number_1);
  }
  for(size_t i = 0; i < 500; i++)
  {
    deque_insert(&deque, &magic_number_2, deque_size(&deque));
  }
  for(size_t i = 0; i < 16; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_1);
  }
   for(size_t i = 16; i < 516; i++)
  {
    assert(*(size_t*)deque_at(&deque, i) == magic_number_2);
  }
  assert(deque_size(&deque) == 516);
 // print_deque(&deque);
 // printf("--------------------------------\n");
  //printf("%lu\n", *(size_t*)deque_at(&deque, 48));
  deque_delete(&deque);
}

void test_deque_erase()
{
  deque deque;
  deque_new(&deque, sizeof(size_t), NULL);
  size_t size = 190;

  for(size_t i = 0; i < size; i++)
  {
    deque_push_back(&deque, &i);
  }

  for(size_t i = 0; i < size; i++)
  {
    //printf("val: %lu, i: %lu\n", *(size_t*)deque_at(&deque, 0), i);
    assert(*(size_t*)deque_at(&deque, 0) == i);
    deque_erase(&deque, 0);
  }

  deque_delete(&deque);

  deque_new(&deque, sizeof(size_t), NULL);

  for(size_t i = 0; i < size; i++)
  {
    deque_push_front(&deque, &i);
  }

  for(size_t i = 0; i < size; i++)
  {
    //printf("val: %lu, i: %lu\n", *(size_t*)deque_at(&deque, 0), i);
    assert(*(size_t*)deque_at(&deque, 0) == size - 1 - i);
    deque_erase(&deque, 0);
  }

  deque_delete(&deque);

  deque_new(&deque, sizeof(size_t), NULL);

  for(size_t i = 0; i < size; i++)
  {
    deque_push_back(&deque, &i);
  }

  for(size_t i = 0; i < 20; i++)
  {
    deque_erase(&deque, 3);
    assert(*(size_t*)deque_at(&deque, 3) == 3 + 1 + i);
  }

  deque_delete(&deque);

  deque_new(&deque, sizeof(size_t), NULL);

  for(size_t i = 0; i < size; i++)
  {
    deque_push_back(&deque, &i);
  }

  for(size_t i = 0; i < 20; i++)
  {
    deque_erase(&deque, 167);
    //printf("val: %lu\n", *(size_t*)deque_at(&deque, 167));
    assert(*(size_t*)deque_at(&deque, 167) == 167 + 1 + i);
  }

  deque_delete(&deque);

  dsconf conf = {.construct_ds = construct_ds, 
                 .copy_ds = NULL, 
                 .delete_ds = delete_ds};
  
  deque_new(&deque, sizeof(size_t*), &conf);

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = malloc(sizeof(size_t));
    *k = i;
    deque_push_back(&deque, &k);
  }

  for(size_t i = 0; i < 20; i++)
  {
    deque_erase(&deque, 3);
    assert(**(size_t**)deque_at(&deque, 3) == 3 + 1 + i);
  }
  deque_delete(&deque);

  deque_new(&deque, sizeof(size_t*), &conf);

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = malloc(sizeof(size_t));
    *k = i;
    deque_push_back(&deque, &k);
  }

  for(size_t i = 0; i < 20; i++)
  {
    deque_erase(&deque, 167);
    assert(**(size_t**)deque_at(&deque, 167) == 167 + 1 + i);
  }
  deque_delete(&deque);

  deque_new(&deque, sizeof(size_t*), &conf);

  for(size_t i = 0; i < size; i++)
  {
    size_t *k = malloc(sizeof(size_t));
    *k = i;
    deque_push_back(&deque, &k);
  }
   for(size_t i = 0; i < size; i++)
  {
    deque_erase(&deque, 0);
    assert(deque.size == size - 1 - i);
  }
  //printf("%lu\n", deque.block_map_last - deque.block_map_first);
  //printf("%lu\n", deque.back.block - deque.front.block);
  deque_delete(&deque);
}

int main(int argc, char const *argv[])
{
  /*deque deque;
  dsconf conf = {NULL, NULL, NULL};
  deque_new(&deque, sizeof(double), &conf);
  deque.front.current = deque.back.current = deque.back.first;
  for(size_t i = 0; i < SIZE_BLOCK; i++)
  {
    *(double*)deque.front.current = 34.56;
    deque.front.current += sizeof(double);
  }
  for(size_t i = 0; i < SIZE_BLOCK; i++)
  {
    printf("%lu: %f\n", i, *(double*)(deque.front.first + sizeof(double) * i));
  }
  deque_reserve_back(&deque);
   deque_reserve_back(&deque);
    deque_reserve_back(&deque);
    deque_reserve_front(&deque);
     deque_reserve_back(&deque);
    deque_reserve_front(&deque);
    deque_reserve_front(&deque);
  for(size_t i = 0; i < SIZE_BLOCK; i++)
  {
    printf("%lu: %f\n", i, *(double*)(deque.front.first + sizeof(double) * i));
  }
  deque_delete(&deque);*/
  /*deque deq;
  deque_new(&deq, sizeof(int), NULL);
  printf("%lu\n", deq.front.first);
  printf("%lu\n", deq.front.block[0]);
  byte_t **test = (byte_t**)256;
  printf("%lu,%lu\n", test, test - 1);*/
  /*int t[5] = {1,2,3,4,5};
  for(int i = 0; i < 5; i++) {
    printf("t:%i\n", t[i]);
  }
  memmove(t + 1, t, 4 * sizeof(int));
  for(int i = 0; i < 5; i++) {
    printf("t:%i\n", t[i]);
  }*/
  test_deque_push_back();
  test_deque_push_front();
  test_deque_push_interplay();

  test_deque_pop_back();
  test_deque_pop_front();
  test_deque_at();
  test_deque_insert();
  printf("[i] Testing deque_erase()...\n");
  test_deque_erase();
  return 0;
}