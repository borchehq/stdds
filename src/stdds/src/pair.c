#include "pair.h"

extern void pair_new(pair *pair, void *first, void *second);
extern void pair_delete(pair *pair, dsconf *conf);
extern void *pair_first(pair *pair);
extern void *pair_second(pair *pair);