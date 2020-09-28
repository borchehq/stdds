#ifndef VECTOR_FDT_H
#define VECTOR_FDT_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

#define INITIAL_CAP 8
#define MIN_CAP     8

typedef struct vector_s vector;

struct vector_s
{
  byte_t *data;
  size_t allocated;
  size_t occupied;
  size_t size_element;
  dsconf *conf;
};

/**
*   @brief Creates a new vector instance.
*   @param delete_ds Optional helper function for deleting struct content.
*   delete_ds will be ignored if NULL is passed.
*   @param size_element Size of the elements in bytes.
*   @param initial_size Initial size of the vector.
*   @return Returns 0 on success and -1 if an error occured.
**/
inline int vector_new(vector *vec, size_t size_element, size_t initial_capacity,
                      dsconf *conf)
{
  if(initial_capacity == 0)
  {
    initial_capacity = INITIAL_CAP;
  }
  vec->data = malloc(size_element * initial_capacity);
  if(vec->data == NULL)
  {
    return -1;
  }
  vec->allocated = initial_capacity;
  vec->occupied = 0;
  vec->size_element = size_element;
  vec->conf = conf;
  return 0;
}

/**
*   @brief Destructs the vec and frees the associated memory.
**/
inline void vector_delete(vector *vec)
{
  if(vec == NULL)
  {
    return;
  }
  if(vec->conf != NULL && vec->conf->delete_ds != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      vec->conf->delete_ds(&vec->data[vec->size_element * i]);
    }
  }
  free(vec->data);
}

/**
*   @brief Returns a pointer to the element at index.
*   @param vec The vector of which a pointer to the element with index index
*   shall be returned.
*   @param index Index of the vector to be looked at.
*   @return Returns a pointer to the element in the vector with index index.
**/
inline void *vector_at(vector *vec, size_t index)
{
  if(index >= vec->occupied)
  {
    return NULL;
  }
  return &vec->data[vec->size_element * index];
}

inline void *vector_front(vector *vec)
{
  return &vec->data[0];
}

inline void *vector_back(vector *vec)
{
 return &vec->data[vec->size_element * (vec->occupied - 1)];
}

