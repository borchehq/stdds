#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vector.h"

typedef struct Test{
  double *data;
  size_t size;
}Test;

void delete_datatype(void *data){
  Test test = *(Test*)data;
  free(test.data);
}

int compare(const void *a, const void *b){
    if(*(size_t*)a > *(size_t*)b){
      return 1;
    }
    if(*(size_t*)a < *(size_t*)b){
      return -1;
    }
    return -1;
}

int main(int argc, char const *argv[]) {
  vector v; new_vector(&v, sizeof(size_t), 1, NULL);
  size_t num_pushs = 100000000;
  printf("[INFO] testing push()\n");
  for(size_t i = 0; i < num_pushs; i++){
    push(&v, &i);
  }
  if(num_pushs == size(&v)){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
      printf("[ERROR] %li != %li\n", num_pushs, size(&v));
  }
  printf("[INFO] testing vec_at()\n");
  bool error = false;
  for(size_t i = 0; i < num_pushs; i++){
    if(i != *(size_t*)vec_at(&v, i)){
      error = true;
      break;
    }
  }
  if(error == false){
    printf("[OK] test successful\n");
  }
  printf("[INFO] testing get()\n");
  error = false;
  for(size_t i = 0; i < num_pushs; i++){
    void *res = get(&v, i);
    if(i != *(size_t*)res){
      error = true;
      break;
    }
    free(res);
  }

  if(error == false){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
  }
  printf("[INFO] testing rem()\n");
  error = false;
  size_t index_remove = num_pushs - 1000;
  size_t successor = *(size_t*)vec_at(&v, index_remove + 1);
  for(size_t i = index_remove; i < size(&v); i++){
    rem(&v, index_remove);
    if(*(size_t*)vec_at(&v, index_remove) != successor){
      error = true;
      break;
    }
    successor = *(size_t*)vec_at(&v, index_remove + 1);
  }
  if(error == false){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
  }
  printf("[INFO] testing set()\n");
  error = false;
  size_t val = 7819;
  for(size_t i = 0; i < 10000; i++){
    set(&v, &val, i);
    if(*(size_t*)vec_at(&v, i) != val){
      error = true;
      break;
    }
  }
  if(error == false){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
  }
  printf("[INFO] testing to_array()\n");
  error = false;
  size_t *array = (size_t*)to_array(&v);
  for(size_t i = 0; i < size(&v); i++){
    if(*(size_t*)vec_at(&v, i) != array[i]) {
      error = true;
      break;
    }
  }
  if(error == false){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
  }
  free(array);
  printf("[INFO] testing insert()\n");
  size_t element = 9102901;
  error = false;
  size_t index = size(&v) - 12552;
  size_t *immediate = malloc(sizeof(size_t) * (size(&v) - index));
  for(size_t i = 0; i < size(&v) - index; i++){
    immediate[i] = *(size_t*)vec_at(&v, index + i);
  }
  printf("Size before: %li\n", size(&v));
  insert(&v, &element, index);
  printf("Size afterwards: %li\n", size(&v));
  if(*(size_t*)vec_at(&v, index) != element){
      error = true;
  }
  for(size_t i = 0; i < size(&v) - 1 - index; i++){
    if(immediate[i] != *(size_t*)vec_at(&v, index + i + 1)){
      error = true;
      break;
    }
  }
  if(error == false){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
  }
  free(immediate);
  delete_vector(&v);

  printf("[INFO] testing sortv()\n");
  new_vector(&v, sizeof(size_t), 1, NULL);
  vector v2;
  new_vector(&v2, sizeof(size_t), 1, NULL);
  for(size_t i = 20; i > 0; i--){
    push(&v, &i);
    push(&v2, &i);
  }
  printf("[ ");
  for(size_t i = 0; i < 20; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  sortv(&v, compare);
  printf("[ ");
  for(size_t i = 0; i < 20; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");

  merge(&v, &v2);
  printf("%lu\n", v.occupied);
  printf("[");
  for(size_t i = 0; i < 40; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");

  delete_vector(&v2);
  split(&v, &v2, 2);
  printf("[");
  for(size_t i = 0; i < v.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  printf("[");
  for(size_t i = 0; i < v2.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v2, i));
  }
  printf("]\n");

  index_valid(&v, 1) == 0 ? printf("false\n") :  printf("true\n");
  index_valid(&v, 2) == 0 ? printf("false\n") :  printf("true\n");
  index_valid(&v2, 37) == 0 ? printf("false\n") :  printf("true\n");
  index_valid(&v2, 38) == 0 ? printf("false\n") :  printf("true\n");
  bool empty = is_empty(&v);
  empty == 0 ? printf("false\n") :  printf("true\n");
  delete_vector(&v);
  new_vector(&v, sizeof(size_t), 4, NULL);
  empty = is_empty(&v);
  empty == 0 ? printf("false\n") :  printf("true\n");
  for(size_t i = 20; i > 0; i--){
    push(&v, &i);
  }
  printf("[");
  for(size_t i = 0; i < v.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v, i));
  }
  printf("]\n");
  delete_vector(&v2);
  clone(&v, &v2);
  printf("[");
  for(size_t i = 0; i < v2.occupied; i++){
    printf("%li ", *(size_t*)vec_at(&v2, i));
  }
  printf("]\n");
  return 0;
}
