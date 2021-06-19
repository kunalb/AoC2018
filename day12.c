#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void) {
  char* state = calloc(256, sizeof(*state));
  char* next_state = calloc(256, sizeof(*state));

  if (scanf(" initial state: %256s", state) != 1) {
    exit(-1);
  }

  char results[35];
  char rules[35][10] = { { 0 } };
  int r = 0;

  while (scanf(" %s => %c", rules[r], &results[r]) == 2) {
    r++;
  }

  printf("%d rules\n", r);

  int start_index = 0; int prev_sum = 0;
  for (int i = 0; i <= 500; i++) {
    size_t len = strlen(state);
    int sum = 0;
    for (int c = 0; state[c] != '\0'; c++) {
      if (state[c] == '#')
        sum += start_index + c;
    }
    printf("%2d: %3d (%3d + %3d) | %3d %d %s\n", i, sum, prev_sum, sum - prev_sum, start_index, len, state);
    prev_sum = sum;

    int leading_dead = 0;
    while (state[leading_dead] != '#' && leading_dead <= 4)
      leading_dead++;

    int trailing_dead = 0;
    while (state[len - trailing_dead - 1] == '.' && trailing_dead <= 4)
      trailing_dead++;

    int move = 4 - leading_dead;
    memmove(state + move, state, len + 1);
    for (int c = 0; c < move; c++)
      state[c] = '.';

    for (int c = 0; c < 4 - trailing_dead; c++)
      state[c + len + move]  = '.';
    state[len + move + 4 - trailing_dead] = '\0';

    start_index -=  move;
    next_state[0] = '.';
    next_state[1] = '.';
    int c;
    for (c = 2; state[c] != '\0'; c++) {
      bool stays_alive = false;
      for (int j = 0; j < r && !stays_alive; j++) {
        if (results[j] == '.') {
          continue;
        }

        int compare = 0;
        for (int k = 0; rules[j][k] != '\0'; k++) {
          compare += state[c - 2 + k] != rules[j][k];
        }

        stays_alive = stays_alive || compare == 0;
      }

      next_state[c] = stays_alive ? '#' : '.';
    }
    next_state[c] = '\0';

    char * tmp = state;
    state = next_state;
    next_state = tmp;
    bzero(next_state, 256);
  }

  long long result = 36768 + (50000000000 - 500) * 69;
  printf("50000000000: %lld", result);

  free(state);
  free(next_state);
  return 0;
}
