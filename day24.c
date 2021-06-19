#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct group {
  int units;
  int hp;
  int[5] immune;
  int immuneLen;
  int[5] weak;
  int weakLen;
  int damage;
  int attack;
  int initiative;
}

int main(void) {
  group groups[20];
  for (int i = 0; i < 20; i++) {
    scanf(" %d, %d, (%d; %d)")
  }
  return 0;
}
