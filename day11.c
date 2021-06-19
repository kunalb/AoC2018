#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int power_level(int x, int y, int serial_number);
void best_cell(int serial_number, int output[2]);
void power_up(int serial_number, int cells[300][300]);
void best_large_cell(int serial_number, int output[3]);
void best_smarter_cell(int serial_number, int output[3]);
void debug(int grid[300][300]);

int power_level(int x, int y, int serial_number) {
  int rack_id = x + 10;
  int power_level = (rack_id) * y + serial_number;
  power_level = power_level * rack_id;
  power_level = (power_level % 1000) / 100;
  power_level -= 5;
  return power_level;
}

void power_up(int serial_number, int cells[300][300]) {
  for (int y = 0; y < 300; y++) {
    for (int x = 0; x < 300; x++) {
      cells[y][x] = power_level(x, y, serial_number);
    }
  }
}

void best_cell(int serial_number, int output[2]) {
  int cells[300][300];
  power_up(serial_number, cells);

  int max_sum = 0;
  for (int y = 0; y < 300 - 3; y++) {
    for (int x = 0; x < 300 - 3; x++) {
      int sum = 0;
      for (int k = y; k < y + 3; k++) {
        for (int l = x; l < x + 3; l++) {
          sum += cells[k][l];
        }
      }

      if (sum > max_sum) {
        output[0] = x;
        output[1] = y;
        max_sum = sum;
      }
    }
  }
}

void best_large_cell(int serial_number, int output[3]) {
  int cells[300][300];
  power_up(serial_number, cells);

  int max_sum = 0;
  for (int s = 1; s <= 300; s++) {
    for (int y = 0; y <= 300 - s; y++) {
      for (int x = 0; x <= 300 - s; x++) {
        int sum = 0;
        for (int k = y; k < y + s; k++) {
          for (int l = x; l < x + s; l++) {
            sum += cells[k][l];
          }
        }

        if (sum > max_sum) {
          output[0] = x;
          output[1] = y;
          output[2] = s;
          max_sum = sum;
        }
      }
    }
  }
}

void debug(int grid[300][300]) {
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < 30; j++) {
      printf("%3d ", grid[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void best_smarter_cell(int serial_number, int output[3]) {
  int cells[300][300];
  power_up(serial_number, cells);

  int sum[300][300];
  for (int y = 0; y < 300; y++) {
    for (int x = 0; x < 300; x++) {
      sum[y][x] = cells[y][x]
        + (x > 0 ? sum[y][x - 1] : 0)
        + (y > 0 ? sum[y - 1][x] : 0)
        - (x > 0 && y > 0 ? sum[y - 1][x - 1] : 0);
    }
  }

  int max_sum = INT_MIN;
  for (int s = 1; s <= 300; s++) {
    for (int y = s - 1; y < 300; y++) {
      for (int x = s - 1; x < 300; x++) {
        int total = sum[y][x]
          - (y >= s ? sum[y - s][x] : 0)
          - (x >= s ? sum[y][x - s] : 0)
          + (y >= s && x >= s ? sum[y - s][x - s] : 0);
        if (total > max_sum) {
          output[0] = x - s + 1;
          output[1] = y - s + 1;
          output[2] = s;
          max_sum = total;
        }
      }
    }
  }
}

int main(void) {
  int inputs[] = {18, 42, 1718};
  int output[2];
  int outputs[3];
  for (int i = 0; i < 3; i++) {
    best_cell(inputs[i], output);
    best_smarter_cell(inputs[i], outputs);

    printf("%d,%d\n", output[0], output[1]);
    printf("%d,%d,%d\n\n", outputs[0], outputs[1], outputs[2]);
  }
}
