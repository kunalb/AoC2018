#include "heap.h"

#include <stdlib.h>
#include <stdio.h>


void extract(void* val) {
  printf("%d", *((int*) val));
}

int val(void* val) {
  return *((int*) val);
}

int compare(void* a, void* b) {
  return val(a) - val(b);
}

int main(void) {
  printf("Starting tests...\n");
  int result = 0;

  struct heap pq;
  heap_init(&pq, compare, 4);
  int test[] = { 9, 8, 1, 2, 4, 100, 12000, 121, -1 };
  size_t len = sizeof(test) / sizeof(test[0]);

  for (int i = 0; i < len; i++) {
    heap_add(&pq, &test[i]);
    heap_print(&pq, extract);
    printf("\n");
  }

  printf("\n");
  for (int i = 0; i < len; i++) {
    printf("> %d\n", val(heap_pop(&pq)));
    heap_print(&pq, extract);
    printf("\n");
  }

  printf("\n");
  for (int i = 0; i < len; i++) {
    heap_add(&pq, &test[i]);
  }
  heap_print(&pq, extract);

  printf("\n");
  printf("%d at %d\n", test[3], heap_find(&pq, &test[3]));
  printf("%d at %d\n", test[7], heap_find(&pq, &test[7]));
  printf("%d at %d\n", test[0], heap_find(&pq, &test[0]));
  int random = 1;
  printf("Random at %d\n", heap_find(&pq, &random));

  for (int i = 0; i < len; i++) {
    printf("\nRemoving %d\n", test[i]);
    heap_remove(&pq, &test[i]);
    heap_print(&pq, extract);
  }

  return result;
}
