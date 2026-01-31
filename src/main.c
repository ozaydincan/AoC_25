#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void day01(void);
void day02(void);
void day03(void);
void day04(void);
void day05(void);
void day06(void);
void day07(void);
void day08(void);
void day09(void);
void day10(void);
void day11(void);
void day12(void);

static void print_usage(const char *prog)
{
  printf("Usage: %s <day|all>\n", prog);
  printf("Example: %s 1\n", prog);
}

int main(int argc, char **argv)
{
  DayFn days[AOC_MAX_DAYS] = {
      day01, day02, day03, day04, day05, day06, day07, day08, day09, day10,
      day11, day12};

  if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    print_usage(argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "all") == 0) {
    for (int i = 0; i < AOC_MAX_DAYS; i++) {
      printf("\n=== Day %02d ===\n", i + 1);
      days[i]();
    }
    return 0;
  }

  char *end = NULL;
  long day = strtol(argv[1], &end, 10);
  if (!end || *end != '\0' || day < 1 || day > AOC_MAX_DAYS) {
    print_usage(argv[0]);
    return 1;
  }

  printf("=== Day %02ld ===\n", day);
  days[day - 1]();
  return 0;
}
