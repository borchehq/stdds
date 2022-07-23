#include <stdlib.h>
#include <string.h>

#include "deque.h"

static int deque_reserve_front(deque *deque)
{
  ptrdiff_t offset = deque->front.block - deque->block_map_first;
  ptrdiff_t range_map = 1 + deque->block_map_last - deque->block_map_first;
  ptrdiff_t offset_back = deque->back.block - deque->front.block;
  byte_t **tmp = realloc(deque->block_map_first,
                         range_map * 2 * sizeof(byte_t*));
  if(tmp != NULL)
  {
    memmove(tmp + offset + range_map,
            tmp + offset, sizeof(byte_t*) * (offset_back + 1));
    deque->block_map_first = tmp;
    deque->block_map_last = tmp + 2 * range_map - 1;
    deque->front.block = tmp + offset + range_map;
    deque->back.block = deque->front.block + offset_back;
    return 0;
  }

  return -1;
}


static int deque_reserve_back(deque *deque)
{
  ptrdiff_t offset = deque->front.block - deque->block_map_first;
  ptrdiff_t range_map = 1 + deque->block_map_last - deque->block_map_first;
  ptrdiff_t offset_back = deque->back.block - deque->front.block;
  byte_t **tmp = realloc(deque->block_map_first,
                         range_map * 2 * sizeof(byte_t*));

  if(tmp != NULL)
  {
    deque->block_map_first = tmp;
    deque->block_map_last = tmp + 2 * range_map - 1;
    deque->front.block = tmp + offset;
    deque->back.block = deque->front.block + offset_back;
    return 0;
  }
  return -1;
}

int deque_new(deque *deque, size_t size_type, dsconf *conf)
{
  byte_t **block_map = malloc(sizeof(byte_t*) * SIZE_MAP);

  if(block_map == NULL)
  {
    return -1;
  }

  for(size_t i = 0; i < SIZE_MAP; i++) 
  {
    block_map[i] = calloc(SIZE_BLOCK, size_type);
    if(block_map[i] == NULL) 
    {
      for(size_t j = 0; j < i + 1; j++)
      {
        free(block_map[j]);
      }
      free(block_map);
      return -1;
    }
  }

  if(conf != NULL)
  {
    deque->conf.construct_ds = conf->construct_ds;
    deque->conf.copy_ds = conf->copy_ds;
    deque->conf.delete_ds = conf->delete_ds;
  }
  else
  {
    deque->conf.construct_ds = NULL;
    deque->conf.copy_ds = NULL;
    deque->conf.delete_ds = NULL;
  }
  deque->back.current = NULL;
  deque->back.first = block_map[SIZE_MAP / 2];
  deque->back.last = block_map[SIZE_MAP / 2] + size_type * (SIZE_BLOCK - 1);
  deque->back.block = &block_map[SIZE_MAP / 2];
  deque->front.current = NULL;
  deque->front.first = block_map[SIZE_MAP / 2];
  deque->front.last = block_map[SIZE_MAP / 2] + size_type * (SIZE_BLOCK - 1);
  deque->front.block = &block_map[SIZE_MAP / 2];
  deque->size = 0;
  deque->block_map_first = block_map;
  deque->block_map_last = block_map;
  deque->size_type = size_type;

  return 0;
}

void deque_delete(deque *deque)
{ 
  if(deque == NULL)
  {
    return;
  }

  byte_t **front = deque->front.block;
  byte_t **back = deque->back.block;
  
  if(deque->conf.delete_ds != NULL && deque->front.current != NULL && deque->back.current != NULL)
  {
    if(front == back)
    {
      for(byte_t *j = deque->front.current; j <= deque->back.current; j += deque->size_type)
      {
        deque->conf.delete_ds(j);
      }
    }
    else
    {
      for(byte_t *j = deque->front.current; j <= deque->front.last; j += deque->size_type)
      {
        deque->conf.delete_ds(j);
      }
      for(byte_t *j = deque->back.first; j <= deque->back.current; j += deque->size_type)
      {
        deque->conf.delete_ds(j);
      }
    }
    
  }
  if(front == back)
  {
    free(*deque->front.block);
  }
  else
  {
    free(*deque->front.block);
    free(*deque->back.block);
  }
  for(byte_t **i = front + 1; i <= back - 1; i++) 
  {
    if(deque->conf.delete_ds != NULL)
    {
      for(size_t j = 0; j < SIZE_BLOCK; j++)
      {
        deque->conf.delete_ds(&i[0][j * deque->size_type]);
      }
    }
    free(i[0]);
  }
  free(deque->block_map_first);
}

