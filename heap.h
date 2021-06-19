#pragma once

#include <stdlib.h>

struct heap {
  void** store;
  size_t size;
  size_t index;
  int (*cmp)(void* a, void* b);
};

void heap_init(struct heap* h, int (*cmp)(void* a, void* b), size_t initial_size);
void heap_free(struct heap* h);
void heap_add(struct heap* h, void* value);
int heap_find(struct heap* h, void* value);
void heap_remove(struct heap* h, void* value);
void* heap_pop(struct heap* h);
void heap_print(struct heap* h, void (*prn)(void* val));
size_t heap_size(struct heap* heap);
