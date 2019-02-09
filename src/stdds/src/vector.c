#include "vector.h"

extern int new_vector(vector *list, size_t size_element,
size_t initial_size, void (*delete_datatype)(void *data));
extern int append(vector *list, const void *element);
extern void delete_vector(vector *list);
extern int insert(vector *list, const void *element, size_t index);
extern void *get(const vector *list, size_t index);
extern int set(vector *list, const void *element, size_t index);
extern int rem(vector *list, size_t index);
extern void *get_last(const vector *list);
extern size_t size(const vector *list);
extern void *to_array(const vector *list);

void sortv(vector *list, int (*compar)(const void *, const void*))
{
  qsort(list->data, list->occupied, list->size_element, compar);
}
