#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

static const int SIZE = 2000;
static const int PADDING = 3;

static const int WATER_X = 500;
static const int WATER_Y = 0;

static inline int min(a, b) {
  return a < b ? a : b;
}

static inline int max(a, b) {
  return a > b ? a : b;
}

static void print_grid(char** grid, int min_x, int max_x, int max_y) {
  for (int i = 0; i <= max_y + PADDING; i++) {
    for (int j = min_x - PADDING; j <= max_x + PADDING; j++) {
      printf("%c", grid[i][j] == 0 ? '.' : grid[i][j]);
    }
    printf("\n");
  }
}

bool emptyish(char c) {
  return c == 0 || c == '|';
}

int droplet(char** grid, int max_y, int x, int y, int px) {
  if (y > max_y)
    return max_y;

  if (emptyish(grid[y][x]) && grid[y][x] != '|')
    grid[y][x] = '|';

  if (emptyish(grid[y + 1][x]))
    return droplet(grid, max_y, x, y + 1, x);

  int dl = 0, dr = 0;

  if (px != x + 1 && emptyish(grid[y][x + 1]))
    dr = droplet(grid, max_y, x + 1, y, x);
  else if (px != x + 1)
    dr = y;

  if (px != x - 1 && emptyish(grid[y][x - 1]))
    dl = droplet(grid, max_y, x - 1, y, x);
  else if (px != x - 1)
    dl = y;

  if (dl == y && dr == y) {
    int i = x;
    while (grid[y][i] == '|')
      grid[y][i--] = '~';

    i = x + 1;
    while (grid[y][i] == '|')
      grid[y][i++] = '~';

    return y;
  }

  return max(dl, dr);
}

int main(void) {
  char** grid = calloc(SIZE, sizeof(char*));
  for (int i = 0; i < SIZE; i++)
    grid[i] = calloc(SIZE, sizeof(char));

  char a, b; int a0, b0, b1;
  int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
  while (scanf(" %c=%d, %c=%d..%d", &a, &a0, &b, &b0, &b1) == 5) {
    int x0, x1, y0, y1;
    if (a == 'x') {
      x0 = a0; x1 = a0; y0 = b0; y1 = b1;
    } else {
      x0 = b0; x1 = b1; y0 = a0; y1 = a0;
    }
    min_x = min(x0, min_x); min_y = min(y0, min_y);
    max_x = max(x1, max_x); max_y = max(y1, max_y);

    for (int i = y0; i <= y1; i++)
      for (int j = x0; j <= x1; j++)
        grid[i][j] = '#';
  }
  printf("Depth: %d, %d\n", min_y, max_y);

  grid[WATER_Y][WATER_X] = '+';

  int blocked = 0;
  int iterations = 100000;
  int water_cells = 0, last_water_cells = 0;
  int retained = 0;

  do {
    last_water_cells = water_cells;

    int x = WATER_X, y = WATER_Y;
    blocked = droplet(grid, max_y, x, y + 1, x);

    // printf("Reached level %d\n", blocked);

    water_cells = 0;
    retained = 0;
    int lim_x0 = max(0, min_x - 10);
    int lim_x1 = min(SIZE - 1, max_x + 10);
    for (int i = min_y; i <= max_y; i++)
      for (int j = lim_x0; j <= lim_x1; j++)
        if (grid[i][j] == '~' || grid[i][j] == '|') {
          water_cells++;
          if (grid[i][j] == '~') retained++;
        }
  } while (water_cells != last_water_cells);

  print_grid(grid, min_x, max_x, max_y);
  printf("%d water cells, %d retained.\n", water_cells, retained);

  return 0;
}
