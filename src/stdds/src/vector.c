#include "vector.h"

extern int new_vector(vector *vec, size_t size_element, size_t initial_size,
                      dsconf conf);
extern int push(vector *vec, void *element);
extern void *pop(vector *vec);
extern void delete_vector(vector *vec);
extern int insert(vector *vec, void *element, size_t index);
extern void *get(vector *vec, size_t index);
extern int set(vector *vec, void *element, size_t index);
extern int rem(vector *vec, size_t index);
extern void *get_last(vector *vec);
extern size_t size(const vector *vec);
extern void *to_array(const vector *vec);
extern void *vec_at(vector *vec, size_t index);
extern int merge(vector *restrict vec_1, vector *restrict vec_2);
extern int split(vector *vec, vector *res, size_t index);
extern bool is_empty(vector *vec);
extern bool index_valid(vector *vec, size_t index);
extern int clone(vector *const vec, vector *res);
extern void sortv(vector *vec, int (*compar)(const void *, const void *));
