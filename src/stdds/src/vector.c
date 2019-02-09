#include "vector.h"

extern int new_vector(vector *vec, size_t size_element,
size_t initial_size, void (*delete_datatype)(void *data));
extern int append(vector *vec, const void *element);
extern void delete_vector(vector *vec);
extern int insert(vector *vec, const void *element, size_t index);
extern void *get(const vector *vec, size_t index);
extern int set(vector *vec, const void *element, size_t index);
extern int rem(vector *vec, size_t index);
extern void *get_last(const vector *vec);
extern size_t size(const vector *vec);
extern void *to_array(const vector *vec);

void sortv(vector *vec, int (*compar)(const void *, const void*))
{
  qsort(vec->data, vec->occupied, vec->size_element, compar);
}
