#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

/** Brute force approach, not sure what's better */
/** Possible follow up, play around with k-d tree */
int main(int argc, char** argv) {

  int r, x, y;
  int* xs = malloc(50 * sizeof(*xs));
  int* ys = malloc(50 * sizeof(*ys));
  int p = 0;

  int* area = calloc(50, sizeof(*area));

  int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

  while ((r = scanf("%d, %d", &x, &y)) == 2) {
    xs[p] = x;
    ys[p] = y;

    minX = x < minX ? x : minX;
    minY = y < minY ? y : minY;
    maxX = x > maxX ? x : maxX;
    maxY = y > maxY ? y : maxY;

    p++;
  }
  printf("%d %d %d %d\n", minX, minY, maxX, maxY);

  for (int i = minX; i <= maxX; i++) {
    for (int j = minY; j <= maxY; j++) {
      int min_dist = INT_MAX; int min_index = -1;
      bool has_equal = false;

      for (int k = 0; k < p; k++) {
        int curr_dist = abs(xs[k] - i) + abs(ys[k] - j);
        if (min_dist > curr_dist) {
          min_dist = curr_dist;
          min_index = k;
          has_equal = false;
        } else if (min_dist == curr_dist) {
          has_equal = true;
        }
      }

      if (!has_equal) {
        area[min_index]++;
      }
    }
  }

  int max_area = -1;
  for (int i = 0; i < p; i++) {
    if (xs[i] == minX || xs[i] == maxX || ys[i] == minY || ys[i] == maxY) {
      continue;
    }

    if (max_area == -1 || area[i] > area[max_area]) {
      max_area = i;
    }
  }

  printf("%d %d\n", max_area, area[max_area]);

  int MAX_DISTANCE = 10000;
  int min_x2 = minX - (MAX_DISTANCE - (maxX - minX));
  int min_y2 = minY - (MAX_DISTANCE - (maxY - minY));
  int max_x2 = maxX + (MAX_DISTANCE - (maxX - minX));
  int max_y2 = maxY + (MAX_DISTANCE - (maxY - minY));
  printf("%d %d %d %d\n", min_x2, min_y2, max_x2, max_y2);


  int safe = 0;
  for (int i = min_x2; i <= max_x2; i++) {
    for (int j = min_y2; j <= max_y2; j++) {
      int total_dist = 0;
      for (int k = 0; k < p && total_dist < MAX_DISTANCE; k++) {
        total_dist += abs(xs[k] - i) + abs(ys[k] - j);
      }
      if (total_dist < MAX_DISTANCE) {
        safe++;
      }
    }
  }

  printf("%d\n", safe);

  free(xs);
  free(ys);
  free(area);
  return 0;
}
