#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(void) {
  char input[] = { 7, 6, 5, 0, 7, 1 };
  int len = 6;

  char* scores = calloc(1000000000, sizeof(*scores));
  scores[0] = 3;
  scores[1] = 7;

  int elf1 = 0;
  int elf2 = 1;
  int i = 2;

  while (true) {
    /* printf("%d, %d\n", elf1, elf2); */
    /* for (int j = 0; j < i; j++) { */
    /*   printf("%c", '0' + scores[j]); */
    /* } */
    /* printf("\n"); */

    char total = scores[elf1] + scores[elf2];
    if (total >= 10) {
      scores[i++] = total / 10;
      scores[i++] = total % 10;
    } else {
      scores[i++] = total;
    }

    elf1 = (elf1 + 1 + scores[elf1]) % i;
    elf2 = (elf2 + 1 + scores[elf2]) % i;

    if (i >= len) {
      int j = 0;
      while (j < len && scores[i - len + j] == input[j])
        j++;
      if (j == len) {
        printf("%d Recipes before pattern\n", i - len);
        break;
      }
    }

    if (i >= len + 1) {
      int j = 0;
      while (j < len && scores[i - len + j - 1] == input[j])
        j++;
      if (j == len) {
        printf("%d Recipes before pattern\n", i - len - 1);
        break;
      }
    }
  }

  free(scores);
  return 0;
}
