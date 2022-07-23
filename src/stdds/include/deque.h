#ifndef DEQUE_FDT_H
#define DEQUE_FDT_H

#define SIZE_BLOCK 16
#define SIZE_MAP   1

#include <stddef.h>
#include <stdbool.h>

#include "typedef.h"

typedef struct deque_s deque;
typedef struct deque_iterator_s deque_iterator;

struct deque_iterator_s
{
  byte_t *current;
  byte_t *first, *last;
  byte_t **block;
};

struct deque_s
{
  byte_t **block_map_first, **block_map_last;
  deque_iterator front, back;
  size_t size_type;
  dsconf conf;
  size_t size;
};

int deque_new(deque *deque, size_t size_type, dsconf *conf);
void deque_delete(deque *deque);
int deque_push_back(deque *deque, void *element);
int deque_push_front(deque *deque, void *element);
void *deque_pop_back(deque *deque);
void *deque_pop_front(deque *deque);
void *deque_at(deque *deque, size_t index);
void *deque_back(deque *deque);
void *deque_front(deque *deque);
size_t deque_size(deque *deque);
bool deque_empty(deque *deque);
int deque_assign(deque *deque, void *element, size_t index);
int deque_insert(deque *deque, void *element, size_t index);

#endif