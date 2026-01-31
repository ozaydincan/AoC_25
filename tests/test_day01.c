#include "day01.h"
#include "test.h"
#include "test_utils.h"

void test_day01_sample(void)
{
  char *input = read_fixture("day01_sample.txt");
  ASSERT_TRUE(input != NULL);

  int result_1 = day01_part1(input);
  int result_2 = day01_part2(input);
  free(input);

  ASSERT_EQ_INT(result_1, 3);
  ASSERT_EQ_INT(result_2, 6);
}
