#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

const bool DEBUG = false;
const int OPCODES_COUNT = 16;

int regs[6] = { 0 };

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

const struct opcode* code(const char* name) {
  for (int i = 0; i < OPCODES_COUNT; i++)
    if (strcmp(name, opcodes[i].name) == 0)
      return &opcodes[i];

  assert(false);
}

void print_regs(int* arg_regs) {
  if (arg_regs == NULL) arg_regs = regs;

  int i = 0;
  printf("[");
  while (i < 5) printf("%12d,", arg_regs[i++]);
  printf("%12d]", arg_regs[i]);
}

int main(void) {
  int ipr, ip = 0, sz = 0;
  char opname[10] = { 0 };

  char instrs[50][10];
  int a[50], b[50], c[50];

  scanf(" #ip %d", &ipr);
  while (scanf(" %s %d %d %d",
               instrs[sz], &a[sz], &b[sz], &c[sz]) == 4)
    sz++;

  regs[0] = 0;
  regs[1] = 3;
  regs[2] = 10551306;
  regs[3] = 10551306;
  regs[4] = 10551306;
  regs[5] = 1;

  ip = 3;

  printf("%10d ", -1);
  print_regs(NULL);
  printf("\n");

  int index = 0;
  while (ip < sz) {
    printf("%10d ip=%2d ", index, ip);
    printf(" %s %2d %2d %2d ", instrs[ip], a[ip], b[ip], c[ip]);

    regs[ipr] = ip;
    apply(code(instrs[ip]), a[ip], b[ip], c[ip]);
    ip = regs[ipr];

    print_regs(NULL);
    printf("\n");

    ip++;
    index++;
  }

  print_regs(NULL);
  printf("\n");

  return 0;
}
