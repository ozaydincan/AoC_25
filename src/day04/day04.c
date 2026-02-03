#include "aoc.h"
#include "common.h"
#include "fileio.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

int day04_part1(const char **input, const size_t lineCount);
int day04_part2(const char **input, const size_t lineCount);
static inline int isReachable(const char **mat, size_t n, size_t m, size_t row,
                              size_t col);
static inline int isInBounds(int row, int col, int n, int m);

void day04(void) {
  size_t lineCount;
  const char **input =
      (const char **)read_lines("inputs/day04.txt", &lineCount);
  int result1 = day04_part1(input, (const size_t)lineCount);
  printf("Day 04 Part 1: %d\n", result1);
  int result2 = day04_part2(input, (const size_t)lineCount);
  printf("Day 04 Part 2: %d\n", result2);
}

int day04_part1(const char **input, const size_t lineCount) {
  const size_t colSize = strlen(input[0]);
  int count = 0;
  for (size_t i = 0; i < lineCount; i++) {
    for (size_t j = 0; j < colSize; j++) {
      if (input[i][j] == '@') {
        count += isReachable(input, lineCount, colSize, i, j);
      }
    }
  }
  return count;
}

int day04_part2(const char **input, const size_t lineCount) {
  const size_t colSize = strlen(input[0]);
  int totalRemoved = 0;
  int changed = 1;
  char **grid = malloc(lineCount * sizeof(char *));
  for (size_t i = 0; i < lineCount; i++) {
    grid[i] = malloc((colSize + 1) * sizeof(char));
    strcpy(grid[i], input[i]);
  }
  while (changed) {
    changed = 0;
    for (size_t i = 0; i < lineCount; i++) {
      for (size_t j = 0; j < colSize; j++) {
        if (grid[i][j] == '@') {
          if (isReachable((const char **)grid, lineCount, colSize, i, j)) {
            grid[i][j] = '.';
            totalRemoved++;
            changed = 1;
          }
        }
      }
    }
  }
  for (size_t i = 0; i < lineCount; i++) {
    free(grid[i]);
  }
  free(grid);
  return totalRemoved;
}

static inline int isInBounds(int row, int col, int n, int m) {
  if (row >= 0 && row < n && col >= 0 && col < m)
    return true;
  return false;
}

static inline int isReachable(const char **mat, size_t n, size_t m, size_t row,
                              size_t col) {
  int count = 0;
  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (size_t i = 0; i < 8; i++) {
    int newRow = row + dx[i];
    int newCol = col + dy[i];
    if (isInBounds(newRow, newCol, n, m) && mat[newRow][newCol] == '@') {
      count++;
      if (count >= 4)
        return 0;
    }
  }
  return 1;
}
