#include "test.h"

#include <string.h>

int g_test_failures = 0;
int g_test_assertions = 0;

int run_tests_filtered(const TestCase *tests, size_t count,
                       const char *filter) {
  size_t selected = 0;
  for (size_t i = 0; i < count; i++) {
    if (!filter || strstr(tests[i].name, filter) != NULL) {
      selected++;
    }
  }

  printf("Running %zu tests...\n", selected);

  for (size_t i = 0; i < count; i++) {
    if (filter && strstr(tests[i].name, filter) == NULL) {
      continue;
    }
    int before = g_test_failures;
    tests[i].fn();
    if (g_test_failures == before) {
      printf("[ OK ] %s\n", tests[i].name);
    } else {
      printf("[FAIL] %s\n", tests[i].name);
    }
  }

  printf("\nAssertions: %d\nFailures:   %d\n", g_test_assertions,
         g_test_failures);
  return (g_test_failures == 0) ? 0 : 1;
}

int run_tests(const TestCase *tests, size_t count) {
  return run_tests_filtered(tests, count, NULL);
}
