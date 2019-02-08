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
  size_t num_appends = 100000000;
  printf("[INFO] testing append()\n");
  for(size_t i = 0; i < num_appends; i++){
    append(&v, &i);
  }
  if(num_appends == size(&v)){
    printf("[OK] test successful\n");
  }
  else{
      printf("[ERROR] test failed\n");
      printf("[ERROR] %li != %li\n", num_appends, size(&v));
  }
  printf("[INFO] testing get()\n");
  bool error = false;
  for(size_t i = 0; i < num_appends; i++){
    if(i != *(size_t*)get(&v, i)){
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
  printf("[INFO] testing rem()\n");
  error = false;
  size_t index_remove = num_appends - 1000;
  size_t successor = *(size_t*)get(&v, index_remove + 1);
  for(size_t i = index_remove; i < size(&v); i++){
    rem(&v, index_remove);
    if(*(size_t*)get(&v, index_remove) != successor){
      error = true;
      break;
    }
    successor = *(size_t*)get(&v, index_remove + 1);
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
    if(*(size_t*)get(&v, i) != val){
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
    if(*(size_t*)get(&v, i) != array[i]) {
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
    immediate[i] = *(size_t*)get(&v, index + i);
  }
  printf("Size before: %li\n", size(&v));
  insert(&v, &element, index);
  printf("Size afterwards: %li\n", size(&v));
  if(*(size_t*)get(&v, index) != element){
      error = true;
  }
  for(size_t i = 0; i < size(&v) - 1 - index; i++){
    if(immediate[i] != *(size_t*)get(&v, index + i + 1)){
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
  for(size_t i = 20; i > 0; i--){
    append(&v, &i);
  }
  printf("[ ");
  for(size_t i = 0; i < 20; i++){
    printf("%li ", *(size_t*)get(&v, i));
  }
  printf("]\n");
  sortv(&v, compare);
  printf("[ ");
  for(size_t i = 0; i < 20; i++){
    printf("%li ", *(size_t*)get(&v, i));
  }
  printf("]\n");
  return 0;
}
