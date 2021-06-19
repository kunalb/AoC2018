#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct bike {
  int x;
  int y;
  char dir;
  char turn;
  char dead;
  char waste;
};

void debug(char the_grid[300][300], struct bike*, int, int, int);
char turn_left(char dir);
char turn_right(char dir);
int bike_comparison(const void* bike1, const void* bike2);

int main(void) {
  int ch;
  char the_grid[300][300] = { { 0 } };
  struct bike bikes[20];

  int y = 0, x = 0, b = 0;
  int max_x = 0;
  int max_y = 0;

  while ((ch = getchar()) != EOF) {
    if (ch == '\n') {
      y++;
      if (x > max_x)
        max_x = x;
      x = 0;
    } else if (ch == '>' || ch == '<') {
      bikes[b].dir = (char) ch;
      bikes[b].x = x;
      bikes[b].y = y;
      b++;

      ch = '-';
      the_grid[y][x++] = (char) ch;
    } else if (ch == 'v' || ch == '^') {
      bikes[b].dir = (char) ch;
      bikes[b].x = x;
      bikes[b].y = y;
      b++;

      ch = '|';
      the_grid[y][x++] = (char) ch;
    } else {
      the_grid[y][x++] = (char) ch;
    }
  }
  max_y = y;

  int ticks = 0;
  int live_bikes = b;

  while (true) {
    // printf("Tick %d:\n", ticks);
    // debug(the_grid, bikes, max_x, max_y, b);
    // printf("\n");
    for (int i = 0; i < b; i++) {
      if (bikes[i].dead == 1) continue;

      switch (bikes[i].dir) {
      case '>':
        bikes[i].x += 1;
        break;
      case '<':
        bikes[i].x -= 1;
        break;
      case 'v':
        bikes[i].y += 1;
        break;
      case '^':
        bikes[i].y -= 1;
        break;
      }

      for (int k = 0; k < b; k++) {
        if (i == k) continue;
        if (bikes[k].dead == 1) continue;
        if (bikes[i].x == bikes[k].x &&
            bikes[i].y == bikes[k].y) {
          printf("Tick %d:\n", ticks);
          printf("Crash at: [%d,%d]\n", bikes[k].x, bikes[k].y);
          live_bikes -= 2;
          printf("%d bikes left.\n", live_bikes);
          // bikes[k].dir = 'X';
          // debug(the_grid, bikes, max_x, max_y, b);
          // goto done;
          bikes[i].dead = 1;
          bikes[k].dead = 1;
        }
      }

      char new_road = the_grid[bikes[i].y][bikes[i].x];
      if (new_road == '/') {
        switch (bikes[i].dir) {
        case '>':
          bikes[i].dir = '^';
          break;
        case '<':
          bikes[i].dir = 'v';
          break;
        case 'v':
          bikes[i].dir = '<';
          break;
        case '^':
          bikes[i].dir = '>';
          break;
        }
      } else if (new_road == '\\') {
        switch (bikes[i].dir) {
        case '>':
          bikes[i].dir = 'v';
          break;
        case '<':
          bikes[i].dir = '^';
          break;
        case 'v':
          bikes[i].dir = '>';
          break;
        case '^':
          bikes[i].dir = '<';
          break;
        }
      } else if (new_road == '+') {
        if (bikes[i].turn == 0) {
          bikes[i].dir = turn_left(bikes[i].dir);
        } else if (bikes[i].turn == 2) {
          bikes[i].dir = turn_right(bikes[i].dir);
        }
        bikes[i].turn = (1 + bikes[i].turn) % 3;
      }
    }

    ticks++;

    qsort(bikes, b, sizeof(*bikes), bike_comparison);

    if (live_bikes == 1) {
      for (int k = 1; k < b; k++) {
        if (bikes[k].dead == 0) {
          printf("Left: %d,%d", bikes[k].x, bikes[k].y);
        }
      }
      goto done;
    }

    if (ticks > 100000) break;
  }


 done:
  return 0;
}

int bike_comparison(const void* a, const void* b) {
  const struct bike* b1 = (const struct bike*) a;
  const struct bike* b2 = (const struct bike*) b;
  if (b1->y != b2->y)
    return b1->y - b2->y;

  return b1->x - b2->x;
}

char turn_left(char dir) {
  switch (dir) {
  case '>': return '^';
  case '^': return '<';
  case '<': return 'v';
  case 'v': return '>';
  }
  return '\0';
}

char turn_right(char dir) {
  switch (dir) {
  case '^': return '>';
  case '<': return '^';
  case 'v': return '<';
  case '>': return 'v';
  }
  return '\0';
}

void debug(char the_grid[300][300], struct bike* bikes, int max_x, int max_y, int b) {
  for (int i = 0; i < max_y; i++) {
    for (int j = 0; j < max_x; j++) {
      if (the_grid[i][j] == '\0') break;

      if (b > 0 && bikes->x == j && bikes->y == i) {
        printf("%c", bikes->dir);
        b--;
        bikes++;
      } else {
        printf("%c", the_grid[i][j]);
      }
    }

    printf("\n");
  }
}
