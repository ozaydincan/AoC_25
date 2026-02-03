#include "day03.h"
#include "fileio.h"
#include "test.h"
#include <stddef.h>

void test_day03_sample(void) {
  size_t lineCount = 0;
  const char **input =
      (const char **)read_lines("tests/fixtures/day03_sample.txt", &lineCount);
  ASSERT_TRUE(input != NULL);
  ASSERT_TRUE(lineCount > 0);

  int result_1 = day03_part1(input, lineCount);
  long long result_2 = day03_part2(input, lineCount);
  free_lines((char **)input, lineCount);

  ASSERT_EQ_INT(result_1, 357);
  ASSERT_EQ_INT(result_2, 3121910778619);
}