/**
*   @brief Checks whether the vector is empty.
*   @param vec The vector that shall be checked.
*   @return Returns true if the vector is empty and false otherwise.
**/
inline bool vector_empty(vector *vec)
{
  if(vec->occupied == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
*   @brief Checks wheter the given index is out of bounds.
*   @param vec The vector that shall be checked..
*   @param index The index that shall be checked.
*   @return Returns true if vector has an element on position index.
**/
inline bool vector_index_valid(vector *vec, size_t index)
{
  if(vec->occupied <= index)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/**
*   @brief Removes the element at position index and if applicable
*   calls delete_ds().
*   @return Returns -1 if no memory, -2 if illegal index and 0 on success.
**/
inline int vector_remove(vector *vec, size_t index)
{
  size_t offset;
  void *tmp = vec->data;
  // Check whether index is out of bounds.
  if(!vector_index_valid(vec, index))
  {
    return -2;
  }
  offset = vec->occupied - 1 - index;
  if(vec->conf != NULL && vec->conf->delete_ds != NULL)
  {
    vec->conf->delete_ds(vector_at(vec, index));
  }
  // Close the gap if not the last element.
  if(index < vec->occupied - 1)
  {
    memmove(&vec->data[index * vec->size_element],
            &vec->data[(index + 1) * vec->size_element],
            offset * vec->size_element);
  }
  vec->occupied--;
  if(vec->allocated >= 2 * MIN_CAP && vec->allocated >= 2 * vec->occupied)
  {
    tmp = realloc(tmp, (vec->allocated / 2) * vec->size_element);
    if(tmp != NULL){
      vec->data = tmp;
      vec->allocated /= 2;
    }
  }
  return 0;
}

/**
*   @brief Appends a new element at the end of the vector.
*   @return Returns -1 if no memory is available and 0 on success.
**/
inline int vector_push_back(vector *vec, void *element)
{
  byte_t *tmp = vec->data;
  if(vec->allocated == vec->occupied)
  {
    tmp = realloc(tmp, vec->allocated * vec->size_element * 2);
    if(tmp != NULL)
    {
      vec->data = tmp;
      vec->allocated *= 2;
    }
    else
    {
      return -1;
    }
  }
  
  if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
  {
    vec->conf->copy_ds(element, &vec->data[vec->occupied * vec->size_element]);
  }
  else
  {
    memcpy(&vec->data[vec->occupied * vec->size_element],
           element, vec->size_element);
  }
  vec->occupied++;
  
  return 0;
}

inline void *vector_pop_back(vector *vec)
{
  if(vec->occupied == 0)
  {
    return NULL;
  }
  void *elem = &vec->data[(vec->occupied - 1) * vec->size_element];
  void *ret = malloc(vec->size_element);
  if(ret == NULL)
  {
    return NULL;
  }
  memcpy(ret, elem, vec->size_element);
  // Delete elem.
  void *tmp = vec->data;
  if(vec->conf != NULL && vec->conf->delete_ds != NULL)
  {
    vec->conf->delete_ds(elem);
  }  
  vec->occupied--;
  if(vec->allocated >= 2 * MIN_CAP && vec->allocated >= 2 * vec->occupied)
  {
    tmp = realloc(tmp, (vec->allocated / 2) * vec->size_element);
    if(tmp != NULL)
    {
      vec->data = tmp;
      vec->allocated /= 2;
    }
  }
  return ret;
}

/**
*   @brief Inserts the specified element at the specified position in the vector
*   and shifts the element currently at that position and any subsequent
*   elements to the right.
*   @return Returns -1 on failure and 0 on success.
**/
inline int vector_insert(vector *vec, void *element, size_t index)
{
  byte_t *tmp = vec->data;
  if(vec->allocated == vec->occupied)
  {
    tmp = realloc(tmp, vec->allocated * vec->size_element * 2);
    if(tmp != NULL)
    {
      vec->data = tmp;
      vec->allocated *= 2;
    }
    else
    {
      return -1;
    }
  }
  // Just append.
  if(index >= vec->occupied)
  {
    if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
    {
      vec->conf->copy_ds(element, &vec->data[vec->occupied * vec->size_element]);
    }
    else
    {
      memcpy(&vec->data[vec->occupied * vec->size_element],
             element, vec->size_element);
    }
  }
  // Move all elements beginning from index to the right.
  else
  {
    memmove(&vec->data[(index + 1) * vec->size_element],
            &vec->data[index * vec->size_element],
            (vec->occupied - index) * vec->size_element);
    if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
    {
      vec->conf->copy_ds(element, &vec->data[index * vec->size_element]);
    }
    else
    {
      memmove(&vec->data[index * vec->size_element], element,
              vec->size_element);
    }
  }
  vec->occupied++;
  return 0;
}

inline int vector_insert_range(vector *vec, void *restrict data, size_t index, size_t len)
{
  size_t allocated = vec->allocated;
  size_t occupied = vec->occupied;
  byte_t *data_resize = vec->data;

  if(allocated - occupied < len)
  {
    while(allocated - occupied < len)
    {
      allocated *= 2;
    }
    data_resize = realloc(data_resize, allocated * vec->size_element);
    if(data_resize != NULL)
    {
      vec->data = data_resize;
      vec->allocated = allocated;
    }
    else
    {
      return -1;
    }
  }

  // Just append.
  if(index >= vec->occupied)
  {
    if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
    {
      for(size_t i = 0; i < len; i++)
      {
        vec->conf->copy_ds(&((byte_t*)data)[i * vec->size_element], 
                           &vec->data[(vec->occupied + i) * vec->size_element]);
      }
    }
    else
    {
      memcpy(&vec->data[vec->occupied * vec->size_element],
             data, vec->size_element * len);
    }
  }
  // Move all elements beginning from index to the right.
  else
  {
    memmove(&vec->data[(index + len) * vec->size_element],
            &vec->data[index * vec->size_element],
            (vec->occupied - index) * vec->size_element);
    if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
    {
      for(size_t i = 0; i < len; i++)
      {
        vec->conf->copy_ds(&((byte_t*)data)[i * vec->size_element], 
                           &vec->data[(index + i) * vec->size_element]);
      }
    }
    else
    {
      memmove(&vec->data[index * vec->size_element], data,
              vec->size_element * len);
    }
  }
  vec->occupied += len;
  return 0;
}

inline int vector_insert_fill(vector *vec, void *restrict data, size_t index, size_t len)
{
  size_t allocated = vec->allocated;
  size_t occupied = vec->occupied;
  byte_t *data_resize = vec->data;

  if(allocated - occupied < len)
  {
    while(allocated - occupied < len)
    {
      allocated *= 2;
    }
    data_resize = realloc(data_resize, allocated * vec->size_element);
    if(data_resize != NULL)
    {
      vec->data = data_resize;
      vec->allocated = allocated;
    }
    else
    {
      return -1;
    }
  }

  // Just append.
  if(index >= vec->occupied)
  {
    if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
    {
      for(size_t i = 0; i < len; i++)
      {
        vec->conf->copy_ds(data, &vec->data[(vec->occupied + i) * vec->size_element]);
      }
    }
    else
    {
      for(size_t i = 0; i < len; i++)
      {
        memcpy(&vec->data[(vec->occupied + i) * vec->size_element],
               data, vec->size_element);
      }
    }
  }
  // Move all elements beginning from index to the right.
  else
  {
    memmove(&vec->data[(index + len) * vec->size_element],
            &vec->data[index * vec->size_element],
            (vec->occupied - index) * vec->size_element);
    if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
    {
      for(size_t i = 0; i < len; i++)
      {
        vec->conf->copy_ds(data, &vec->data[(index + i) * vec->size_element]);
      }
    }
    else
    {
      for(size_t i = 0; i < len; i++)
      {
        memcpy(&vec->data[(index + i) * vec->size_element], data,
                vec->size_element);
      }
    }
  }
  vec->occupied += len;
  return 0;
}

/**
*   @brief Retrieves the element at the specified index.
*   @return The corresponding element on success and NULL on error.
**/
inline void *vector_get(vector *vec, size_t index)
{
  if(index >= vec->occupied)
  {
    return NULL;
  }
  void *elem = &vec->data[vec->size_element * index];
  void *ret = malloc(vec->size_element);
  if(ret == NULL)
  {
    return NULL;
  }
  memcpy(ret, elem, vec->size_element);
  return ret;
}

/**
*   @brief Sets a new element at the specified index.
*   @return Returns -1 if index is out of bounds and 0 on success.
**/
inline int vector_assign(vector *vec, void *element, size_t index)
{
  if(index >= vec->occupied)
  {
    return -1;
  }

  if(vec->conf != NULL && vec->conf->copy_ds != NULL && vec->conf->delete_ds != NULL) 
  {
    vec->conf->delete_ds(&vec->data[index * vec->size_element]);
    vec->conf->copy_ds(element, &vec->data[index * vec->size_element]);
  }
  else
  {
    memmove(&vec->data[index * vec->size_element], element,
            vec->size_element);
  }
  return 0;
}

inline int vector_assign_range(vector *vec, void *restrict data, size_t len)
{
  if(vec->conf != NULL && vec->conf->delete_ds != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      vec->conf->delete_ds(&vec->data[vec->size_element * i]);
    }
  }

  size_t allocated = vec->allocated;
  byte_t *data_resize = vec->data;
  if(allocated >= 2 * MIN_CAP && allocated >= 2 * len)
  {
    while(allocated >= 2 * MIN_CAP && allocated >= 2 * len)
    {
      allocated /= 2;
    }
    data_resize = realloc(data_resize, allocated * vec->size_element);
    if(data_resize != NULL)
    {
      vec->data = data_resize;
      vec->allocated = allocated;
    }
    else
    {
      return -1;
    }
  }
  else if(allocated < len)
  {
    while(allocated < len)
    {
      allocated *= 2;
    }
    data_resize = realloc(data_resize, allocated * vec->size_element);
    if(data_resize != NULL)
    {
      vec->data = data_resize;
      vec->allocated = allocated;
    }
    else
    {
      return -1;
    }
  }

  if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
  {
    for(size_t i = 0; i < len; i++)
    {
      vec->conf->copy_ds(&((byte_t*)data)[i * vec->size_element], &vec->data[i * vec->size_element]);
    }
  }
  else
  {
    memcpy(vec->data, data, len * vec->size_element);
  }
  vec->occupied = len;
  return 0;
}

inline int vector_assign_fill(vector *vec, void *restrict data, size_t len)
{
  if(vec->conf != NULL && vec->conf->delete_ds != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      vec->conf->delete_ds(&vec->data[vec->size_element * i]);
    }
  }

  size_t allocated = vec->allocated;
  byte_t *data_resize = vec->data;
  if(allocated >= 2 * MIN_CAP && allocated >= 2 * len)
  {
    while(allocated >= 2 * MIN_CAP && allocated >= 2 * len)
    {
      allocated /= 2;
    }
    data_resize = realloc(data_resize, allocated * vec->size_element);
    if(data_resize != NULL)
    {
      vec->data = data_resize;
      vec->allocated = allocated;
    }
    else
    {
      return -1;
    }
  }
  else if(allocated < len)
  {
    while(allocated < len)
    {
      allocated *= 2;
    }
    data_resize = realloc(data_resize, allocated * vec->size_element);
    if(data_resize != NULL)
    {
      vec->data = data_resize;
      vec->allocated = allocated;
    }
    else
    {
      return -1;
    }
  }

  if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
  {
    for(size_t i = 0; i < len; i++)
    {
      vec->conf->copy_ds(data, &vec->data[i * vec->size_element]);
    }
  }
  else
  {
    for(size_t i = 0; i < len; i++)
    {
      memcpy(&vec->data[i * vec->size_element], data, vec->size_element);
    }
  }
  vec->occupied = len;
  return 0;
}

/**
*   @brief returns the last element in the vec
**/
inline void *vector_get_last(vector *vec)
{
  if(vector_empty(vec))
  {
    return NULL;
  }
  void *elem = &vec->data[(vec->occupied - 1) * vec->size_element];
  void *ret = malloc(vec->size_element);
  if(ret == NULL)
  {
    return NULL;
  }
  memcpy(ret, elem, vec->size_element);
  return ret;
}

/**
*   @brief returns the number of elements in the vec.
**/
inline size_t vector_size(const vector *vec)
{
  return vec->occupied;
}

/**
*   @brief Returns an array with the content of the specified vector type.
*   @return Returns an array with the original content on success and NULL on
*   error.
**/
inline void *vector_to_array(const vector *vec)
{
  byte_t *array = malloc(vec->occupied * vec->size_element);
  if(array == NULL){
    return NULL;
  }
  if(vec->conf != NULL && vec->conf->copy_ds != NULL) 
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      vec->conf->copy_ds(&vec->data[i * vec->size_element], &array[i * vec->size_element]);
    }
  }
  else
  {
    memcpy(array, vec->data, vec->occupied * vec->size_element);
  }
  return array;
}

