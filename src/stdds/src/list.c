#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

extern int list_new(list *list, size_t size_element, dsconf *conf);

int list_push_front(list *list, void *element)
{
  struct node_s *node = malloc(sizeof(struct node_s));
  byte_t *data = malloc(list->size_element);

  if(node == NULL || data == NULL)
  {
    free(list->head);
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
  node->data = data;
  if(list->size == 0)
  {
    node->prev = NULL;
    node->next = NULL;
    list->head = list->tail = node;
  }
  else
  {
    node->prev = NULL;
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
  }

  list->size++;
  return 0;
}

extern void *list_pop_front(list *list);

int list_push_back(list *list, void *element) 
{
  struct node_s *node = malloc(sizeof(struct node_s));
  byte_t *data = malloc(list->size_element);

  if(node == NULL || data == NULL)
  {
    free(list->head);
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
  node->data = data;
  if(list->size == 0)
  {
    node->prev = NULL;
    node->next = NULL;
    list->head = list->tail = node;
  }
  else
  {
    node->prev = list->tail;
    node->next = NULL;
    list->tail->next= node;
    list->tail = node;
  }

  list->size++;
  return 0;
}

extern void *list_pop_back(list *list);

int list_insert(list *list, size_t position, void *element)
{
  struct node_s *new_node = malloc(sizeof(struct node_s));
  byte_t *data = malloc(list->size_element);

  if(new_node == NULL || data == NULL)
  {
    free(new_node);
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
  new_node->data = data;

  if(list->size == 0)
  {
    new_node->next = NULL;
    new_node->prev = NULL;
    list->head = new_node;
    list->tail = new_node;
    list->size++;
    return 0;
  }
  if(position >= list->size)
  {
    list->tail->next = new_node;
    new_node->prev = list->tail;
    new_node->next = NULL;
    list->tail = new_node;
    list->size++;
    return 0;
  }
  if(position == 0)
  {
    list->head->prev = new_node;
    new_node->next = list->head;
    new_node->prev = NULL;
    list->head = new_node;
    list->size++;
    return 0;
  }

  struct node_s *tmp = list->head->next;
  for(size_t i = 1; i < position; i++)
  {
    tmp = tmp->next;
  }
  new_node->next = tmp;
  new_node->prev = tmp->prev;
  tmp->prev = new_node;
  new_node->prev->next = new_node;
  list->size++;
  return 0;
}

extern int list_erase(list *list, size_t position);

void list_clear(list *list)
{
  if(list->size == 0)
  {
    return;
  }
  
  struct node_s *tmp;
  while(list->head != NULL)
  { 
    tmp = list->head->next;
    if(list->conf != NULL && list->conf->delete_ds != NULL)
    {
      list->conf->delete_ds(list->head->data);
    }
    free(list->head->data);
    free(list->head);
    list->head = tmp;
  }
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}

void list_delete(list *list)
{
  if(list->size == 0)
  {
    return;
  }
  
  struct node_s *tmp;
  while(list->head != NULL)
  { 
    tmp = list->head->next;
    if(list->conf != NULL && list->conf->delete_ds != NULL)
    {
      list->conf->delete_ds(list->head->data);
    }
    free(list->head->data);
    free(list->head);
    list->head = tmp;
  }
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}