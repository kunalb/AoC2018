#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

void debug(bool graph[26][26], bool enabled[26]);

int main(int argc, char** argv) {

  int s; char c1, c2;
  bool graph[26][26] = { { false } };
  bool enabled[26] = { false };
  int finish_at[26];
  for (int i = 0; i < 26; i++)
    finish_at[i] = INT_MAX;


  while ((s = scanf("Step %c must be finished before step %c can begin.\n", &c1, &c2)) == 2) {
    graph[c1 - 'A'][c2 - 'A'] = true;
    enabled[c1 - 'A'] = true;
    enabled[c2 - 'A'] = true;
  }

  debug(graph, enabled);
  printf("\n");

  int JOB_TIME = 60;
  int MAX_WORKERS = 5;
  int tick = 0;
  int workers = MAX_WORKERS;

  int remaining = 0;
  for (int i = 0; i < 26; i++)
    remaining += enabled[i];

  while (remaining > 0) {
    for (int i = 0; i < 26; i++) {
      if (finish_at[i] == tick) {
        remaining--;
        workers++;
        for (int j = 0; j < 26; j++) {
          graph[i][j] = false;
        }

        printf("Tick %d\n", tick);
        debug(graph, enabled);
        printf("\n");
      }
    }

    for (int i = 0; workers > 0 && i < 26; i++) {
      if (!enabled[i])
        continue;
      int deps = 0;
      for (int j = 0; j < 26; j++) {
        deps += graph[j][i];
      }

      if (deps == 0 && workers > 0) {
        finish_at[i] = tick + JOB_TIME + i + 1;
        workers--;
        enabled[i] = false;
      }
    }

    tick++;
  }

  for (int i = 0; i < 26; i++) {
    if (finish_at[i] != INT_MAX) {
      printf("%c %d\n", 'A' + i, finish_at[i]);
    }
  }

  printf("\nFinished at %d", tick - 1);
  return 0;
}


void debug(bool graph[26][26], bool enabled[26]) {
  printf("0 ");
  for (int i = 0; i < 26; i++) {
    if (enabled[i]) {
      printf("%c ", i + 'A');
    }
  }

  for (int i = 0; i < 26; i++) {
    if (!enabled[i]) continue;

    printf("\n%c ", i + 'A');

    for (int j = 0; j < 26; j++) {
      if (!enabled[j]) continue;

      printf("%d ", graph[i][j] ? 1 : 0);
    }
  }

  printf("\n");
}
