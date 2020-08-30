#ifndef TYPEDEF_FDT_H
#define TYPEDEF_FDT_H

typedef unsigned char byte_t;
typedef struct type_conf_s dsconf;

struct type_conf_s
{
  int (*copy_ds)(void *data, void *res);
  void (*delete_ds)(void *data);
  int (*construct_ds)(void *data);
};
#endif