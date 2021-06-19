#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int get_sum() {
  int children, metadata, sum = 0;
  scanf(" %d %d", &children, &metadata);

  while (children-- > 0) {
    sum += get_sum();
  }

  int value;
  while (metadata-- > 0) {
    scanf(" %d", &value);
    sum += value;
  }

  return sum;
}

int get_value() {
  int children, metadata, sum = 0;
  scanf(" %d %d", &children, &metadata);

  int* children_values = malloc(sizeof(*children_values) * children);
  int i = 0;
  while (i < children) {
    children_values[i++] = get_value();
  }

  int value;
  while (metadata-- > 0) {
    scanf(" %d", &value);
    if (children == 0) {
      sum += value;
    } else if (value <= children) {
      sum += children_values[value - 1];
    }
  }

  free(children_values);
  return sum;
}

int main(int argc, char* argv[]) {
  // printf("%d\n", get_sum());
  printf("%d\n", get_value());
  return 0;
}
