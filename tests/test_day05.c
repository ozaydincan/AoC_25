#include "day05.h"
#include "fileio.h"
#include "test.h"
#include <stddef.h>

void test_day05_sample(void) {
  size_t lineCount = 0;
  const char **input =
      (const char **)read_lines("tests/fixtures/day05_sample.txt", &lineCount);
  ASSERT_TRUE(input != NULL);
  ASSERT_TRUE(lineCount > 0);

  int result_1 = day05_part1(input, lineCount);
  long long result_2 = day05_part2(input, lineCount);
  free_lines((char **)input, lineCount);

  ASSERT_EQ_INT(result_1, 3);
  ASSERT_EQ_INT(result_2, 14);
}
