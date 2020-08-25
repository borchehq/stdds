#ifndef LIST_FDT_H
#define LIST_FDT_H

#include <stdlib.h>

#include "typedef.h"

typedef struct list_s list;

struct node_s
{
  byte_t *data;
  struct node_s *prev;
  struct node_s *next;
};

struct list_s
{
  struct node_s *head;
  struct node_s *tail;
  size_t size_element;
  size_t size;
  dsconf *conf;
};

int list_new(list *list, size_t size_element, dsconf *conf);
int list_push_front(list *list, void *element);
void *list_pop_front(list *list);
int list_push_back(list *list, void *element);
void *list_pop_back(list *list);
int list_insert(list *list, size_t position, void *element);
int list_erase(list *list, size_t position);
void list_clear(list *list);
void list_delete(list *list);

#endif