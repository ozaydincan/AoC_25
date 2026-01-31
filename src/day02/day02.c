#include "aoc.h"
#include "common.h"
#include "string_utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef bool (*validator_fn)(
    const char *); // Function pointer type thing to use as a wrapper

static char *solve_day02(const char *input, validator_fn is_invalid);
static bool is_invalid_part1(const char *id_str);
static bool is_invalid_part2(const char *id_str);

// Function signatures for Testing
char *day02_part1(const char *input) {
  return solve_day02(input, is_invalid_part1);
}

char *day02_part2(const char *input) {
  return solve_day02(input, is_invalid_part2);
}

void day02(void) {
  char *input = read_file("inputs/day02.txt");
  if (!input) {
    perror("File not read correctly");
    return;
  }

  char *result_1 = day02_part1(input);
  printf("Day 02 Part 1: %s\n", result_1);
  free(result_1);

  char *result_2 = day02_part2(input);
  printf("Day 02 Part 2: %s\n", result_2);
  free(result_2);

  free(input);
}

static char *solve_day02(const char *input, validator_fn is_invalid) {
  char *result = malloc(32);
  if (!result)
    return NULL;

  char *input_cpy = str_dup(input);
  if (!input_cpy) {
    free(result);
    return NULL;
  }

  long long start, end;
  unsigned long long total_sum = 0;

  char *tok = strtok(input_cpy, ",");
  while (tok) {
    if (sscanf(tok, "%lld-%lld", &start, &end) == 2) {
      for (long long i = start; i <= end; i++) {
        char id[32];
        sprintf(id, "%lld", i);

        if (is_invalid(id)) {
          total_sum += i;
        }
      }
    }
    tok = strtok(NULL, ",");
  }

  sprintf(result, "%llu", total_sum);
  free(input_cpy);

  return result;
}

static bool is_invalid_part1(const char *id_str) {
  size_t len = strlen(id_str);

  if (len % 2 != 0)
    return false;

  size_t half = len / 2;
  return memcmp(id_str, id_str + half, half) == 0;
}

static bool is_invalid_part2(const char *id_str) {
  size_t len = strlen(id_str);

  for (size_t pattern_len = 1; pattern_len <= len / 2; pattern_len++) {

    if (len % pattern_len != 0)
      continue;

    bool is_repeated = true;
    for (size_t i = pattern_len; i < len; i += pattern_len) {
      if (memcmp(id_str, id_str + i, pattern_len) != 0) {
        is_repeated = false;
        break;
      }
    }

    if (is_repeated) {
      return true;
    }
  }

  return false;
}
