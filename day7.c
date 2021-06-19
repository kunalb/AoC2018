#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void debug(bool graph[26][26], bool enabled[26]);

int main(int argc, char** argv) {

  int s; char c1, c2;
  bool graph[26][26];
  for (int i = 0; i < 26; i++)
    for (int j = 0; j < 26; j++)
      graph[i][j] = false;

  bool enabled[26];
  bool visited[26];
  int queue[26];

  int qstart = 0, qend = 0;

  while ((s = scanf("Step %c must be finished before step %c can begin.\n", &c1, &c2)) == 2) {
    graph[c1 - 'A'][c2 - 'A'] = true;
    enabled[c1 - 'A'] = true;
    enabled[c2 - 'A'] = true;
  }

  debug(graph, enabled);

  int total = 0;
  for (int i = 0; i < 26; i++)
    total += enabled[i];

  whoa: for (int i = 0; i < 26; i++) {
    if (!enabled[i]) {
      continue;
    }

    int deps = 0;
    for (int j = 0; j < 26; j++) {
      deps += graph[j][i] ? 1 : 0;
    }

    if (deps == 0) {
      queue[qend++] = i;
      for (int j = 0; j < 26; j++) {
        graph[i][j] = false;
      }
      enabled[i] = false;

      printf("\n");
      debug(graph, enabled);
      goto whoa;
    }
  }

  printf("\n");
  for (int i = 0; i < qend; i++) {
    printf("%c", 'A' + queue[i]);
  }

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
