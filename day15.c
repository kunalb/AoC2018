#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "heap.h"

#define MAX 500

struct path {
  int cost;
  int visited;
};

struct cell {
  struct cell* n;
  struct cell* p;

  int x;
  int y;

  int hp;
  char t;

  int debug;
};

struct pair {
  void* a;
  void* b;
};

void print_army(struct cell* iter);
void print_grid(struct cell** grid, int x, int y);
void pathfind(int ax, int ay, int bx, int by, struct cell** grid, int x, int y, int* output);
void reset_debug(struct cell** grid, int x, int y);

int compare3(int az, int ay, int ax, int bz, int by, int bx) {
  if (az != bz) return az - bz;
  if (ay != by) return ay - by;
  return ax - bx;
}

const static int target_xs[] = { 0, -1, 1, 0 };
const static int target_ys[] = { -1, 0, 0, 1 };

bool debug = false;

int outcome(struct cell** grid, int x, int y, int elf_power) {
  // reset_debug(grid, x, y);
  // print_grid(grid, x, y);

  struct cell* head = NULL;
  struct cell* tail = NULL;

  int round = 0;
  while (round <= 1000) {
    // Re-sort list into reading order
    head = NULL;
    tail = NULL;
    for (int i = 0; i < y; i++) {
      for (int j = 0; j < x; j++) {
        if (grid[i][j].t == 'G' || grid[i][j].t == 'E') {
          if (head == NULL) {
            head = &grid[i][j];
            tail = head;
          } else {
            tail->n = &grid[i][j];
            grid[i][j].p = tail;
            tail = &grid[i][j];
          }
        }
      }
    }

    head->p = NULL;
    tail->n = NULL;
    struct cell* iter = head;
    while (iter != NULL) {
      bool in_range = false;
      int pt_x, pt_y, pt_hp = INT_MAX;
      for (int i = 0; i < 4; i++) {
        int itx = iter->x + target_xs[i];
        int ity = iter->y + target_ys[i];
        if (grid[ity][itx].hp != 0 &&
            grid[ity][itx].t != iter->t &&
            compare3(grid[ity][itx].hp, ity, itx,
                     pt_hp, pt_y, pt_x) < 0) {
          in_range = true;
          pt_y = ity;
          pt_x = itx;
          pt_hp = grid[pt_y][pt_x].hp;
        }
      }

      if (!in_range) {
        struct cell* target = head;
        int ft_x = INT_MAX, ft_y = INT_MAX, ft_cost = INT_MAX;
        int ft_sx = INT_MAX, ft_sy = INT_MAX;
        struct cell* final_target = NULL;
        bool any_valid = false;
        while (target != NULL) {
          if (target->hp != 0 && target->t != iter->t) {
            any_valid = true;

            if (debug) printf("Evaluating target %d, %d.\n", target->x, target->y);

            for (int i = 0; i < 4; i++) {
              int target_x = target->x + target_xs[i];
              int target_y = target->y + target_ys[i];

              if (grid[target_y][target_x].t == '.') {
                int output[3] = { 0 };
                pathfind(iter->x, iter->y, target_x, target_y, grid, x, y, output);
                // printf(">> %d; next step %d, %d\n\n", output[0], output[1], output[2]);

                if (debug) printf("Evaluating target [%d, %d]: %d, [%d, %d]\n",
                                  target_x, target_y,
                                  output[0], output[1], output[2]);

                int compare1 = compare3(output[0], target_y, target_x, ft_cost, ft_sy, ft_sx);
                int compare2 = compare3(0, output[2], output[1], 0, ft_y, ft_x);
                if (output[0] != INT_MAX &&
                    ((compare1 < 0) || (compare1 == 0 && compare2 < 0))) {

                  if (debug) printf("Updating best target.\n");

                  final_target = target;
                  ft_cost = output[0];
                  ft_x = output[1];
                  ft_y = output[2];
                  ft_sy = target_y;
                  ft_sx = target_x;
                }
              }
            }
          }

          target = target->n;
        }

        if (!any_valid) {
          printf("Couldn't find any antagonists for %c [%d, %d]. Ending match during round %d.\n\n",
                 iter->t, iter->x, iter->y, round + 1);
          // reset_debug(grid, x, y);
          // print_grid(grid, x, y);
          // print_army(head);

          int outcome = 0;
          struct cell* result_iter = head;
          while (result_iter != NULL) {
            outcome += result_iter->hp;
            result_iter = result_iter->n;
          }
          printf("\nThe OUTCOME of the match is: %d * %d = %d.\n",
                 outcome, round, outcome * round);
          return outcome;
        }

        if (final_target != NULL) {
          // printf("%c [%d, %d] moves to [%d, %d] targeting %c [%d, %d]\n",
          //        iter->t, iter->x, iter->y, ft_x, ft_y,
          //        final_target->t, final_target->x, final_target->y);

          grid[ft_y][ft_x].hp = iter->hp;
          grid[ft_y][ft_x].t = iter->t;
          grid[ft_y][ft_x].n = iter->n;
          grid[ft_y][ft_x].p = iter->p;

          if (iter->n != NULL) iter->n->p = &grid[ft_y][ft_x];
          else tail = &grid[ft_y][ft_x];
          if (iter->p != NULL) iter->p->n = &grid[ft_y][ft_x];
          else head = &grid[ft_y][ft_x];

          iter->hp = 0;
          iter->t = '.';
          iter->n = NULL;
          iter->p = NULL;

          iter = &grid[ft_y][ft_x];

          for (int i = 0; i < 4; i++) {
            int itx = iter->x + target_xs[i];
            int ity = iter->y + target_ys[i];
            if (grid[ity][itx].hp != 0 &&
                grid[ity][itx].t != iter->t &&
                compare3(grid[ity][itx].hp, ity, itx,
                         pt_hp, pt_y, pt_x) < 0) {
              in_range = true;
              pt_y = ity;
              pt_x = itx;
              pt_hp = grid[pt_y][pt_x].hp;
            }
          }
        }
      }

      if (in_range) {
        int damage = 3;
        if (iter->t  == 'E') {
          damage = elf_power;
        }
        // printf("%c [%d, %d] (%d) attacks %c [%d, %d] (%d -> %d)\n",
        //        iter->t, iter->x, iter->y, iter->hp,
        //        grid[pt_y][pt_x].t, pt_x, pt_y, grid[pt_y][pt_x].hp,
        //        grid[pt_y][pt_x].hp - damage);
        grid[pt_y][pt_x].hp -= damage;

        if (grid[pt_y][pt_x].hp < 0) {
          // printf("%c [%d, %d] died.\n", grid[pt_y][pt_x].t, pt_x, pt_y);
          if (grid[pt_y][pt_x].t == 'E') {
            printf("Elf died at power level %d, skipping this battle.\n", elf_power);
            return -1;
          }
          grid[pt_y][pt_x].hp = 0;
          grid[pt_y][pt_x].t = '.';
          if (grid[pt_y][pt_x].p != NULL) grid[pt_y][pt_x].p->n = grid[pt_y][pt_x].n;
          else head = head->n;
          if (grid[pt_y][pt_x].n != NULL) grid[pt_y][pt_x].n->p = grid[pt_y][pt_x].p;
          else tail = tail->p;
          grid[pt_y][pt_x].n = NULL;
          grid[pt_y][pt_x].p = NULL;
        }
      }

      // reset_debug(grid, x, y);
      // print_grid(grid, x, y);
      iter = iter->n;
    }

    round++;

    // printf("\n");
    // reset_debug(grid, x, y);
    // print_grid(grid, x, y);
    // print_army(head);

    // printf("\n^ Finished round %d\n\n", round);
  }

  return -1;
}

