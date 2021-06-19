#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "sparseintarray.h"

int SLEEP = -1;
int WAKE = -2;

struct entry {
  int y;
  int m;
  int d;
  int hh;
  int mm;
  int guard;
};

int compare(const void * a, const void * b) {
  struct entry* entry_a = (struct entry*) a;
  struct entry* entry_b = (struct entry*) b;

  if (entry_a->y != entry_b->y)
    return entry_a->y - entry_b->y;

  if (entry_a->m != entry_b->m)
    return entry_a->m - entry_b->m;

  if (entry_a->d != entry_b->d)
    return entry_a->d - entry_b->d;

  if (entry_a->hh != entry_b->hh)
    return entry_a->hh - entry_b->hh;

  if (entry_a->mm != entry_b->mm)
    return entry_a->mm - entry_b->mm;

  return 0;
}

int main(int argc, char** argv) {
  int len = 2;
  struct entry* entries = malloc(len * sizeof(*entries));

  int n = 0, r;
  int y, m, d, hh, mm;
  char buffer[128];
  while (true) {
    r = scanf(" [%d-%d-%d %d:%d] %127[^\n]s",
              &entries[n].y,
              &entries[n].m,
              &entries[n].d,
              &entries[n].hh,
              &entries[n].mm,
              buffer);
    if (r != 6) {
      break;
    }

    if (strcmp(buffer, "falls asleep") == 0) {
      entries[n].guard = SLEEP;
    } else if (strcmp(buffer, "wakes up") == 0) {
      entries[n].guard = WAKE;
    } else if (1 == sscanf(buffer, "Guard #%d wakes up", &entries[n].guard)) {
    } else {
      printf("Unknown input %s", buffer);
      return -1;
    }

    n++;

    if (n == len) {
      entries = realloc(entries, 2 * len * sizeof(*entries));
      if (entries == NULL) {
        perror("Couldn't allocate memory");
      }
      len *= 2;
    }
  }

  qsort(entries, n, sizeof(struct entry), compare);

  struct sparse arr;
  sparse_init(&arr, 8);

  struct sparse twoarr;
  sparse_init(&twoarr, 512);

  int current_guard = 0;
  int start_sleep = 0;
  for (int i = 0; i < n; i++) {
    if (entries[i].guard != SLEEP &&
        entries[i].guard != WAKE) {
      current_guard = entries[i].guard;
    } else if (entries[i].guard == SLEEP) {
      start_sleep = entries[i].mm;
    } else {
      sparse_increment(&arr, current_guard, entries[i].mm - start_sleep);
      for (int j = start_sleep; j < entries[i].mm; j++) {
        sparse_increment(&twoarr, current_guard * 100 + j, 1);
      }
    }
  }

  int max_sleep = 0;
  int max_index = 0;
  for (int i = 0; i < arr.size; i++) {
    if (arr.values[i] > max_sleep) {
      max_index = i;
      max_sleep = arr.values[i];
    }
  }

  int max_guard = arr.keys[max_index];

  struct sparse mmarray;
  sparse_init(&mmarray, 60);

  bool count_minutes = false;
  int last_sleep = 0;
  for (int i = 0; i < n; i++) {
    if (entries[i].guard == max_guard) {
      count_minutes = true;
    } else if (entries[i].guard == SLEEP && count_minutes) {
      last_sleep = entries[i].mm;
    } else if (entries[i].guard == WAKE && count_minutes) {
      for (int j = last_sleep; j < entries[i].mm; j++) {
        sparse_increment(&mmarray, j, 1);
      }
    } else {
      count_minutes = false;
    }
  }

  int max_mm_index = 0;
  for (int i = 0; i < mmarray.size; i++) {
    if (mmarray.values[i] > mmarray.values[max_mm_index]) {
      max_mm_index = i;
    }
  }

  int max_mm = mmarray.keys[max_mm_index];
  printf("%d %d => %d\n", max_guard, max_mm, max_guard * max_mm);

  int current_max = 0;
  for (int i = 0; i < twoarr.size; i++) {
    if (twoarr.values[i] > twoarr.values[current_max]) {
      current_max = i;
    }
  }

  max_mm = twoarr.keys[current_max] % 100;
  max_guard = twoarr.keys[current_max] / 100;
  printf("%d %d => %d\n", max_guard, max_mm, max_guard * max_mm);

  free(entries);
  return 0;
}
