#ifndef LIST_FDT_H
#define LIST_FDT_H

#include <stdlib.h>
#include <stdbool.h>

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
  dsconf conf;
};

inline int list_new(list *list, size_t size_element, dsconf *conf)
{
  if(conf != NULL)
  {
    list->conf.construct_ds = conf->construct_ds;
    list->conf.copy_ds = conf->copy_ds;
    list->conf.delete_ds = conf->delete_ds;
  }
  else
  {
    list->conf.construct_ds = NULL;
    list->conf.copy_ds = NULL;
    list->conf.delete_ds = NULL;
  }
  list->head = NULL;
  list->tail = NULL;
  list->size_element = size_element;
  list->size = 0;
}

int list_push_front(list *list, void *element);

inline void *list_pop_front(list *list)
{
  if(list->size == 0)
  {
    return NULL;
  }

  byte_t *data = list->head->data;
  if(list->head->next == NULL)
  {
    free(list->head);
    list->head = list->tail = NULL;
  }
  else
  {
    list->head = list->head->next;
    free(list->head->prev);
    list->head->prev = NULL;
  }
  list->size--;

  return data;
}

int list_push_back(list *list, void *element);

inline void *list_pop_back(list *list)
{
  if(list->size == 0)
  {
    return NULL;
  }

  byte_t *data = list->tail->data;
  if(list->tail->prev == NULL)
  {
    free(list->tail);
    list->head = list->tail = NULL;
  }
  else
  {
    list->tail = list->tail->prev;
    free(list->tail->next);
    list->tail->next = NULL;
  }
  list->size--;

  return data;
}

int list_insert(list *list, size_t position, void *element);

inline int list_erase(list *list, size_t position)
{
  if(position >= list->size)
  {
    return -1;
  }

  struct node_s *tmp = list->head;
  for(size_t i = 0; i < position; i++)
  {
    tmp = tmp->next;
  }
  if(tmp->next == NULL && tmp->prev == NULL)
  {
    list->head = NULL;
    list->tail = NULL;
  }
  else if(tmp->next == NULL)
  { 
    tmp->prev->next = NULL;
    list->tail = tmp->prev;
  }
  else if(tmp->prev == NULL)
  {
    tmp->next->prev = NULL;
    list->head = tmp->next;
  }
  else
  {
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev; 
  }
  if(list->conf.delete_ds != NULL)
  {
    list->conf.delete_ds(tmp->data);
  }
  free(tmp->data);
  free(tmp);
  list->size--;

  return 0;
}

void list_clear(list *list);
void list_delete(list *list);

inline bool list_empty(list *list)
{
  if(list->size == 0)
  {
    return true;
  }
  return false;
}

inline size_t list_size(list *list)
{
  return list->size;
}

inline void *list_front(list *list)
{
  if(list->size == 0)
  {
    return NULL;
  }
  return list->head->data;
}

inline void *list_back(list *list)
{
  if(list->size == 0)
  {
    return NULL;
  }
  return list->tail->data;
}

#endif