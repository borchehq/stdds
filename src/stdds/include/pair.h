#ifndef PAIR_FDT_H
#define PAIR_FDT_H

#include "typedef.h"

typedef struct pair_s pair;

struct pair_s
{
  void *first;
  void *second;
};

void pair_new(pair *pair, void *first, void *second)
{
  pair->first = first;
  pair->second = second;
}

void pair_delete(pair *pair, dsconf *conf)
{
  if(conf != NULL && conf->delete_ds != NULL)
  {
    conf->delete_ds(pair);
  }
  return;
}

void *pair_first(pair *pair)
{
  return pair->first;
}

void *pair_second(pair *pair)
{
  return pair->second;
}

#endif