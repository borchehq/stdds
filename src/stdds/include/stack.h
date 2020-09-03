#ifndef STACK_FDT_H
#define STACK_FDT_H

#define STACK_INITIAL_CAP 8
#define STACK_MIN_CAP     8

#include <stdlib.h>
#include <stdbool.h>

#include "typedef.h"

typedef struct stack_s stack;

struct stack_s
{
  byte_t *data;
  size_t allocated;
  size_t occupied;
  size_t size_element;
  dsconf *conf;
};


inline int stack_new(stack *stack, size_t size_element, size_t initial_capacity,
              dsconf *conf)
{
  if(initial_capacity == 0)
  {
    initial_capacity = STACK_INITIAL_CAP;
  }
  stack->data = malloc(size_element * initial_capacity);
  if(stack->data == NULL)
  {
    return -1;
  }

  stack->allocated = initial_capacity;
  stack->occupied = 0;
  stack->size_element = size_element;
  stack->conf = conf;
  return 0;

}

inline void stack_delete(stack *stack)
{
  if(stack == NULL)
  {
    return;
  }
  if(stack->conf != NULL && stack->conf->delete_ds != NULL)
  {
    for(size_t i = 0; i < stack->occupied; i++)
    {
      stack->conf->delete_ds(&stack->data[stack->size_element * i]);
    }
  }
  free(stack->data);
}

inline bool stack_empty(stack *stack)
{
  if(stack->occupied == 0)
  {
    return true;
  }
  return false;
}

inline size_t stack_size(stack *stack)
{
  return stack->occupied;
}

inline void *stack_top(stack *stack)
{
  return &stack->data[stack->size_element * (stack->occupied - 1)];
}

inline int stack_push(stack *stack, void *element)
{
  byte_t *tmp = stack->data;
  if(stack->allocated == stack->occupied)
  {
    tmp = realloc(tmp, stack->allocated * stack->size_element * 2);
    if(tmp != NULL)
    {
      stack->data = tmp;
      stack->allocated *= 2;
    }
    else
    {
      return -1;
    }
  }
  
  if(stack->conf != NULL && stack->conf->copy_ds != NULL) 
  {
    stack->conf->copy_ds(element, &stack->data[stack->occupied * stack->size_element]);
  }
  else
  {
    memcpy(&stack->data[stack->occupied * stack->size_element],
           element, stack->size_element);
  }
  stack->occupied++;
  
  return 0;
}

inline void *stack_pop(stack *stack)
{
  if(stack->occupied == 0)
  {
    return NULL;
  }
  void *elem = &stack->data[(stack->occupied - 1) * stack->size_element];
  void *ret = malloc(stack->size_element);
  if(ret == NULL)
  {
    return NULL;
  }
  memcpy(ret, elem, stack->size_element);
  // Delete elem.
  //stacktor_remove(stack, stack->occupied - 1);
  void *tmp = stack->data;
  if(stack->conf != NULL && stack->conf->delete_ds != NULL)
  {
    stack->conf->delete_ds(elem);
  }  
  stack->occupied--;
  if(stack->allocated >= 2 * STACK_MIN_CAP && stack->allocated >= 2 * stack->occupied)
  {
    tmp = realloc(tmp, (stack->allocated / 2) * stack->size_element);
    if(tmp != NULL){
      stack->data = tmp;
      stack->allocated /= 2;
    }
  }
  return ret;
}

#endif