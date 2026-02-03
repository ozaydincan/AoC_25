#include "day03.h"
#include "aoc.h"
#include "common.h"
#include "fileio.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int day03_part1(const char **input, const size_t lineCount);
long long day03_part2(const char **input, const size_t lineCount);
static inline int findMaxBattery(const char *line, const size_t lineLen);
static inline long long findMaxBattery12(const char *line,
                                         const size_t lineLen);
void day03(void) {
  size_t lineCount = 0;
  const char **input =
      (const char **)read_lines("inputs/day03.txt", &lineCount);
  if (!input) {
    perror("File not read!");
  }

  int result1 = day03_part1(input, lineCount);
  long long result2 = day03_part2(input, lineCount);

  printf("Day 03 Part 1: %d\n", result1);
  printf("Day 03 Part 2: %lld\n", result2);
}

int day03_part1(const char **input, const size_t lineCount) {
  int totalJolt = 0;
  for (size_t i = 0; i < lineCount; i++) {
    size_t lineLen = strlen(input[i]);
    char *batteryBank = strdup(input[i]);
    totalJolt += findMaxBattery(batteryBank, lineLen);
  }
  return totalJolt;
}

static inline int findMaxBattery(const char *line, const size_t lineLen) {
  if (lineLen < 2)
    return -1;

  char bestLeft = '0';
  char bestRight = '0';
  char maxRight = line[lineLen - 1];

  for (int i = (int)lineLen - 2; i >= 0; i--) {
    char current = line[i];

    if (current > bestLeft || (current == bestLeft && maxRight > bestRight)) {
      bestLeft = current;
      bestRight = maxRight;
    }

    if (current > maxRight) {
      maxRight = current;
    }
  }

  return (bestLeft - '0') * 10 + (bestRight - '0');
}

long long day03_part2(const char **input, const size_t lineCount) {
  long long totalJolt = 0;
  for (size_t i = 0; i < lineCount; i++) {
    size_t lineLen = strlen(input[i]);
    char *batteryBank = strdup(input[i]);
    totalJolt += findMaxBattery12(batteryBank, lineLen);
  }
  return totalJolt;
}

static inline long long findMaxBattery12(const char *line,
                                         const size_t lineLen) {
  const int NEEDED = 12;
  if (lineLen < NEEDED)
    return -1;

  char result[13] = {0};
  int resultIdx = 0;
  int startPos = 0;

  for (int pos = 0; pos < NEEDED; pos++) {
    int remaining = NEEDED - pos - 1;
    int searchEnd = lineLen - remaining;

    char maxDigit = '0';
    int maxIdx = startPos;

    for (int i = startPos; i < searchEnd; i++) {
      if (line[i] > maxDigit) {
        maxDigit = line[i];
        maxIdx = i;
      }
    }

    result[resultIdx++] = maxDigit;
    startPos = maxIdx + 1;
  }

  return atoll(result);
}
