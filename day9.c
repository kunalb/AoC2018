#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct node {
  long marble;
  struct node* n;
  struct node* p;
};

void debug(struct node*);

long max_score(int players, int last_marble) {
  long* scores = calloc(players, sizeof(*scores));
  long marble = 0;
  int player = 0;

  struct node* zero = malloc(sizeof(*zero));
  zero->marble = marble++;
  zero->n = zero;
  zero->p = zero;

  struct node* current = zero;
  struct node* iter;

  while (marble < last_marble) {
    if (marble % 23 == 0) {
      for (int i = 1; i <= 7; i++)
        current = current->p;

      scores[player] += marble + current->marble;
      current = current->n;

      struct node* tmp = current->p;
      tmp->n->p = tmp->p;
      tmp->p->n = tmp->n;
      free(tmp);
    } else {
      struct node* new_marble = malloc(sizeof(*new_marble));
      if (new_marble == NULL) {
        perror("Failed to allocate!");
        exit(-1);
      }

      new_marble->marble = marble;
      new_marble->p = current->n;
      new_marble->n = current->n->n;
      new_marble->p->n = new_marble;
      new_marble->n->p = new_marble;

      current = new_marble;
    }

    // printf("%d\n", player + 1); debug(zero);

    player = (player + 1) % players;
    marble++;
  }

  iter = current;
  do {
    struct node* tmp = iter->n;
    free(iter);
    iter = tmp;
  } while (iter != current);

  int max = 0;
  for (int i = 0; i < players; i++) {
    if (scores[i] > scores[max])
      max = i;
    // printf("%d:%d ", i + 1, scores[i]);
  }

  long max_score = scores[max];
  free(scores);
  return max_score;
}

void debug(struct node* node) {
  struct node* tmp = node;
  do {
    printf("%2ld ", tmp->marble);
    tmp = tmp->n;
  } while (tmp != node);
  printf("\n");
}

int main(void) {
  int last_marble[] = {32, 1618, 7999, 1104, 6111, 5807, 72058, 7205800};
  int players[] = {9, 10, 13, 17, 21, 30, 426, 426};

  for (int i = 0; i < 8; i++) {
    printf("%d %d => %ld\n",
           last_marble[i],
           players[i],
           max_score(players[i], last_marble[i]));
  }

  return 0;
}
