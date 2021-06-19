#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>


int main(void) {
  int ps[500][2] = {{0}}, vs[500][2] = {{0}};
  int p = 0;

  int min_x = 0, min_y = 0, max_x = 0, max_y = 0;

  while(scanf(" position=< %d,  %d> velocity=< %d, %d>",
              &ps[p][0], &ps[p][1], &vs[p][0], &vs[p][1]) == 4) {
    min_x = min_x > ps[p][0] ? ps[p][0] : min_x;
    min_y = min_y > ps[p][1] ? ps[p][1] : min_y;
    max_x = max_x < ps[p][0] ? ps[p][0] : max_x;
    max_y = max_y < ps[p][1] ? ps[p][1] : max_y;

    p++;
  }
  printf("%d %d %d %d\n\n", min_x, min_y, max_x, max_y);

  bool started = false;
  long area, prev_area = -1;
  long ticks = 0;
  while (true) {
    long cols = max_x - min_x + 1;
    long rows = max_y - min_y + 1;

    area = cols * rows;
    if (prev_area != -1 && area > prev_area) {
      break;
    }

    if (cols < 180 && rows < 180) {
      bool* grid = calloc((size_t) 100000, sizeof *grid);
      for (int k = 0; k < p; k++) {
        grid[(ps[k][0] - min_x) + (ps[k][1] - min_y) * cols] = true;
      }

      for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
          printf(grid[c + cols * r] ? "x" : ".");
        }
        printf("\n");
      }

      printf("- at %d\n\n", ticks);
      free(grid);
    }

    min_x = INT_MAX; min_y = INT_MAX; max_x = INT_MIN; max_y = INT_MIN;
    for (int k = 0; k < p; k++) {
      ps[k][0] += vs[k][0];
      ps[k][1] += vs[k][1];

      min_x = min_x > ps[k][0] ? ps[k][0] : min_x;
      min_y = min_y > ps[k][1] ? ps[k][1] : min_y;
      max_x = max_x < ps[k][0] ? ps[k][0] : max_x;
      max_y = max_y < ps[k][1] ? ps[k][1] : max_y;
    }

    prev_area = area;
    ticks++;
  }
}
