#pragma once

#include <stdlib.h>

struct sparse {
  size_t capacity;
  size_t size;

  int* keys;
  int* values;
};

extern void sparse_init(struct sparse* arr, int capacity) {
  arr->capacity = capacity;
  arr->size = 0;
  arr->keys = malloc(sizeof(*(arr->keys)) * capacity);
  arr->values = malloc(sizeof(*(arr->values)) * capacity);
}

void sparse_increase_capacity(struct sparse* arr) {
  arr->capacity *= 2;
  arr->keys = realloc(arr->keys, 2 * sizeof(*(arr->keys)) * arr->capacity);
  arr->values = realloc(arr->values, 2 * sizeof(*(arr->values)) * arr->capacity);
}

extern void sparse_insert(struct sparse* arr, int key, int value) {
  for (int i = 0; i < arr->size; i++) {
    if (arr->keys[i] == key) {
      arr->values[i] = value;
      return;
    }
  }

  arr->size++;
  if (arr->size == arr->capacity) {
    sparse_increase_capacity(arr);
  }

  arr->keys[arr->size - 1] = key;
  arr->values[arr->size - 1] = value;
}


extern void sparse_increment(struct sparse* arr, int key, int value) {
  for (int i = 0; i < arr->size; i++) {
    if (arr->keys[i] == key) {
      arr->values[i] += value;
      return;
    }
  }

  sparse_insert(arr, key, value);
}

extern int sparse_get(struct sparse* arr, int key, int def) {
  for (int i = 0; i < arr->size; i++) {
    if (arr->keys[i] == key) {
      return arr->values[i];
    }
  }
  return def;
}

extern void sparse_print(struct sparse* arr) {
  for (int i = 0; i < arr->size; i++) {
    printf("%d => %d\n", arr->keys[i], arr->values[i]);
  }
}
