#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

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

void test_list_new()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  assert(list.head == NULL);
  assert(list.tail == NULL);
  assert(list.size_element == sizeof(double));
  assert(list.size == 0);
  assert(list.conf == NULL);
  list_delete(&list);
}

void test_list_push_front()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;
  list_push_front(&list, &a);
  assert(list.size == 1);
  assert(*(double*)(list.head->data) == 6.0 && 6.0 == *(double*)(list.tail->data));
  list_push_front(&list, &b);
  list_push_front(&list, &c);
  assert(list.size == 3);
  assert(*(double*)(list.head->data) == 7.3);
  assert(*(double*)(list.head->next->data) == 5.0);
  assert(*(double*)(list.head->next->next->data) == 6.0);
  assert(*(double*)(list.tail->data) == 6.0);
  assert(*(double*)(list.tail->prev->data) == 5.0);
  assert(*(double*)(list.tail->prev->prev->data) == 7.3);
  list_delete(&list);

  dsconf conf = {copy_ds, delete_ds};
  size_t *x[10];
  list_new(&list, sizeof(size_t*), &conf);

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    list_push_front(&list, &x[i]);
  }
  for(size_t i = 0; i < 10; i++)
  {
    size_t **tmp = (size_t**)list_pop_back(&list);
    assert(**tmp == *x[i]);
    assert(*tmp != x[i]);
    free(x[i]);
    assert(**tmp == i);
    delete_ds(tmp);
    free(tmp);
  }
  list_delete(&list);
}

void test_list_pop_front()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;
  double *a_ret = NULL;
  double *b_ret = NULL;
  double *c_ret = NULL;

  list_push_front(&list, &a);
  list_push_front(&list, &b);
  list_push_front(&list, &c);
  c_ret = list_pop_front(&list);
  b_ret = list_pop_front(&list);
  a_ret = list_pop_front(&list);
  assert(*c_ret == c);
  assert(*b_ret == b);
  assert(*a_ret == a);
  assert(list.head == list.tail);
  assert(list.head == NULL);
  assert(list.size == 0);
  free(c_ret);
  free(b_ret);
  free(a_ret);
}

void test_list_push_back()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;
  list_push_back(&list, &a);
  assert(list.size == 1);
  assert(*(double*)(list.head->data) == 6.0 && 6.0 == *(double*)(list.tail->data));
  list_push_back(&list, &b);
  list_push_back(&list, &c);
  assert(list.size == 3);
  assert(*(double*)(list.tail->data) == 7.3);
  assert(*(double*)(list.tail->prev->data) == 5.0);
  assert(*(double*)(list.tail->prev->prev->data) == 6.0);
  assert(*(double*)(list.head->data) == 6.0);
  assert(*(double*)(list.head->next->data) == 5.0);
  assert(*(double*)(list.head->next->next->data) == 7.3);
  list_delete(&list);

  dsconf conf = {copy_ds, delete_ds};
  size_t *x[10];
  list_new(&list, sizeof(size_t*), &conf);

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    list_push_back(&list, &x[i]);
  }
  for(size_t i = 0; i < 10; i++)
  {
    size_t **tmp = (size_t**)list_pop_front(&list);
    assert(**tmp == *x[i]);
    assert(*tmp != x[i]);
    free(x[i]);
    assert(**tmp == i);
    delete_ds(tmp);
    free(tmp);
  }
  list_delete(&list);
}

void test_list_pop_back()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;
  double *a_ret = NULL;
  double *b_ret = NULL;
  double *c_ret = NULL;

  list_push_back(&list, &a);
  list_push_back(&list, &b);
  list_push_back(&list, &c);
  c_ret = list_pop_back(&list);
  b_ret = list_pop_back(&list);
  a_ret = list_pop_back(&list);
  assert(*c_ret == c);
  assert(*b_ret == b);
  assert(*a_ret == a);
  assert(list.head == list.tail);
  assert(list.head == NULL);
  assert(list.size == 0);
  free(c_ret);
  free(b_ret);
  free(a_ret);
}

void test_list_insert()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;

  list_insert(&list, 7, &a);
  assert(list.head == list.tail);
  assert(*(double*)list.head->data == a);
  assert(list.size == 1);
  list_insert(&list, 0, &b);
  assert(*(double*)list.head->data == b);
  assert(list.size == 2);
  list_insert(&list, 2, &c);
  assert(*(double*)list.tail->data == c);
  assert(list.size == 3);
  list_insert(&list, 2, &c);
  assert(*(double*)list.tail->prev->data == c);
  assert(*(double*)list.tail->prev->prev->data == a);
  assert(list.size == 4);
  list_delete(&list);

  dsconf conf = {copy_ds, delete_ds};
  size_t *x[10];
  list_new(&list, sizeof(size_t*), &conf);

  for(size_t i = 0; i < 10; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    list_insert(&list, 1, &x[i]);
  }

  size_t **tmp = (size_t**)list_pop_back(&list);
  assert(**tmp == 1);
  delete_ds(tmp);
  free(tmp);
  free(x[1]);
  tmp = (size_t**)list_pop_front(&list);
  assert(**tmp == 0);
  delete_ds(tmp);
  free(tmp);
  free(x[0]);
  for(size_t i = 2; i < 10; i++)
  {
    tmp = (size_t**)list_pop_back(&list);
    assert(**tmp == *x[i]);
    assert(*tmp != x[i]);
    free(x[i]);
    assert(**tmp == i);
    delete_ds(tmp);
    free(tmp);
  }
  list_delete(&list);
}

void test_list_erase()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;

  list_push_back(&list, &a);
  list_push_back(&list, &b);
  list_push_back(&list, &c);
  list_erase(&list, 0);
  assert(list.size == 2);
  assert(*(double*)list.head->data == b);
  list_erase(&list, 1);
  assert(*(double*)list.tail->data == b);
  list_erase(&list, 0);
  assert(list.head == list.tail);
  assert(list.head == NULL);
  assert(list.size == 0);
  assert(list_erase(&list, 0) == -1);
}

void test_list_clear()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  double a = 6.0;
  double b = 5.0;
  double c = 7.3;

  list_push_back(&list, &a);
  list_push_back(&list, &b);
  list_push_back(&list, &c);
  list_clear(&list);

  assert(list.size == 0);
  assert(list.head == list.tail);
  assert(list.head == NULL);
}

int main(int argc, char const *argv[])
{
  printf("[i] Testing list_new()...\n");
  test_list_new();
  printf("[ok] Testing list_new() successful...\n");
  printf("[i] Testing list_push_front()...\n");
  test_list_push_front();
  printf("[ok] Testing list_push_front() successful...\n");
  printf("[i] Testing list_pop_front()...\n");
  test_list_pop_front();
  printf("[ok] Testing list_pop_front() successful...\n");
  printf("[i] Testing list_push_back()...\n");
  test_list_push_back();
  printf("[ok] Testing list_push_back() successful...\n");
  printf("[i] Testing list_pop_back()...\n");
  test_list_pop_back();
  printf("[ok] Testing list_pop_back() successful...\n");
  printf("[i] Testing list_insert()...\n");
  test_list_insert();
  printf("[ok] Testing list_insert() successful...\n");
  printf("[i] Testing list_erase()...\n");
  test_list_erase();
  printf("[ok] Testing list_erase() successful...\n");
  printf("[i] Testing list_clear()...\n");
  test_list_clear();
  printf("[ok] Testing list_clear() successful...\n");
}