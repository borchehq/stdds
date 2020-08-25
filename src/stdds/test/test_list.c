#include <assert.h>
#include <stdio.h>

#include "list.h"

void test_list_new()
{
  list list;
  list_new(&list, sizeof(double), NULL);
  assert(list.head == NULL);
  assert(list.tail == NULL);
  assert(list.size_element == sizeof(double));
  assert(list.size == 0);
  assert(list.conf == NULL);
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