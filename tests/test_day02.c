#include "day02.h"
#include "test.h"
#include "test_utils.h"

void test_day02_sample(void) {
  char *input = read_fixture("day02_sample.txt");
  ASSERT_TRUE(input != NULL);

  char *result_1 = day02_part1(input);
  char *result_2 = day02_part2(input);
  free(input);

  ASSERT_EQ_STR(result_1, "1227775554");
  ASSERT_EQ_STR(result_2, "4174379265");
}
