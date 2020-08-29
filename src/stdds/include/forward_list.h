#ifndef FORWARD_LIST_FDT_H
#define FORWARD_LIST_FDT_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "typedef.h"

typedef struct forward_list_s forward_list;

struct forward_node_s
{
  byte_t *data;
  struct forward_node_s *next;
};

struct forward_list_s
{
  struct forward_node_s *head;
  size_t size_element;
  size_t size;
  dsconf *conf;
};

inline int forward_list_new(forward_list *list, size_t size_element, dsconf *conf)
{
  list->conf = conf;
  list->size_element = size_element;
  list->size = 0;
  list->head = NULL;
}

inline bool forward_list_empty(forward_list *list)
{
  if(list->size == 0)
  {
    return true;
  }
  return false;
}

inline size_t forward_list_size(forward_list *list)
{
  return list->size;
}

inline void *forward_list_front(forward_list *list)
{
  if(list->size == 0)
  {
    return NULL;
  }
  return list->head->data;
}

int forward_list_push_front(forward_list *list, void *element);

inline void *forward_list_pop_front(forward_list *list)
{
  if(list->size == 0)
  {
    return NULL;
  }

  struct forward_node_s *node = list->head;
  byte_t *data = node->data;
  list->head = list->head->next;
  free(node);
  list->size--;

  return data;
}


inline int forward_list_insert_after(forward_list *list, size_t index, void *element)
{
  if(list->size == 0 || index >= list->size)
  {
    return -1;
  }

  struct forward_node_s *insert = malloc(sizeof(struct forward_node_s));
  byte_t *data = malloc(sizeof(list->size_element));
  
  if(insert == NULL || data == NULL)
  {
    free(insert);
    free(data);
    return -1;
  }

  if(list->conf != NULL && list->conf->copy_ds != NULL) 
  {
    list->conf->copy_ds(element, data);
  }
  else
  {
    memcpy(data, element, list->size_element);
  }
  insert->data = data;

  struct forward_node_s *tmp = list->head;
  for(size_t i = 0; i < index; i++)
  {
    tmp = tmp->next;
  }
  insert->next = tmp->next;
  tmp->next = insert;
  list->size++;

  return 0;
}

inline int forward_list_erase_after(forward_list *list, size_t index)
{
  if(list->size == 0 || index >= list->size - 1)
  {
    return -1;
  }

  struct forward_node_s *tmp = list->head;
  struct forward_node_s *new_next = NULL;
  for(size_t i = 0; i < index; i++)
  {
    tmp = tmp->next;
  }
  new_next = tmp->next->next;

  if(list->conf != NULL && list->conf->delete_ds != NULL)
  {
    list->conf->delete_ds(tmp->next->data);
  }
  free(tmp->next->data);
  free(tmp->next);
  tmp->next = new_next;
  list->size--;

  return 0;
}

void forward_list_clear(forward_list *list);
void forward_list_delete(forward_list *list);

#endif