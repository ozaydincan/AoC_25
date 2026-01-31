#include "day01.h"
#include "aoc.h"
#include "common.h"
#include "string_utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define DIAL_LEN 100

static inline int wrap_position(int pos) {
  pos %= DIAL_LEN;
  return pos < 0 ? pos + DIAL_LEN : pos;
}


int day01_part1(const char *input) {
  int count = 0;
  int position = 50;
  size_t lineCount = 0;
  char **positions = str_split(input, '\n', &lineCount);
  for (size_t i = 0; i < lineCount; i++) {
    char drxn;
    int magnitude;
    if (positions[i][0] == '\0') {
      continue;
    }
    if (sscanf(positions[i], " %c%d", &drxn, &magnitude) != 2) {
      continue;
    }
    if (drxn == 'L') {
      position = wrap_position(position - magnitude);
    } else if (drxn == 'R') {
      position = wrap_position(position + magnitude);
    }
    if (position == 0) {
      count++;
    }
  }
  free_split(positions, lineCount);
  return count;
}

static inline int wrap_with_count(int pos, int delta, int *zero_count) {
  int step = (delta >= 0) ? 1 : -1;
  int steps = (delta >= 0) ? delta : -delta;

  for (int i = 0; i < steps; i++) {
    pos = wrap_position(pos + step);
    if (pos == 0) {
      (*zero_count)++;
    }
  }

  return pos;
}

int day01_part2(const char *input) {
  int count = 0;
  int pos = 50;
  size_t lineCount = 0;
  char **positions = str_split(input, '\n', &lineCount);
  for (size_t i = 0; i < lineCount; i++) {
    char drxn;
    int magnitude;
    if (positions[i][0] == '\0') {
      continue;
    }
    if (sscanf(positions[i], " %c%d", &drxn, &magnitude) != 2) {
      continue;
    }
    if (drxn == 'L') {
      pos = wrap_with_count(pos, -magnitude, &count);
    } else if (drxn == 'R') {
      pos = wrap_with_count(pos, magnitude, &count);
    }
  }
  free_split(positions, lineCount);
  return count;
}

void day01(void) {
  const char *path = "inputs/day01.txt";
  char *input = read_file(path);
  if (!input) {
    printf("Day 01: missing input at %s\n", path);
    return;
  }

  int result = day01_part1(input);
  int result_2 = day01_part2(input);
  printf("Day 01, Part 1: %d\n", result);
  printf("Day 01, Part 2: %d\n", result_2);
  free(input);
}
