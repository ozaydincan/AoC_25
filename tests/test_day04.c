#include "day04.h"
#include "fileio.h"
#include "test.h"
#include <stddef.h>

void test_day04_sample(void) {
  size_t lineCount = 0;
  const char **input =
      (const char **)read_lines("tests/fixtures/day04_sample.txt", &lineCount);
  ASSERT_TRUE(input != NULL);
  ASSERT_TRUE(lineCount > 0);

  int result_1 = day04_part1(input, lineCount);
  int result_2 = day04_part2(input, lineCount);
  free_lines((char **)input, lineCount);

  ASSERT_EQ_INT(result_1, 13);
  ASSERT_EQ_INT(result_2, 43);
}
