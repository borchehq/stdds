#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int list_new(list *list, size_t size_element, dsconf *conf)
{
  list->head = NULL;
  list->tail = NULL;
  list->size_element = size_element;
  list->size = 0;
  list->conf = conf;
}

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

  memcpy(data, element, list->size_element);
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

void *list_pop_front(list *list)
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

  memcpy(data, element, list->size_element);
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

void *list_pop_back(list *list)
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

  memcpy(data, element, list->size_element);
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

int list_erase(list *list, size_t position)
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
  if(list->conf != NULL && list->conf->delete_ds != NULL)
  {
    list->conf->delete_ds(tmp->data);
  }
  free(tmp->data);
  free(tmp);
  list->size--;

  return 0;
}

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