#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int capacity;
  char* buffer = malloc(50010 * sizeof(*buffer));
  char c;

  int count = 0;
  int position = 0;

  while ((c = getc(stdin)) != '\n') {
    if (position == 0) {
      buffer[position++] = c;
    } else {
      int diff = c - buffer[position - 1];
      if (diff == 32 || diff == -32) {
        position--;
      } else {
        buffer[position++] = c;
      }
    }
    count++;
  }

  buffer[position]='\0';
  printf("%d\n", position);

  char* improved_buffer = malloc(position * sizeof(*buffer));
  int smallest_k = position;
  int smallest_char = 0;

  for (int i = 0; i < 26; i++) {
    int k = 0;
    char sp = 'A' + i;
    for (int j = 0; j < position; j++) {
      if (buffer[j] == sp || buffer[j] == sp + 32) {
        // continue
      } else if (k > 0 && abs(improved_buffer[k - 1] - buffer[j]) == 32) {
        k--;
      } else {
        improved_buffer[k++] = buffer[j];
      }
    }

    if (k < smallest_k) {
      smallest_k = k;
      smallest_char = i;
    }
  }

  free(buffer);
  free(improved_buffer);
  printf("%d %c\n", smallest_k, 'A' + smallest_char);
  return 0;
}