int main(void) {
  struct cell** grid = calloc(MAX, sizeof(struct cell*));
  for (int i = 0; i < MAX; i++) {
    grid[i] = calloc(MAX, sizeof(struct cell));
  }

  int x = 0, y = 0;

  int ch;

  int max_x = 0;

  while((ch = getchar()) != EOF) {
    if (ch == '\n') {
      if (max_x < x) max_x = x;
      x = 0;
      y++;
    } else {
      grid[y][x].t = (char) ch;
      grid[y][x].y = y;
      grid[y][x].x = x;
      if (ch == 'G' || ch == 'E') {
        grid[y][x].hp = 200;
      }

      x++;
    }
  }
  x = max_x;

  reset_debug(grid, x, y);
  print_grid(grid, x, y);


  printf("\n%d, %d\n",x, y);
  struct cell** grid_copy = calloc(y, sizeof(struct cell*));

  for (int i = 0; i < y; i++)
    grid_copy[i] = calloc(MAX, sizeof(struct cell));

  int i = 3;
  int final_outcome = -1;
  for (; final_outcome == -1 && i < 1000; i++){
    for (int j = 0; j < y; j++) {
      memcpy(grid_copy[j], grid[j], x * sizeof(struct cell));
    }
    final_outcome = outcome(grid_copy, x, y, i);
  }

  printf("Final outcome at power level %d: %d\n", i - 1, final_outcome);

  for (int i = 0; i < MAX; i++) {
    free(grid[i]);
  }
  for (int i = 0; i < y; i++)
    free(grid_copy[i]);

  free(grid);
  free(grid_copy);
  return 0;
}

void print_grid(struct cell** grid, int x, int y) {
  printf("xx ");
  for (int j = 0; j < x; j+=2) {
    printf("%02d  ", j);
  }
  printf("\n");

  for (int i = 0; i < y; i++) {
    printf("%02d ", i);
    for (int j = 0; j < x; j++) {
      int c = grid[i][j].debug == 0 ? grid[i][j].t : grid[i][j].debug;
      if (c > 0) {
        printf("%c%c", (char) c, (char) c);
      } else {
        printf("%02d", -c);
      }
    }
    printf("\n");
  }

  printf("xx ");
  for (int j = 1; j < x; j+=2) {
    printf("  %02d", j);
  }
  printf("\n\n");
}

void reset_debug(struct cell** grid, int x, int y) {
  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      grid[i][j].debug = 0;
    }
  }
}