inline int vector_reserve(vector *vec, size_t new_size)
{
  byte_t *tmp = vec->data;
  tmp = realloc(tmp, new_size * vec->size_element);
    if(tmp != NULL)
    {
      vec->data = tmp;
      vec->allocated = new_size;
      if(vec->allocated < vec->occupied)
      {
        vec->occupied = vec->allocated;
      }
      return 0;
    }
    return -1;
}

inline size_t vector_capacity(vector *vec)
{
  return vec->allocated;
}

/**
*   @brief Merges to vectors.
*   @param vec_1 The vector who is merged into.
*   @param vec_2 The vector which will be merged in vec_1. vec_2 will be deleted
*   afterwards.
*   @return Returns 0 on success and -1 on failure.
**/
inline int vector_merge(vector *restrict vec_1, vector *restrict vec_2)
{
  void *tmp = realloc(vec_1->data,
  (vec_1->allocated + vec_2->occupied) * vec_1->size_element);

  if(tmp == NULL)
  {
    return -1;
  }

  vec_1->data = tmp;
  vec_1->allocated += vec_2->occupied;
  memcpy(&vec_1->data[vec_1->occupied * vec_1->size_element], vec_2->data,
  vec_2->occupied * vec_2->size_element);
  vec_1->occupied += vec_2->occupied;

  // We don't want to delete data that is on the heap.
  vec_2->conf = NULL;
  vector_delete(vec_2);
  return 0;
}

