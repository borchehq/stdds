#ifndef ARRAYLIST_H
#define ARRAYLIST_H

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

/**@brief creates a new list instance
*  @param delete_datatype optional helper function for deleting struct content,
*        will be ignored if NULL is passed
*  @param size_element size of the elements of the list
*  @param initial_size initial size of list
*  @return returns a new list on success and NULL on error
*/
inline int new_vector(vector *list, size_t size_element,
                      size_t initial_size, void (*delete_datatype)(void *data))
{
  //set standard alloc size
  if(initial_size == 0)
  {
    initial_size = INITIAL_CAP;
  }
  //allocate array and return NULL on error
  list->data = malloc(size_element * initial_size);
  if(list->data == NULL)
  {
    return -1;
  }
  /*set metadata*/
  list->allocated = initial_size;
  list->occupied = 0;
  list->size_element = size_element;
  list->delete_datatype = delete_datatype;
  return 0;
}

/**@brief destructs the list and frees associated memory space*/
inline void delete_vector(vector *list)
{
  if(list->delete_datatype != NULL)
  {
    for(size_t i = 0; i < list->occupied; i++)
    {
      list->delete_datatype(&list->data[list->size_element * i]);
    }
  }
  free(list->data);
}

/**@brief appends a new element to the end of the list
*  @return -1 if no memory available and 0 on success
*/
inline int append(vector *list, const void *element)
{
  byte_t *tmp = list->data;
  if(list->allocated == list->occupied)
  {
    //double the space
    tmp = realloc(tmp, list->allocated * list->size_element * 2);
    //check NULL
    if(tmp != NULL)
    {
      list->data = tmp;
      list->allocated *= 2;
    }
    else
    {
      return -1;
    }
  }
  memcpy(&list->data[list->occupied * list->size_element],
         element, list->size_element);
  list->occupied++;
  return 0;
}

/**@brief inserts the specified element at the specified position in this list
*        shifts the element currently at that position (if any) and any subsequent
*        elements to the right
*  @return returns -1 on failure and 0 on success
*/
inline int insert(vector *list, const void *element, size_t index)
{
  byte_t *tmp = list->data;
  if(list->allocated == list->occupied)
  {
    /*double the space*/
    tmp = realloc(tmp, list->allocated * list->size_element * 2);
    /*check NULL*/
    if(tmp != NULL)
    {
      list->data = tmp;
      list->allocated *= 2;
    }
    else
    {
      return -1;
    }
  }
  /*just append*/
  if(index >= list->occupied)
  {
    memcpy(&list->data[list->occupied * list->size_element],
           element, list->size_element);
  }
  /*move all elements beginning from index to the right*/
  else{
    memmove(&list->data[(index + 1) * list->size_element],
            &list->data[index * list->size_element],
            (list->occupied - index) * list->size_element);
    memmove(&list->data[index * list->size_element], element,
            list->size_element);
  }
  list->occupied++;
  return 0;
}

/**@brief retrieves the element at the specified index
*  @return the corresponding element on success and NULL on error
*/
inline void *get(const vector *list, size_t index)
{
  if(index >= list->occupied)
  {
    return NULL;
  }
  return &list->data[list->size_element * index];
}

/**@brief sets a new element at the specified index
*  @return -1 if illegal index and 0 on success
*/
inline int set(vector *list, const void *element, size_t index)
{
  if(index >= list->occupied)
  {
    return -1;
  }
  memmove(&list->data[index * list->size_element], element,
          list->size_element);
  return 0;
}

/**@brief removes the element at the specified index and if applicable
         calls delete_datatype()
* @return -1 if no memory, -2 if illegal index and 0 on success
*/
inline int rem(vector *list, size_t index)
{
  size_t offset;
  void *tmp = list->data;
  /*check whether index is legal*/
  if(index >= list->occupied)
  {
    return -2;
  }
  offset = list->occupied - 1 - index;
  if(list->delete_datatype != NULL)
  {
    list->delete_datatype(get(list, index));
  }
  /*close gap if not the last element*/
  if(index < list->occupied - 1)
  {
    memmove(&list->data[index * list->size_element],
            &list->data[(index + 1) * list->size_element],
            offset * list->size_element);
  }
  list->occupied--;
  if(list->allocated >= 2 * MIN_CAP && list->allocated >= 2 * list->occupied)
  {
    tmp = realloc(tmp, (list->allocated / 2) * list->size_element);
    /*check NULL*/
    if(tmp != NULL){
      list->data = tmp;
      list->allocated /= 2;
    }
    else
    {
      return -1;
    }
  }
  return 0;
}

/**@brief returns the last element in the list*/
inline void *get_last(const vector *list)
{
    return &list->data[(list->occupied - 1) * list->size_element];
}

/**@brief returns the number of elements in the list*/
inline size_t size(const vector *list)
{
  return list->occupied;
}

/**@brief returns an array with the content of the specified vector type
*  @return returns an array with the original content on success and NULL on error
*/
inline void *to_array(const vector *list)
{
  void *array = malloc(list->occupied * list->size_element);
  if(array == NULL){
    return NULL;
  }
  memcpy(array, list->data, list->occupied * list->size_element);
  return array;
}

/**@brief sorts a list
   @param compar pointer to compare function (see qsort)
*/
void sortv(vector *list, int (*compar)(const void *, const void*));

#endif
