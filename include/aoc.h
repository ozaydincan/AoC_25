#ifndef AOC_H
#define AOC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* program limits */
#define AOC_MAX_DAYS 12

/* day function signature */
typedef void (*DayFn)(void);

/* utilities */
void aoc_fatal(const char *msg);

/* macros */
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef AOC_DEBUG
#include <stdio.h>
#define DBG(fmt, ...) fprintf(stderr, fmt "\n", __VA_ARGS__)
#else
#define DBG(...)
#endif

#endif