/**
*   @brief Splits a vector at position index. The element at position index will
*   be included in res.
*   @param[in] vec The vector that is split. All elements >= index will be
*   deleted.
*   @param[out] res The resulting second vector. res includes all elements with
*   index greater or equal index. res should be uninitialized.
*   @param index The position where the vector shall be split at.
*   @return Returns 0 on success and -1 otherwise.
**/
inline int vector_split(vector *vec, vector *res, size_t index)
{
  if(index >= vec->occupied)
  {
    return -1;
  }

  int stat = vector_new(res, vec->size_element, (vec->occupied) - index,
  vec->conf);

  if(stat == -1)
  {
    return -1;
  }

  if(vec->conf != NULL && vec->conf->copy_ds != NULL)
  {
    for(size_t i = index; i < vec->occupied; i++)
    {
      vec->conf->copy_ds(&vec->data[i * vec->size_element],
      &res->data[(i - index) * vec->size_element]);
      vec->conf->delete_ds(&vec->data[i * vec->size_element]);
    }
  }
  else
  {
    memcpy(res->data, &vec->data[index * vec->size_element],
    ((vec->occupied) - index) * vec->size_element);
  }

  res->occupied = (vec->occupied) - index;
  vec->occupied = index;

  if(vec->allocated >= 2 * MIN_CAP && vec->allocated >= 2 * vec->occupied)
  {
    void *tmp = realloc(vec->data, (vec->allocated / 2) * vec->size_element);
    if(tmp != NULL){
      vec->data = tmp;
      vec->allocated /= 2;
    }
  }

  return 0;
}

