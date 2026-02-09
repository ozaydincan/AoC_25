#include "aoc.h"
#include "fileio.h"
#include "vector.h"

#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  long long low;
  long long high;
} Range;

int day05_part1(const char **input, const size_t lineCount);
long long day05_part2(const char **input, const size_t lineCount);
static inline int isFresh(long long boundL, long long boundH, long long ingID);
static int range_cmp(const void *a, const void *b);
static Vector merge_ranges(Vector ranges);
static Vector parse_merged_ranges(const char **input, size_t lineCount,
                                  size_t *out_start_ids);
static inline int is_blank_line(const char *s);
static inline int findInRange(Vector ranges, const size_t vecLen,
                              const char *ingId);

void day05(void) {
  size_t lineCount = 0;
  char **input = read_lines("inputs/day05.txt", &lineCount);
  if (!lineCount) {
    perror("File not read!\n");
  }
  int result1 = day05_part1((const char **)input, lineCount);
  printf("Day 05 Part 1: %d\n", result1);
  long long result2 = day05_part2((const char **)input, lineCount);
  printf("Day 05 Part 2: %lld\n", result2);
}

int day05_part1(const char **input, const size_t lineCount) {
  size_t ids_start = 0;
  Vector merged = parse_merged_ranges(input, lineCount, &ids_start);
  int freshIng = 0;
  const size_t mergedLen = merged.len;
  for (size_t i = ids_start; i < lineCount; i++) {
    freshIng += findInRange(merged, mergedLen, input[i]);
  }
  vec_destroy(&merged);
  return freshIng;
}

long long day05_part2(const char **input, const size_t lineCount) {
  size_t ids_start = 0;
  Vector merged = parse_merged_ranges(input, lineCount, &ids_start);
  (void)ids_start;
  long long total = 0;
  for (size_t i = 0; i < merged.len; i++) {
    Range *current = (Range *)vec_get(&merged, i);
    if (current == NULL) {
      perror("Out of bounds error!");
      continue;
    }
    if (current->high >= current->low) {
      total += (current->high - current->low + 1);
    }
  }
  vec_destroy(&merged);
  return total;
}

static inline int isFresh(long long boundL, long long boundH, long long ingID) {
  if (boundH >= ingID && boundL <= ingID) {
    return 1;
  }
  return 0;
}

static int range_cmp(const void *a, const void *b) {
  const Range *ra = (const Range *)a;
  const Range *rb = (const Range *)b;
  if (ra->low < rb->low)
    return -1;
  if (ra->low > rb->low)
    return 1;
  if (ra->high < rb->high)
    return -1;
  if (ra->high > rb->high)
    return 1;
  return 0;
}

static Vector merge_ranges(Vector ranges) {
  Vector merged = vec_create(sizeof(Range));
  if (ranges.len == 0) {
    return merged;
  }

  Range cur = *(Range *)vec_get(&ranges, 0);
  for (size_t i = 1; i < ranges.len; i++) {
    Range next = *(Range *)vec_get(&ranges, i);
    if (next.low <= cur.high + 1) {
      if (next.high > cur.high) {
        cur.high = next.high;
      }
      continue;
    }
    vec_push(&merged, &cur);
    cur = next;
  }
  vec_push(&merged, &cur);
  return merged;
}

static Vector parse_merged_ranges(const char **input, size_t lineCount,
                                  size_t *out_start_ids) {
  Vector ranges = vec_create(sizeof(Range));
  size_t i = 0;
  for (; i < lineCount; i++) {
    if (is_blank_line(input[i])) {
      i++;
      break;
    }
    Range newIDS;
    if (sscanf(input[i], "%lld-%lld", &newIDS.low, &newIDS.high) == 2) {
      vec_insert(&ranges, ranges.len, &newIDS);
      continue;
    }
    break;
  }

  qsort(ranges.data, ranges.len, sizeof(Range), range_cmp);
  Vector merged = merge_ranges(ranges);
  vec_destroy(&ranges);

  if (out_start_ids) {
    *out_start_ids = i;
  }
  return merged;
}

static inline int is_blank_line(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}

static inline long long getNum(const char *str) {
  char *eptr;
  long long result;

  result = strtoll(str, &eptr, 10);

  if (result == 0) {
    if (errno == EINVAL) {
      printf("Conversion failed: %d\n", errno);
      exit(0);
    }
  }
  if (result == LLONG_MIN || result == LLONG_MAX) {
    /* If the value provided was out of range, display a warning message */
    if (errno == ERANGE)
      printf("The value provided was out of range\n");
  }
  return result;
}
static inline int findInRange(Vector ranges, const size_t vecLen,
                              const char *ingId) {
  if (ingId[0] == '\0') {
    return 0;
  }
  for (size_t i = 0; i < vecLen; i++) {
    Range *current;
    current = (Range *)vec_get(&ranges, i);
    if (current == NULL) {
      perror("Out of bounds error!");
    }
    long long ingredient = getNum(ingId);
    if (isFresh(current->low, current->high, ingredient))
      return 1;
  }

  return 0;
}
