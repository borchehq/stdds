#include <string.h>

#include "forward_list.h"

extern int forward_list_new(forward_list *list, size_t size_element, dsconf *conf);
extern bool forward_list_empty(forward_list *list);
extern size_t forward_list_size(forward_list *list);
extern void *forward_list_front(forward_list *list);

int forward_list_push_front(forward_list *list, void *element)
{
  struct forward_node_s *new_node = malloc(sizeof(struct forward_node_s));
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
  if(list->head == NULL)
  {
    new_node->next = NULL;
    list->head = new_node;
  }
  else
  {
    new_node->next = list->head;
    list->head = new_node;
  }
  list->size++;

  return 0;
}

extern void *forward_list_pop_front(forward_list *list);
extern int forward_list_insert_after(forward_list *list, size_t index, void *element);
extern int forward_list_erase_after(forward_list *list, size_t index);

void forward_list_clear(forward_list *list)
{
  struct forward_node_s *tmp = list->head;
  struct forward_node_s *next = NULL;
  while(tmp != NULL)
  {
    next = tmp->next;
    if(list->conf != NULL && list->conf->delete_ds != NULL)
    {
      list->conf->delete_ds(tmp->data);
    }
    free(tmp->data);
    free(tmp);
    tmp = next;
  }
  list->size = 0;
  list->head = NULL;
}

void forward_list_delete(forward_list *list)
{
  struct forward_node_s *tmp = list->head;
  struct forward_node_s *next = NULL;
  while(tmp != NULL)
  {
    next = tmp->next;
    if(list->conf != NULL && list->conf->delete_ds != NULL)
    {
      list->conf->delete_ds(tmp->data);
    }
    free(tmp->data);
    free(tmp);
    tmp = next;
  }
  list->size = 0;
  list->head = NULL;
}