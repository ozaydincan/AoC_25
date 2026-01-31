
#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*TestFn)(void);

typedef struct {
  const char *name;
  TestFn fn;
} TestCase;

/* --- Internal counters (defined in test.c) --- */
extern int g_test_failures;
extern int g_test_assertions;

/* --- Assertions --- */

#define TEST_FAIL(msg)                                                         \
  do {                                                                         \
    fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, (msg));          \
    g_test_failures++;                                                         \
    return;                                                                    \
  } while (0)

#define ASSERT_TRUE(expr)                                                      \
  do {                                                                         \
    g_test_assertions++;                                                       \
    if (!(expr)) {                                                             \
      fprintf(stderr, "[FAIL] %s:%d: ASSERT_TRUE(%s)\n", __FILE__, __LINE__,   \
              #expr);                                                          \
      g_test_failures++;                                                       \
      return;                                                                  \
    }                                                                          \
    printf("[ OK ] %s:%d: ASSERT_TRUE(%s)\n", __FILE__, __LINE__, #expr);      \
  } while (0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ_INT(a, b)                                                    \
  do {                                                                         \
    g_test_assertions++;                                                       \
    long long _a = (long long)(a);                                             \
    long long _b = (long long)(b);                                             \
    if (_a != _b) {                                                            \
      fprintf(stderr, "[FAIL] %s:%d: ASSERT_EQ_INT(%s,%s) got %lld vs %lld\n", \
              __FILE__, __LINE__, #a, #b, _a, _b);                             \
      g_test_failures++;                                                       \
      return;                                                                  \
    }                                                                          \
    printf("[ OK ] %s:%d: ASSERT_EQ_INT(%s,%s)\n", __FILE__, __LINE__, #a,     \
           #b);                                                                \
  } while (0)

#define ASSERT_EQ_SIZE(a, b)                                                   \
  do {                                                                         \
    g_test_assertions++;                                                       \
    size_t _a = (size_t)(a);                                                   \
    size_t _b = (size_t)(b);                                                   \
    if (_a != _b) {                                                            \
      fprintf(stderr, "[FAIL] %s:%d: ASSERT_EQ_SIZE(%s,%s) got %zu vs %zu\n",  \
              __FILE__, __LINE__, #a, #b, _a, _b);                             \
      g_test_failures++;                                                       \
      return;                                                                  \
    }                                                                          \
    printf("[ OK ] %s:%d: ASSERT_EQ_SIZE(%s,%s)\n", __FILE__, __LINE__, #a,    \
           #b);                                                                \
  } while (0)

#define ASSERT_EQ_STR(a, b)                                                    \
  do {                                                                         \
    g_test_assertions++;                                                       \
    const char *_a = (a);                                                      \
    const char *_b = (b);                                                      \
    if ((_a == NULL && _b != NULL) || (_a != NULL && _b == NULL) ||            \
        (_a && _b && strcmp(_a, _b) != 0)) {                                   \
      fprintf(stderr, "[FAIL] %s:%d: ASSERT_EQ_STR(%s,%s)\n", __FILE__,        \
              __LINE__, #a, #b);                                               \
      fprintf(stderr, "       got \"%s\" vs \"%s\"\n", _a ? _a : "(null)",     \
              _b ? _b : "(null)");                                             \
      g_test_failures++;                                                       \
      return;                                                                  \
    }                                                                          \
    printf("[ OK ] %s:%d: ASSERT_EQ_STR(%s,%s)\n", __FILE__, __LINE__, #a,     \
           #b);                                                                \
  } while (0)

/* Register a test case easily */
#define TEST(name) static void name(void)
#define TEST_CASE(test_name)                                                   \
  (TestCase) { .name = #test_name, .fn = test_name }

/* Run a list of tests. Returns exit code (0 ok, 1 fail). */
int run_tests(const TestCase *tests, size_t count);

/* Run tests matching filter (substring on test name). */
int run_tests_filtered(const TestCase *tests, size_t count,
                       const char *filter);

#endif /* TEST_H */
