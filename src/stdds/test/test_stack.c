#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stack.h"

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

void test_stack_new()
{
  stack stack;
  stack_new(&stack, sizeof(size_t), 0, NULL);
  assert(stack.occupied == 0);
  assert(stack.allocated == STACK_INITIAL_CAP);
  assert(stack.size_element == sizeof(size_t));
  assert(stack.conf == NULL);
  stack_delete(&stack);
}

void test_stack_delete()
{
  stack stack;
  stack_new(&stack, sizeof(size_t), 1, NULL);
  stack_delete(&stack);
}

void test_stack_emtpy()
{
  stack stack;
  stack_new(&stack, sizeof(size_t), 1, NULL);
  assert(stack_empty(&stack) == true);
  stack.occupied = 1;
  assert(stack_empty(&stack) == false);
  stack.occupied = 0;
  stack_delete(&stack);
}

void test_stack_size()
{
  stack stack;
  stack_new(&stack, sizeof(size_t), 1, NULL);
  assert(stack_size(&stack) == 0);
  stack.occupied = 5;
  assert(stack_size(&stack) == 5);
  stack_delete(&stack);
}

void test_stack_push()
{
  dsconf conf = {NULL, NULL};
  stack stack; stack_new(&stack, sizeof(size_t), 1, &conf);
  size_t num_push = 10000;
  for(size_t i = 0; i < num_push; i++)
  {
    stack_push(&stack, &i);
  }
  assert(num_push == stack_size(&stack));
  stack_delete(&stack);

  conf = (dsconf){copy_ds, delete_ds};
  stack_new(&stack, sizeof(size_t), 1, &conf);
  size_t *x[num_push];
  for(size_t i = 0; i < num_push; i++)
  {
    x[i] = malloc(sizeof(size_t));
    *x[i] = i;
    stack_push(&stack, &x[i]);
  }
   for(size_t i = 0; i < num_push; i++)
  {
    assert(**(size_t**)&stack.data[stack.size_element * i] == *x[i]);
    assert(*(size_t**)&stack.data[stack.size_element * i] != x[i]);
    free(x[i]);
    assert(**(size_t**)&stack.data[stack.size_element * i] == i);
  }
  stack_delete(&stack);
}

void test_stack_pop()
{
  stack stack; 
  stack_new(&stack, sizeof(double), 0, NULL);
  for(double i = 0; i < 35; i++)
  {
   stack_push(&stack, &i);
  }
  assert(stack.occupied == 35);
  for(double i = 34; i >= 0; i--)
  {
    double *tmp = stack_pop(&stack);
    assert(*tmp == i);
    assert(stack.occupied == i);
    free(tmp);
  }
  assert(stack.allocated == STACK_MIN_CAP);
  stack_delete(&stack);
}

void test_stack_top()
{
  stack stack; 
  stack_new(&stack, sizeof(double), 0, NULL);
  for(double i = 0; i < 35; i++)
  {
    stack_push(&stack, &i);
  }
  assert(*(double*)stack_top(&stack) == 34.0);
  stack_delete(&stack);
}

int main(int argc, char const *argstack[])
{
  printf("[i] Testing stack_new()...\n");
  test_stack_new();
  printf("[ok] Testing stack_new() successful...\n");
  printf("[i] Testing stack_delete()...\n");
  test_stack_delete();
  printf("[ok] Testing stack_delete() successful...\n");
  printf("[i] Testing stack_empty()...\n");
  test_stack_emtpy();
  printf("[ok] Testing stack_empty() successful...\n");
  printf("[i] Testing stack_size()...\n");
  test_stack_size();
  printf("[ok] Testing stack_size() successful...\n");
  printf("[i] Testing stack_push()...\n");
  test_stack_push();
  printf("[ok] Testing stack_push() successful...\n");
  printf("[i] Testing stack_pop()...\n");
  test_stack_pop();
  printf("[ok] Testing stack_pop() successful...\n");
  printf("[i] Testing stack_top()...\n");
  test_stack_pop();
  printf("[ok] Testing stack_top() successful...\n");
  return 0;
}