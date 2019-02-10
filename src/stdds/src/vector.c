#include "vector.h"

extern int new_vector(vector *vec, size_t size_element,
size_t initial_size, void (*delete_datatype)(void *data));
extern int push(vector *vec, void *element);
inline void *pop(vector *vec);
extern void delete_vector(vector *vec);
extern int insert(vector *vec, void *element, size_t index);
extern void *get(vector *vec, size_t index);
extern int set(vector *vec, void *element, size_t index);
extern int rem(vector *vec, size_t index);
extern void *get_last(vector *vec);
extern size_t size(const vector *vec);
extern void *to_array(const vector *vec);
void *at(vector *vec, size_t index);

void sortv(vector *vec, int (*compar)(const void *, const void *))
{
  qsort(vec->data, vec->occupied, vec->size_element, compar);
}