/**
*   @brief Clones a vector.
*   @param[in] vec The vector that shall be cloned.
*   @param[out] res The clone of vec. res should be uninitialized.
*   @return Returns 0 on success and -1 on failure.
**/
inline int vector_clone(vector *const vec, vector *res)
{
  int stat = vector_new(res, vec->size_element, vec->allocated, vec->conf);
  if(stat == -1)
  {
    return -1;
  }
  res->occupied = vec->occupied;
  if(vec->conf != NULL && vec->conf->copy_ds != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      int stat = vec->conf->copy_ds(&vec->data[i * vec->size_element],
      &res->data[i * res->size_element]);
      if(stat == -1)
      {
        return -1;
      }
    }
  }
  else
  {
    memcpy(res->data, vec->data, vec->size_element * vec->occupied);
  }
  return 0;
}

/**
*   @brief Sorts a vector (see qsort).
*   @param compar Pointer to compare function (see qsort).
**/
inline void vector_sort(vector *vec, int (*compar)(const void *, const void *))
{
  qsort(vec->data, vec->occupied, vec->size_element, compar);
}

inline int vector_resize(vector *vec, size_t size)
{
  size_t size_realloc = size >= MIN_CAP ? size : MIN_CAP;
  byte_t *tmp = NULL;

  if(size <= vec->occupied)
  {
    if(vec->conf != NULL && vec->conf->delete_ds != NULL) 
    {
      for(size_t i = size; i < vec->occupied; i++)
      {
        vec->conf->delete_ds(&vec->data[i * vec->size_element]);
      }
    }
  }
  
  tmp = realloc(vec->data, size_realloc * vec->size_element);
  if(tmp == NULL)
  {
    return -1;
  }
  
  if(size > vec->occupied)
  {
    if(vec->conf != NULL && vec->conf->construct_ds != NULL) 
    {
      for(size_t i = vec->occupied; i < size; i++)
      { 
        vec->conf->construct_ds(&tmp[i * vec->size_element]);
      }
    }
  }
 
  vec->occupied = size;
  vec->allocated = size_realloc;
  vec->data = tmp;

  return 0;
}

#endif