int deque_push_back(deque *deque, void *element)
{
  int status = 0;

  if(deque->back.current == deque->back.last)
  {
   if(deque->back.block == deque->block_map_last)
    {
      status = deque_reserve_back(deque);
      if(status == -1)
      {
        return -1;
      }
    }

   *(deque->back.block + 1) = calloc(SIZE_BLOCK, deque->size_type);
    if(*(deque->back.block + 1) == NULL)
    {
      return -1;
    }
    deque->back.block++;
    deque->back.current = *deque->back.block;
    deque->back.first = deque->back.current;
    deque->back.last = deque->back.current + 
                       deque->size_type * (SIZE_BLOCK - 1);
  }
  else
  {
    if(deque->back.current == NULL)
    {
      deque->back.current = deque->back.first;
      deque->front.current = deque->back.first;
    }
    else
    {
      deque->back.current = deque->back.current + deque->size_type;
    }
  }

  if(deque->conf.copy_ds != NULL) 
  {
    deque->conf.copy_ds(element, deque->back.current);
  }
  else
  {
    memcpy(deque->back.current, element, deque->size_type);
  }
  deque->size++;

  return 0;
}

int deque_push_front(deque *deque, void *element)
{
  int status = 0;

  if(deque->front.current == deque->front.first)
  {
   if(deque->front.block == deque->block_map_first)
    {
      status = deque_reserve_front(deque);
      if(status == -1)
      {
        return -1;
      }
    }

   *(deque->front.block - 1) = calloc(SIZE_BLOCK, deque->size_type);
    if(*(deque->front.block - 1) == NULL)
    {
      return -1;
    }
    deque->front.block--;
    deque->front.current = *deque->front.block + 
                           deque->size_type * (SIZE_BLOCK - 1);
    deque->front.first = *deque->front.block;
    deque->front.last = deque->front.current;
  }
  else
  {
    if(deque->front.current == NULL)
    {
      deque->back.current = deque->front.first;
      deque->front.current = deque->front.first;
    }
    else
    {
      deque->front.current = deque->front.current - deque->size_type;
    }
  }

  if(deque->conf.copy_ds != NULL) 
  {
    deque->conf.copy_ds(element, deque->front.current);
  }
  else
  {
    memcpy(deque->front.current, element, deque->size_type);
  }
  deque->size++;

  return 0;
}

void *deque_pop_back(deque *deque)
{
  if(deque->back.current == NULL)
  {
    return NULL;
  }
  void *ret = malloc(deque->size_type);
  if(ret == NULL)
  {
    return NULL;
  }
  memcpy(ret, deque->back.current, deque->size_type);
  if(deque->back.current == deque->back.first)
  {
    if(deque->back.block != deque->front.block)
    {
      deque->back.block--;
      deque->back.first = *deque->back.block;
      deque->back.last = deque->back.first + (deque->size_type * (SIZE_BLOCK - 1));
      deque->back.current = deque->back.last;
      free(*(deque->back.block + 1));
    }
    else
    {
      deque->back.current = NULL;
      deque->front.current = NULL;
    }

    ptrdiff_t range_active = deque->back.block - deque->front.block + 1;
    ptrdiff_t range_map = deque->block_map_last - deque->block_map_first + 1;
    ptrdiff_t free_front = deque->front.block - deque->block_map_first;
    ptrdiff_t free_back = deque->block_map_last - deque->back.block;

    if(range_map >= 2 * range_active && range_map > 1)
    {
      memmove(deque->front.block - free_front / 2, deque->front.block,
              range_active * sizeof(byte_t*));
      void *tmp = realloc(deque->block_map_first, (range_map - free_front / 2 - free_back / 2) * sizeof(byte_t*));
      if(tmp == NULL)
      {
        memmove(deque->front.block + free_front / 2, deque->front.block,
                range_active * sizeof(byte_t*));
        free(ret);
        return NULL;
      }
      deque->block_map_first = tmp;
      deque->block_map_last = deque->block_map_first + (range_map - free_front / 2 - free_back / 2) - 1;
      deque->back.block = deque->block_map_last - (free_back - free_back / 2);
      deque->front.block = deque->block_map_first + (free_front - free_front / 2);
    }
  }
  else
  {
    deque->back.current -= deque->size_type;
  }
  deque->size--;
  return ret;
}

