#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static char dirs[4] = { 1, 2, 4, 8 };
static int ioffset[4] = { -1, 0, 1, 0 };
static int joffset[4] = { 0, 1, 0, -1 };

static size_t N = 0;
static size_t E = 1;
static size_t S = 2;
static size_t W = 3;

struct area {
  char* maze;
  int side;

  int ir[2];
  int jr[2];

  int i;
  int j;
};

void init(struct area* area, int initial_side) {
  area->side = initial_side;
  area->maze = calloc(area->side * area->side, sizeof(char));

  area->i = initial_side / 2;
  area->j = initial_side / 2;

  area->ir[0] = area->i;
  area->ir[1] = area->i;

  area->jr[0] = area->j;
  area->jr[1] = area->j;
}

//   0
// 3   1
//   2
bool checkdir(struct area* area, int i, int j, int dir) {
  return area->maze[i * area->side + j] & dirs[dir];
}

void printdir(struct area* area, int i, int j, int dir) {
  if (checkdir(area, i, j, dir)) {
    if (dir & 1) printf("|");
    else printf("-");
  } else {
    printf("#");
  }
}

void print(struct area* area) {
  printf("#");
  for (int j = area->jr[0]; j <= area->jr[1]; j++) {
    printdir(area, 0, j, 0);
    printf("#");
  }
  printf("\n");

  for (int i = area->ir[0]; i <= area->ir[1]; i++) {
    printdir(area, i, 0, 3);
    for (int j = area->jr[0]; j <= area->jr[1]; j++) {
      if (i == area->i && j == area->j)
        printf("X");
      else
        printf(".");
      printdir(area, i, j, 1);
    }
    printf("\n");

    printf("#");
    for (int j = area->jr[0]; j <= area->jr[1]; j++) {
      printdir(area, i, j, 2);
      printf("#");
    }
    printf("\n");
  }
}

void addroom(struct area* a, int k, int l, size_t dir) {
  int i1 = k + a->i;
  int j1 = l + a->j;

  a->maze[i1 * a->side + j1] |= dirs[dir];

  int i2 = i1 + ioffset[dir];
  int j2 = j1 + joffset[dir];

  if (i2 >= a->side || j2 >= a->side) {
    char* newmaze = calloc(4 * a->side * a->side, sizeof(char));
    int newside = a->side * 2;
    int newi = newside / 2, newj = newside / 2;
    for (int i = a->maze->ir[0]; i <= a->maze->ir[1]; i++) {
      int iprime = (i - a->maze->i) + newi;
      memcpy(newmaze + (iprime * newside + a->side), a->maze[i * maze->side], maze->side);
    }

    // memcpy(newmaze + ..., a->maze, a->side * a->side * sizeof(char));

    // a->side = 2 * a->side;
    // a->maze = realloc(a->maze, a->side * a->side * sizeof(char));
    // Resize the grid!
  }

  a->maze[i2 * a->side + j2] |= dirs[ (dir + 2) % 4 ];
  if (i2 < a->ir[0]) a->ir[0] = i2;
  else if (i2 > a->ir[1]) a->ir[1] = i2;

  if (j2 < a->jr[0]) a->jr[0] = j2;
  else if (j2 > a->jr[1]) a->jr[1] = j2;
}

int main(void) {
  struct area A;

  init(&A, 10);
  addroom(&A, 0, 0, N);
  addroom(&A, 0, 0, S);
  addroom(&A, 0, 0, E);
  addroom(&A, 0, 0, W);

  print(&A);

  // char regex[10000];
  // scanf("%s", regex);

  return 0;
}