void print_army(struct cell* iter) {
  while (iter != NULL) {
    printf("%c %d, %d (%d)\n", iter->t, iter->x, iter->y, iter->hp);
    iter = iter->n;
  }
  printf("\n");
}

struct pathfind_cell {
  int x;
  int y;

  bool visited;

  int source_x;
  int source_y;

  int score;

  bool in_heap;
};


int pathfind_cmp(void* a, void* b) {
  struct pathfind_cell* pa = (struct pathfind_cell*) a;
  struct pathfind_cell* pb = (struct pathfind_cell*) b;
  return compare3(pa->score, pa->y, pa->x,
                  pb->score, pb->y, pb->x);
}

void pathfind_print(void* node) {
  struct pathfind_cell* cell = (struct pathfind_cell*) node;
  printf("%d [%d, %d]", cell->score, cell->x, cell->y);
}

void pathfind(int ax, int ay, int bx, int by, struct cell** grid, int x, int y, int* output) {
  bool debug2 = false;

  reset_debug(grid, x, y);
  grid[ay][ax].debug = 'A';
  grid[by][bx].debug = 'B';

  // printf("Looking for path\n%d, %d -> %d, %d\n", ax, ay, bx, by);
  // print_grid(grid, x, y);

  struct heap open_nodes;
  heap_init(&open_nodes, pathfind_cmp, x);

  struct pathfind_cell** pgrid = calloc(y, sizeof(*pgrid));
  for (int i = 0; i < y; i++) {
    pgrid[i] = calloc(x, sizeof(*pgrid[i]));
  }

  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      pgrid[i][j].y = i;
      pgrid[i][j].x = j;
      pgrid[i][j].score = INT_MAX;
      pgrid[i][j].source_x = INT_MAX;
      pgrid[i][j].source_y = INT_MAX;
      pgrid[i][j].in_heap = false;
    }
  }

  pgrid[ay][ax].score = 0;
  pgrid[ay][ax].in_heap = true;

  heap_add(&open_nodes, &pgrid[ay][ax]);

  bool found = false;
  while (heap_size(&open_nodes) > 0) {
    if (debug2) {
      heap_print(&open_nodes, pathfind_print);
      printf("\n");
    }

    struct pathfind_cell* top = heap_pop(&open_nodes);
    int top_x = top->x, top_y = top->y;
    grid[top_y][top_x].debug = 'o';
    if (debug2) printf("o %d, %d: %d\n", top_x, top_y, top->score );
    if (debug2) print_grid(grid, x, y);

    top->visited = true;
    top->in_heap = false;

    if (top_x == bx && top_y == by) {
      found = true;
      if (debug2) printf("Found path: %d\n", top->score);
      // break;
    }

    for (int k = 0; k < 4; k++) {
      int i = target_xs[k];
      int j = target_ys[k];

      struct pathfind_cell* neighbor = &pgrid[top_y + i][top_x + j];
      if (neighbor->visited || grid[top_y + i][top_x + j].t != '.')
        continue;

      int new_score = top->score + 1;
      if (compare3(new_score, top_y, top_x,
                   neighbor->score, neighbor->source_y, neighbor->source_x) < 0) {
        if (neighbor->in_heap)
          heap_remove(&open_nodes, &neighbor);
        if (debug2) {
          heap_print(&open_nodes, pathfind_print);
          printf("\nRemoving ");
          pathfind_print(neighbor);
          printf("\n");
          heap_print(&open_nodes, pathfind_print);
          printf("\n");
        }

        neighbor->score = new_score;
        neighbor->source_x = top->x;
        neighbor->source_y = top->y;
        neighbor->in_heap = true;

        heap_add(&open_nodes, neighbor);
        if (debug2) {
          printf("\nAdding ");
          pathfind_print(neighbor);
          printf("\n");
          heap_print(&open_nodes, pathfind_print);
          printf("\n");
        }

        grid[top_y + i][top_x + j].debug = -(new_score);

        if (debug2) printf("x %d, %d: %d from (%d, %d)\n",
                           neighbor->x, neighbor->y, neighbor->score,
                           top->x, top->y);
        if (debug2) print_grid(grid, x, y);
      }
    }

    grid[top_y][top_x].debug = '@';
  }

  int ret = INT_MAX;
  int last_x = INT_MAX;
  int last_y = INT_MAX;
  if (found) {
    reset_debug(grid, x, y);
    grid[ay][ax].debug = 'A';

    int itx = bx, ity = by;
    int temp = 0;
    while (itx != ax || ity != ay) {
      grid[ity][itx].debug = '$';
      int p_ity = pgrid[ity][itx].source_y;
      int p_itx = pgrid[ity][itx].source_x;

      last_x = itx;
      last_y = ity;

      ity = p_ity;
      itx = p_itx;
    }

    ret = pgrid[by][bx].score;
    if (debug) printf("Distance: %d\n", ret);
    if (debug) print_grid(grid, x, y);
  } else {
    if (debug) printf("No path found!\n");
  }

  for (int i = 0; i < y; i++)
    free(pgrid[i]);
  free(pgrid);

  output[0] = ret;
  output[1] = last_x;
  output[2] = last_y;
}