void *deque_pop_front(deque *deque)
{
  if(deque->front.current == NULL)
  {
    return NULL;
  }
  void *ret = malloc(deque->size_type);
  if(ret == NULL)
  {
    return NULL;
  }
  memcpy(ret, deque->front.current, deque->size_type);
   if(deque->front.current == deque->front.last)
  {
    if(deque->front.block != deque->back.block)
    {
      deque->front.block++;
      deque->front.first = *deque->front.block;
      deque->front.last = deque->front.first + (deque->size_type * (SIZE_BLOCK - 1));
      deque->front.current = deque->front.first;
      free(*(deque->front.block - 1));
    }
    else
    {
      deque->back.current = NULL;
      deque->front.current = NULL;
    }

    ptrdiff_t range_active = deque->back.block - deque->front.block + 1;
    ptrdiff_t range_map = deque->block_map_last - deque->block_map_first + 1;
    ptrdiff_t free_front = deque->front.block - deque->block_map_first;
    ptrdiff_t free_back = deque->block_map_last - deque->back.block;

    if(range_map >= 2 * range_active && range_map > 1)
    {
      memmove(deque->front.block - free_front / 2, deque->front.block,
              range_active * sizeof(byte_t*));
      void *tmp = realloc(deque->block_map_first, (range_map - free_front / 2 - free_back / 2) * sizeof(byte_t*));
      if(tmp == NULL)
      {
        memmove(deque->front.block + free_front / 2, deque->front.block,
                range_active * sizeof(byte_t*));
        free(ret);
        return NULL;
      }
      deque->block_map_first = tmp;
      deque->block_map_last = deque->block_map_first + (range_map - free_front / 2 - free_back / 2) - 1;
      deque->back.block = deque->block_map_last - (free_back - free_back / 2);
      deque->front.block = deque->block_map_first + (free_front - free_front / 2);
    }
  }
  else
  {
    deque->front.current += deque->size_type;
  }
  deque->size--;
  return ret;
}

void *deque_at(deque *deque, size_t index)
{
  size_t size_type = deque->size_type;
  size_t size = deque->size;
  
  if(size - 1 < index)
  {
    return NULL;
  }

  ptrdiff_t offset = ((byte_t*)deque->front.current - 
                      (byte_t*)deque->front.first) / size_type;
  void *val = &(deque->front.block
              [(index + offset) / SIZE_BLOCK]
              [((index + offset) % SIZE_BLOCK) * size_type]);
  return val;
}

void *deque_back(deque *deque)
{
  return deque->back.current;
}

void *deque_front(deque *deque)
{
  return deque->front.current;
}

size_t deque_size(deque *deque)
{
  return deque->size;
}

bool deque_empty(deque *deque)
{
  if(deque->front.current != NULL || deque->back.current != NULL)
  {
    return true;
  }

  return false;
}

int deque_assign(deque *deque, void *element, size_t index)
{ 
  size_t size_type = deque->size_type;
  size_t size = deque->size;  

  if(size - 1 < index)
  {
    return -1;
  }

  ptrdiff_t offset = ((byte_t*)deque->front.current - 
                      (byte_t*)deque->front.first) / size_type;
  void *val = &(deque->front.block
              [(index + offset) / SIZE_BLOCK]
              [((index + offset) % SIZE_BLOCK) * size_type]);

  if(deque->conf.copy_ds != NULL && deque->conf.delete_ds != NULL) 
  {
    deque->conf.delete_ds(val);
    deque->conf.copy_ds(element, val);
  }
  else
  {
    memmove(val, element, size_type);
  }

  return 0;
}

