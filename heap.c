#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

void* heap_remove_impl(struct heap* h, int index);

void swap(struct heap* h, int a, int b) {
  void* tmp = h->store[a];
  h->store[a] = h->store[b];
  h->store[b] = tmp;
}

void heap_init(struct heap* h, int (*cmp)(void* a, void* b), size_t initial_size) {
  h->store = calloc(initial_size, sizeof(*(h->store)));
  h->cmp = cmp;
  h->size = initial_size;
  h->index = 0;
}

void heap_add(struct heap* h, void* value) {
  if (h->index == h->size - 1) {
    if ((h->store = realloc(h->store, h->size * 2 * sizeof(*(h->store)))) == 0) {
      perror("heap realloc");
      exit(-1);
    }
    h->size += h->size;
  }

  int iter = h->index;
  h->store[h->index++] = value;

  while (iter > 0) {
    int parent = (iter - 1) >> 1;
    if (h->cmp(h->store[iter], h->store[parent]) < 0) {
      swap(h, iter, parent);
    }
    iter >>= 1;
  }
}

int heap_find(struct heap* h, void* node) {
  for (int i = 0; i < h->index; i++) {
    if (h->store[i] == node) {
      return i;
    }
  }

  return -1;
}

void* heap_pop(struct heap* h) {
  return heap_remove_impl(h, 0);
}

void heap_remove(struct heap* h, void* value) {
  heap_remove_impl(h, heap_find(h, value));
}

void* heap_remove_impl(struct heap* h, int point) {
  if (point < 0 || point >= h->index)
    return NULL;

  void* ret = h->store[point];

  if (h->index - 1 != point) {
    h->store[point] = h->store[h->index - 1];

    int iter = 0;
    while (iter < h->index - 1) {
      int a = iter * 2 + 1;
      int b = iter * 2 + 2;

      if (b < h->index - 1 &&
          h->cmp(h->store[a], h->store[b]) > 0) {
        a = b;
      }

      if (a < h->index - 1 &&
          h->cmp(h->store[iter], h->store[a]) > 0) {
        swap(h, iter, a);
        iter = a;
      } else {
        break;
      }
    }
  }

  h->index--;
  return ret;
}

void heap_print(struct heap* h, void (*prn)(void* val)) {
  int level_end = 0;
  int current_level = 0;
  for (int i = 0; i < h->index; i++) {
    printf("[%d] ", i);
    prn(h->store[i]);

    if (i == level_end) {
      printf("\n");
      level_end = level_end + (2 << current_level);
      current_level++;
    } else printf(" ");
  }
}

void heap_free(struct heap* h) {
  free(h->store);
}

size_t heap_size(struct heap* h) {
  return h->index;
}
