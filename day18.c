#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const int SIDE = 52;

void print_grid(char** grid, int max_x, int y) {
  for (int i = 1; i < y;  i++) {
    for (int j = 1; j < max_x; j++)
      printf("%c", grid[i][j]);
    printf("\n");
  }
}


int main(void) {
  char** grid1 = calloc(SIDE, sizeof(*grid1));
  char** grid2 = calloc(SIDE, sizeof(*grid1));

  for (int i = 0; i < SIDE; i++) {
    grid1[i] = calloc(SIDE, sizeof(**grid1));
    grid2[i] = calloc(SIDE, sizeof(**grid2));
  }

  char ch;

  int x = 1, y = 1;
  int max_x = 0;

  while(scanf("%c", &ch) == 1) {
    if (ch != '\n') {
      grid1[y][x++] = ch;
    } else {
      max_x = x > max_x ? x : max_x;
      y++; x = 1;
    }
  }

  int mins = 0;
  char** cur_grid = grid1;
  char** next_grid = grid2;
  char** tmp;

  print_grid(cur_grid, max_x, y);
  printf("\n");
  while (mins++ < 1000000000) {
    for (int i = 1; i < y; i++) {
      for (int j = 1; j < max_x; j++) {
        char next = cur_grid[i][j];

        int trees = 0;
        int lumberyards = 0;
        for (int k = -1; k <= 1; k++) {
          for (int l = -1; l <= 1; l++) {
            if (k == 0 && l == 0) continue;

            char ch = cur_grid[i + k][j + l];
            if (ch == '|') trees++;
            else if (ch == '#') lumberyards++;
          }
        }

        switch (cur_grid[i][j]) {
        case '.':
          if (trees >= 3)
            next = '|';
          break;
        case '|':
          if (lumberyards >= 3)
            next = '#';
          break;
        case '#':
          if (trees < 1 || lumberyards < 1)
            next = '.';
          break;
        }

        next_grid[i][j] = next;
      }
    }

    tmp = cur_grid;
    cur_grid = next_grid;
    next_grid = tmp;

    if (mins % 10000000 == 0) {
      printf("After %d minutes\n", mins);
      print_grid(cur_grid, max_x, y);
      printf("\n");

      int trees = 0, lumber = 0;
      for (int i = 1; i < y; i++) {
        for (int j = 1; j < max_x; j++) {
          if (cur_grid[i][j] == '|') trees++;
          else if (cur_grid[i][j] == '#') lumber++;
        }
      }

      printf("Total Resource: trees %d * lumberyards %d = %d\n\n",
             trees, lumber, trees * lumber);
    }
  }

  //int trees = 0, lumber = 0;
  //for (int i = 1; i < y; i++) {
  //  for (int j = 1; j < max_x; j++) {
  //    if (cur_grid[i][j] == '|') trees++;
  //    else if (cur_grid[i][j] == '#') lumber++;
  //  }
  //}

  //printf("After %d minutes; ", mins);
  //printf("Total Resource: trees %d * lumberyards %d = %d",
  //       trees, lumber, trees * lumber);

  return 0;
}