int deque_insert(deque *deque, void *element, size_t index)
{
  size_t size_type = deque->size_type;
  size_t size = deque->size;  
  int status = 0;

  if(index == 0)
  {
    return deque_push_front(deque, element);
  }

  if(index >= size)
  {
    return deque_push_back(deque, element);
  }

  if(deque->size - index <= index)
  {
    if(deque->back.current == deque->back.last)
    {
      if(deque->back.block == deque->block_map_last)
        {
          status = deque_reserve_back(deque);
          if(status == -1)
          {
            return -1;
          }
        }

      *(deque->back.block + 1) = calloc(SIZE_BLOCK, deque->size_type);
        if(*(deque->back.block + 1) == NULL)
        {
          return -1;
        }
        deque->back.block++;
        deque->back.current = *deque->back.block;
        deque->back.first = deque->back.current;
        deque->back.last = deque->back.current + 
                          deque->size_type * (SIZE_BLOCK - 1);
    }
    else
    {
      if(deque->back.current == NULL)
      {
        deque->back.current = deque->back.first;
        deque->front.current = deque->back.first;
      }
      else
      {
        deque->back.current = deque->back.current + deque->size_type;
      }
    }
  }
  else 
  {
    if(deque->front.current == deque->front.first)
    {
      if(deque->front.block == deque->block_map_first)
      {
        status = deque_reserve_front(deque);
        if(status == -1)
        {
          return -1;
        }
      }

      *(deque->front.block - 1) = calloc(SIZE_BLOCK, deque->size_type);
      if(*(deque->front.block - 1) == NULL)
      {
        return -1;
      }
      deque->front.block--;
      deque->front.current = *deque->front.block + 
                              deque->size_type * (SIZE_BLOCK - 1);
      deque->front.first = *deque->front.block;
      deque->front.last = deque->front.current;
      index++; // compensate for adding a new element to the front
    }
    else
    {
      if(deque->front.current == NULL)
      {
        deque->back.current = deque->front.first;
        deque->front.current = deque->front.first;
      }
      else
      {
        deque->front.current = deque->front.current - deque->size_type;
      }
    }
  }

  ptrdiff_t offset = ((byte_t*)deque->front.current - 
                      (byte_t*)deque->front.first) / size_type;
  size_t offset_back = ((byte_t*)deque->back.current - 
                        (byte_t*)deque->back.first);
  size_t offset_front = (byte_t*)deque->front.last - 
                        (byte_t*)deque->front.current;                                   
  byte_t *val = &(deque->front.block
              [(index + offset) / SIZE_BLOCK]
              [((index + offset) % SIZE_BLOCK) * size_type]);
  size_t offset_val;
  byte_t **start = NULL;
  byte_t **end = &(deque->front.block[(index + offset) / SIZE_BLOCK]);
  if(deque->size - index <= index)
  {
    offset_val = ((byte_t*)deque->back.current - val);
    start = deque->back.block;
    end = &(deque->front.block[(index + offset) / SIZE_BLOCK]);
    if(start == end) {
      memmove(val + size_type, val, offset_val);
    }
    else 
    {
      memmove(deque->back.first + size_type, deque->back.first, offset_back);
      memcpy(deque->back.first, &((start - 1)[0][(SIZE_BLOCK - 1) * size_type]), size_type);
      for(byte_t **i = start - 1; i >= end + 1; i--)
      {
        memmove(*i + size_type, *i, (SIZE_BLOCK - 1) * size_type);
        memcpy(*i, &((i - 1)[0][(SIZE_BLOCK - 1) * size_type]), size_type);
      }
      size_t offset_val_end = &end[0][(SIZE_BLOCK - 1) * size_type] - val;
      memmove(val + size_type, val, offset_val_end);
    }
  }
  else
  {
    offset_val = ((byte_t*)deque->front.current - val);
    start = deque->front.block;
    if(start == end) {
      memmove((byte_t*)deque->front.current, 
              (byte_t*)deque->front.current + size_type, offset_val);
    }
      else 
    {
      memmove((byte_t*)deque->front.current, (byte_t*)deque->front.current + size_type, offset_front);
      memcpy(deque->front.last, &((start + 1)[0][0]), size_type);
      for(byte_t **i = start + 1; i <= end - 1; i++)
      {
        memmove(*i, *i + size_type, (SIZE_BLOCK - 1) * size_type);
        memcpy(&i[0][(SIZE_BLOCK - 1) * size_type], &(i + 1)[0][0], size_type);
      }
      size_t offset_val_end = val - &end[0][0] + size_type;
      memmove(&end[0][0], &end[0][1], offset_val_end);
    }
  }
   if(deque->conf.copy_ds != NULL) 
    {
      deque->conf.copy_ds(element, val);
    }
    else
    {
      memcpy(val, element, size_type);
    }
  deque->size++;
}

int deque_erase(deque *deque, size_t index)
{
  size_t size_type = deque->size_type;
  size_t size = deque->size;
  size_t n_blocks = deque->block_map_last - deque->block_map_first;
}