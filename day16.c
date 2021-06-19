#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const bool DEBUG = false;
const int OPCODES_COUNT = 16;

int regs[4] = { 0 };

struct opcode {
  char* name;
  int (*fn)(int a, int b);
  int (*a)(int arg);
  int (*b)(int arg);
};

int add(int a, int b) {
  return a + b;
}

int mul(int a, int b) {
  return a * b;
}

int ban(int a, int b) {
  return a & b;
}

int bor(int a, int b) {
  return a | b;
}

int set(int a, int _b) {
  return a;
}

int gt(int a, int b) {
  return a > b ? 1 : 0;
}

int eq(int a, int b) {
  return a == b ? 1 : 0;
}

int r(int reg) {
  return regs[reg];
}

int i(int immediate) {
  return immediate;
}

struct opcode opcodes[OPCODES_COUNT] =
  {
   {.name = "addr", .a=r, .b=r, .fn=add},
   {.name = "addi", .a=r, .b=i, .fn=add},
   {.name = "mulr", .a=r, .b=r, .fn=mul},
   {.name = "muli", .a=r, .b=i, .fn=mul},
   {.name = "banr", .a=r, .b=r, .fn=ban},
   {.name = "bani", .a=r, .b=i, .fn=ban},
   {.name = "borr", .a=r, .b=r, .fn=bor},
   {.name = "bori", .a=r, .b=i, .fn=bor},
   {.name = "setr", .a=r, .b=r, .fn=set},
   {.name = "seti", .a=i, .b=i, .fn=set},
   {.name = "gtir", .a=i, .b=r, .fn=gt},
   {.name = "gtri", .a=r, .b=i, .fn=gt},
   {.name = "gtrr", .a=r, .b=r, .fn=gt},
   {.name = "eqir", .a=i, .b=r, .fn=eq},
   {.name = "eqri", .a=r, .b=i, .fn=eq},
   {.name = "eqrr", .a=r, .b=r, .fn=eq}
  };

void apply(const struct opcode* op, int a, int b, int c) {
  regs[c] = op->fn(op->a(a), op->b(b));
}

void print_regs(int* arg_regs) {
  if (arg_regs == NULL) arg_regs = regs;

  int i = 0;
  while (i < 3) printf("%3d, ", arg_regs[i++]);
  printf("%3d", arg_regs[i]);
}

int main(void) {
  int oregs[4] = { 0 };
  int eregs[4] = { 0 };

  int matching3 = 0;
  int total = 0;

  int defs[16][16] = { { 0 } };
  int map[16];
  for (int i = 0; i < 16; i++) map[i] = -1;

  while (true) {
    int o, a, b, c;
    int res = scanf(" Before: [%d, %d, %d, %d]",
          &oregs[0], &oregs[1],
          &oregs[2], &oregs[3]);
    if (res != 4) break;

    int rres = scanf(" %d %d %d %d", &o, &a, &b, &c);
    int eres = scanf(" After: [%d, %d, %d, %d]",
          &eregs[0], &eregs[1],
          &eregs[2], &eregs[3]);

    total++;
    int matching = 0;
    for (int i = 0; i < OPCODES_COUNT; i++) {
      memcpy(regs, oregs, 4 * sizeof(int));
      if (DEBUG) {
        printf("`%s %d %d %d`:  ", opcodes[i].name, a, b, c);
        print_regs(regs);
        printf(" -> ");
      }

      apply(&opcodes[i], a, b, c);

      int comp = memcmp(regs, eregs, 4 * sizeof(int));

      if (DEBUG) {
        print_regs(regs); printf(" =? ");
        print_regs(eregs);
        printf(" -- %d\n", comp);
      }

      if (comp == 0) matching++;
      else defs[o][i]++;
    }

    if (matching >= 3) matching3++;
  }
  printf("Matching 3+ opcodes: %d/%d\n\n", matching3, total);

  int unmapped = OPCODES_COUNT;
  int iterations = 0;
  while (unmapped > 0 && iterations++ < 1000) {
    for (int i = 0; i < OPCODES_COUNT; i++) {
      if (map[i] != -1) continue;

      int last_option = -1;
      int options = 0;
      for (int j = 0; j < OPCODES_COUNT; j++) {
        if (defs[i][j] == 0) {
          options++;
          last_option = j;
        }
      }

      if (options == 1) {
        map[i] = last_option;
        for (int j = 0; j < OPCODES_COUNT; j++) {
          defs[j][last_option]++;
        }
        unmapped--;
      }
    }
  }

  printf("Opcode mapping:\n");
  for (int i = 0; i < OPCODES_COUNT; i++) {
    printf("%d = %s\n", i, opcodes[map[i]].name);
  }
  printf("\n");

  memset(regs, 0, sizeof(regs));
  while (true) {
    int o, a, b, c;
    if (scanf(" %d %d %d %d", &o, &a, &b, &c) != 4)
      break;

    if (DEBUG) {
      print_regs(NULL);
      printf(" | %2d %2d %2d %2d == %s %2d %2d %2d | ",
             o, a, b, c,
             opcodes[map[o]].name, a, b, c);
    }

    apply(&opcodes[map[o]], a, b, c);

    if (DEBUG) {
      print_regs(NULL);
      printf("\n");
    }
  }

  printf("\nThe final value in register 0 is %d\n", regs[0]);

  return 0;
}
