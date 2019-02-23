#ifndef VECTOR_FDT_H
#define VECTOR_FDT_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAP 8
#define MIN_CAP     8

typedef struct type_conf_s dsconf;
typedef struct vector_s vector;
typedef unsigned char byte_t;

struct type_conf_s
{
  int (*clone_ds)(void *data, void *res);
  void (*delete_ds)(void *data);
};

struct vector_s
{
  byte_t *data;
  size_t allocated;
  size_t occupied;
  size_t size_element;
  dsconf conf;
};

/**
*   @brief Creates a new vector instance.
*   @param delete_ds Optional helper function for deleting struct content.
*   delete_ds will be ignored if NULL is passed.
*   @param size_element Size of the elements in bytes.
*   @param initial_size Initial size of the vector.
*   @return Returns 0 on success and -1 if an error occured.
**/
inline int new_vector(vector *vec, size_t size_element, size_t initial_size,
                      dsconf conf)
{
  if(initial_size == 0)
  {
    initial_size = INITIAL_CAP;
  }
  vec->data = malloc(size_element * initial_size);
  if(vec->data == NULL)
  {
    return -1;
  }
  vec->allocated = initial_size;
  vec->occupied = 0;
  vec->size_element = size_element;
  vec->conf = conf;
  return 0;
}

/**
*   @brief Destructs the vec and frees the associated memory.
**/
inline void delete_vector(vector *vec)
{
  if(vec == NULL)
  {
    return;
  }
  if(vec->conf.delete_ds != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      vec->conf.delete_ds(&vec->data[vec->size_element * i]);
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
inline void *vec_at(vector *vec, size_t index)
{
  if(index >= vec->occupied)
  {
    return NULL;
  }
  return &vec->data[vec->size_element * index];
}

/**
*   @brief Checks whether the vector is empty.
*   @param vec The vector that shall be checked.
*   @return Returns true if the vector is empty and false otherwise.
**/
inline bool is_empty(vector *vec)
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
inline bool index_valid(vector *vec, size_t index)
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
inline int rem(vector *vec, size_t index)
{
  size_t offset;
  void *tmp = vec->data;
  // Check whether index is out of bounds.
  if(!index_valid(vec, index))
  {
    return -2;
  }
  offset = vec->occupied - 1 - index;
  if(vec->conf.delete_ds != NULL)
  {
    vec->conf.delete_ds(vec_at(vec, index));
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
inline int push(vector *vec, void *element)
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
  memcpy(&vec->data[vec->occupied * vec->size_element],
         element, vec->size_element);
  vec->occupied++;
  return 0;
}

inline void *pop(vector *vec)
{
  if(is_empty(vec))
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
  rem(vec, vec->size_element - 1);
  return ret;
}

/**
*   @brief Inserts the specified element at the specified position in the vector
*   and shifts the element currently at that position and any subsequent
*   elements to the right.
*   @return Returns -1 on failure and 0 on success.
**/
inline int insert(vector *vec, void *element, size_t index)
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
    memcpy(&vec->data[vec->occupied * vec->size_element],
           element, vec->size_element);
  }
  // Move all elements beginning from index to the right.
  else
  {
    memmove(&vec->data[(index + 1) * vec->size_element],
            &vec->data[index * vec->size_element],
            (vec->occupied - index) * vec->size_element);
    memmove(&vec->data[index * vec->size_element], element,
            vec->size_element);
  }
  vec->occupied++;
  return 0;
}

/**
*   @brief Retrieves the element at the specified index.
*   @return The corresponding element on success and NULL on error.
**/
inline void *get(vector *vec, size_t index)
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
inline int set(vector *vec, void *element, size_t index)
{
  if(index >= vec->occupied)
  {
    return -1;
  }
  memmove(&vec->data[index * vec->size_element], element,
          vec->size_element);
  return 0;
}

/**
*   @brief returns the last element in the vec
**/
inline void *get_last(vector *vec)
{
  if(is_empty(vec))
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
inline size_t size(const vector *vec)
{
  return vec->occupied;
}

/**
*   @brief Returns an array with the content of the specified vector type.
*   @return Returns an array with the original content on success and NULL on
*   error.
**/
inline void *to_array(const vector *vec)
{
  void *array = malloc(vec->occupied * vec->size_element);
  if(array == NULL){
    return NULL;
  }
  memcpy(array, vec->data, vec->occupied * vec->size_element);
  return array;
}

/**
*   @brief Merges to vectors.
*   @param vec_1 The vector who is merged into.
*   @param vec_2 The vector which will be merged in vec_1. vec_2 will be deleted
*   afterwards.
*   @return Returns 0 on success and -1 on failure.
**/
inline int merge(vector *restrict vec_1, vector *restrict vec_2)
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
  vec_2->conf = (dsconf){NULL, NULL};
  delete_vector(vec_2);
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
inline int split(vector *vec, vector *res, size_t index)
{
  if(index >= vec->occupied)
  {
    return -1;
  }

  int stat = new_vector(res, vec->size_element, (vec->occupied) - index,
  vec->conf);

  if(stat == -1)
  {
    return -1;
  }

  if(vec->conf.clone_ds != NULL)
  {
    for(size_t i = index; i < vec->occupied; i++)
    {
      vec->conf.clone_ds(&vec->data[i * vec->size_element],
      &res->data[(i - index) * vec->size_element]);
      vec->conf.delete_ds(&vec->data[i * vec->size_element]);
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
inline int clone(vector *const vec, vector *res)
{
  int stat = new_vector(res, vec->size_element, vec->allocated, vec->conf);
  if(stat == -1)
  {
    return -1;
  }
  res->occupied = vec->occupied;
  if(vec->conf.clone_ds != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      int stat = vec->conf.clone_ds(&vec->data[i * vec->size_element],
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
inline void sortv(vector *vec, int (*compar)(const void *, const void *))
{
  qsort(vec->data, vec->occupied, vec->size_element, compar);
}

#endif
