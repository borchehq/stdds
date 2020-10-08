#include "vector.h"

extern int vector_new(vector *vec, size_t size_element, size_t initial_capacity,
                      dsconf *conf);
extern int vector_push_back(vector *vec, void *element);
extern void *vector_pop_back(vector *vec);
extern void vector_delete(vector *vec);
extern int vector_insert(vector *vec, void *element, size_t index);
extern int vector_insert_range(vector *vec, void *restrict data, size_t index, size_t len);
extern int vector_insert_fill(vector *vec, void *restrict data, size_t index, size_t len);
extern void *vector_get(vector *vec, size_t index);
extern int vector_assign(vector *vec, void *element, size_t index);
extern int vector_assign_range(vector *vec, void *restrict data, size_t len);
extern int vector_assign_fill(vector *vec, void *restrict data, size_t len);
extern int vector_remove(vector *vec, size_t index);
extern void *vector_get_last(vector *vec);
extern size_t vector_size(const vector *vec);
extern int vector_reserve(vector *vec, size_t new_size);
extern size_t vector_capacity(vector *vec);
extern void *vector_to_array(const vector *vec);
extern void *vector_at(vector *vec, size_t index);
extern void *vector_front(vector *vec);
extern void *vector_back(vector *vec);
extern int vector_merge(vector *restrict vec_1, vector *restrict vec_2);
extern int vector_split(vector *vec, vector *res, size_t index);
extern bool vector_empty(vector *vec);
extern bool vector_index_valid(vector *vec, size_t index);
extern int vector_clone(vector *const vec, vector *res);
extern void vector_sort(vector *vec, int (*compar)(const void *, const void *));
extern int vector_resize(vector *vec, size_t size);
extern int vector_clear(vector *vec, bool reallocate);
extern int vector_erase(vector *vec, size_t index);
extern int vector_erase_range(vector *vec, size_t begin, size_t end);
extern void *vector_data(vector *vec);
