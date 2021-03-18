#include <stdio.h>
#include <string.h>
#include <assert.h>


#include "forward_list.h"

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

void test_forward_list_new()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  assert(list.conf.construct_ds == NULL);
  assert(list.conf.copy_ds == NULL);
  assert(list.conf.delete_ds == NULL);
  assert(list.head == NULL);
  assert(list.size == 0);
  assert(list.size_element == sizeof(size_t));
}

void test_forward_list_empty()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  assert(forward_list_empty(&list) == true);
  list.size = 1;
  assert(forward_list_empty(&list) == false);
}

void test_forward_list_size()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  assert(forward_list_size(&list) == 0);
  list.size = 1;
  assert(forward_list_size(&list) == true);
}

void test_forward_list_front()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);
  assert(forward_list_front(&list) == NULL);
}

void test_forward_list_push_front()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  for(size_t i = 0; i < 10; i++)
  {
    forward_list_push_front(&list, &i);
    assert(list.size == i + 1);
  }
  for(size_t i = 0; i < 10; i++)
  {
     size_t *tmp = forward_list_pop_front(&list);
     assert(*tmp == 9 - i);
     free(tmp);
  }
  forward_list_delete(&list);
  dsconf conf = {copy_ds, delete_ds};
  forward_list_new(&list, sizeof(size_t*), &conf);
  size_t *x[10];

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    forward_list_push_front(&list, &x[i]);
  }
   for(size_t i = 0; i < 10; i++)
  {
    size_t **tmp = forward_list_pop_front(&list);
    assert(**tmp == *x[9 - i]);
    assert(*tmp != x[9 - i]);
    free(x[9 - i]);
    free(*tmp);
    free(tmp);
  }
  forward_list_delete(&list);
}

void test_forward_list_pop_front()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  for(size_t i = 0; i < 10; i++)
  {
    forward_list_push_front(&list, &i);
  }
  for(size_t i = 0; i < 10; i++)
  {
    size_t *tmp = forward_list_pop_front(&list);
    assert(*tmp == 9 - i);
    assert(list.size == 9 - i);
    free(tmp);
  }
  forward_list_delete(&list);
}

void test_forward_list_insert_after()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);
  for(size_t i = 0; i < 1; i++)
  {
    forward_list_push_front(&list, &i);
  }
  for(size_t i = 0; i < 10; i++)
  {
    forward_list_insert_after(&list, 0, &i);
  }
  size_t *tmp = forward_list_pop_front(&list);
  assert(*tmp == 0);
  assert(list.size == 10);
  free(tmp);
  for(size_t i = 0; i < 10; i++)
  {
    tmp = forward_list_pop_front(&list);
    assert(*tmp == 9 - i);
    free(tmp);
  }
  forward_list_delete(&list);
  dsconf conf = {copy_ds, delete_ds};
  forward_list_new(&list, sizeof(size_t*), &conf);
  size_t *x[10];

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
  }
  forward_list_push_front(&list, &x[0]);
  for(size_t i = 1; i < 10; i++)
  {
    forward_list_insert_after(&list, 0, &x[i]);
  }
  
  size_t **tmp2 = forward_list_pop_front(&list);
  assert(**tmp2 == 0);
  assert(list.size == 9);
  free(x[0]);
  free(*tmp2);
  free(tmp2);
  for(size_t i = 0; i < 9; i++)
  {
    tmp2 = forward_list_pop_front(&list);
    assert(**tmp2 == *x[9 - i]);
    assert(*tmp2 != x[9 - i]);
    free(x[9 - i]);
    free(*tmp2);
    free(tmp2);
  }
  forward_list_delete(&list);
}

void test_forward_list_erase_after()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  for(size_t i = 0; i < 10; i++)
  {
    forward_list_push_front(&list, &i);
  }

  for(size_t i = 0; i < 9; i++)
  {
    forward_list_erase_after(&list, 0);
    assert(list.size == 9 - i);
    assert(list.head->next == NULL || *(size_t*)list.head->next->data == 9 - i - 2);
  }
  free(forward_list_pop_front(&list));
  forward_list_delete(&list);
  dsconf conf = {copy_ds, delete_ds};
  forward_list_new(&list, sizeof(size_t*), &conf);
  size_t *x[10];

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    forward_list_push_front(&list, &x[i]);
    free(x[i]);
  }

  for(size_t i = 0; i < 9; i++)
  {
    forward_list_erase_after(&list, 0);
    assert(list.size == 9 - i);
  }
  size_t **last = forward_list_pop_front(&list);
  assert(list.size == 0);
  free(*last);
  free(last);
  forward_list_delete(&list);
}

void test_forward_list_clear()
{
  forward_list list;
  forward_list_new(&list, sizeof(size_t), NULL);

  for(size_t i = 0; i < 10; i++)
  {
    forward_list_push_front(&list, &i);
  }
  forward_list_clear(&list);
  assert(list.size == 0);
  assert(list.head == NULL);
  forward_list_delete(&list);
  dsconf conf = {copy_ds, delete_ds};
  forward_list_new(&list, sizeof(size_t*), &conf);
  size_t *x[10];

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    forward_list_push_front(&list, &x[i]);
    free(x[i]);
  }
  forward_list_clear(&list);
  assert(list.size == 0);
  assert(list.head == NULL);
  forward_list_delete(&list);
}

int main(int argc, char const *argv[])
{
  printf("[i] Testing forward_list_new()...\n");
  test_forward_list_new();
  printf("[ok] Testing forward_list_new() successful...\n");
  printf("[i] Testing forward_list_empty()...\n");
  test_forward_list_empty();
  printf("[ok] Testing forward_list_empty() successful...\n");
  printf("[i] Testing forward_list_size()...\n");
  test_forward_list_size();
  printf("[ok] Testing forward_list_size() successful...\n");
  printf("[i] Testing forward_list_front()...\n");
  test_forward_list_front();
  printf("[ok] Testing forward_list_front() successful...\n");
  printf("[i] Testing forward_list_push_front()...\n");
  test_forward_list_push_front();
  printf("[ok] Testing forward_list_push_front() successful...\n");
  printf("[i] Testing forward_list_pop_front()...\n");
  test_forward_list_pop_front();
  printf("[ok] Testing forward_list_pop_front() successful...\n");
  printf("[i] Testing forward_list_insert_after()...\n");
  test_forward_list_insert_after();
  printf("[ok] Testing forward_list_insert_after() successful...\n");
  printf("[i] Testing forward_list_erase_after()...\n");
  test_forward_list_erase_after();
  printf("[ok] Testing forward_list_erase_after() successful...\n");
  printf("[i] Testing forward_list_clear()...\n");
  test_forward_list_erase_after();
  printf("[ok] Testing forward_list_clear() successful...\n");
  return 0;
}