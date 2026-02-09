#include "aoc.h"
#include "string_utils.h"
#include "test.h"
#include "vector.h"

void test_day01_sample(void);
void test_day02_sample(void);
void test_day03_sample(void);
void test_day04_sample(void);
void test_day05_sample(void);

TEST(test_str_trim) {
  char s[] = "  hello\t\n";
  str_trim(s);
  ASSERT_EQ_STR(s, "hello");
}

TEST(test_str_split) {
  size_t count = 0;
  char **parts = str_split("a,b,,c", ',', &count);
  ASSERT_EQ_SIZE(count, 4);
  ASSERT_EQ_STR(parts[0], "a");
  ASSERT_EQ_STR(parts[1], "b");
  ASSERT_EQ_STR(parts[2], "");
  ASSERT_EQ_STR(parts[3], "c");
  free_split(parts, count);
}

TEST(test_vector_basic) {
  Vector v = vec_create(sizeof(int));
  int x = 42;
  ASSERT_TRUE(vec_push(&v, &x));
  ASSERT_EQ_SIZE(v.len, 1);
  int *p = VEC_AT(int, &v, 0);
  ASSERT_TRUE(p != NULL);
  ASSERT_EQ_INT(*p, 42);
  vec_destroy(&v);
}

int main(int argc, char **argv) {
  const char *filter = NULL;
  if (argc > 1) {
    filter = argv[1];
  }
  TestCase tests[] = {
      TEST_CASE(test_str_trim),     TEST_CASE(test_str_split),
      TEST_CASE(test_vector_basic), TEST_CASE(test_day01_sample),
      TEST_CASE(test_day02_sample), TEST_CASE(test_day03_sample),
      TEST_CASE(test_day04_sample), TEST_CASE(test_day05_sample)};
  return run_tests_filtered(tests, ARRAY_LEN(tests), filter);
}
