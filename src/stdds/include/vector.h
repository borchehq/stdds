#ifndef VECTOR_FDT_H
#define VECTOR_FDT_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 8
#define MIN_CAP     8

typedef struct vector_s vector;
typedef unsigned char byte_t;

struct vector_s{
  byte_t *data;
  size_t allocated;
  size_t occupied;
  size_t size_element;
  void (*delete_datatype)(void *data);
};

/**
*   @brief Creates a new vector instance.
*   @param delete_datatype Optional helper function for deleting struct content.
*   delete_datatype will be ignored if NULL is passed.
*   @param size_element Size of the elements in bytes.
*   @param initial_size Initial size of the vector.
*   @return Returns a new vector on success and NULL on error.
*/
inline int new_vector(vector *vec, size_t size_element,
                      size_t initial_size, void (*delete_datatype)(void *data))
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
  vec->delete_datatype = delete_datatype;
  return 0;
}

/**
*   @brief Destructs the vec and frees the associated memory.
**/
inline void delete_vector(vector *vec)
{
  if(vec->delete_datatype != NULL)
  {
    for(size_t i = 0; i < vec->occupied; i++)
    {
      vec->delete_datatype(&vec->data[vec->size_element * i]);
    }
  }
  free(vec->data);
}

inline void *at(vector *vec, size_t index)
{
  if(index >= vec->occupied)
    {
      return NULL;
    }
  return &vec->data[vec->size_element * index];
}

/**
*   @brief Removes the element at the specified index and if applicable
*   calls delete_datatype().
*   @return Returns -1 if no memory, -2 if illegal index and 0 on success.
*/
inline int rem(vector *vec, size_t index)
{
  size_t offset;
  void *tmp = vec->data;
  // Check whether index is out of bounds.
  if(index >= vec->occupied)
  {
    return -2;
  }
  offset = vec->occupied - 1 - index;
  if(vec->delete_datatype != NULL)
  {
    vec->delete_datatype(at(vec, index));
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
*   @brief Appends a new element to the end of the vector.
*   @return Returns -1 if no memory is available and 0 on success.
*/
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
*   @brief Inserts the specified element at the specified position in this vec
*   and shifts the element currently at that position and any subsequent
*   elements to the right.
*   @return Returns -1 on failure and 0 on success.
*/
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
  else{
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
*/
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
*   @return Returns -1 if illegal index and 0 on success.
*/
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
*   @brief Sorts a vector (see qsort).
*   @param compar Pointer to compare function (see qsort).
**/
void sortv(vector *vec, int (*compar)(const void *, const void *));

#endif
