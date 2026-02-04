CC=gcc
CFLAGS ?= -Wall -Wextra -O2 -Iinclude
LDFLAGS ?=
CFLAGS_DEBUG=-Wall -Wextra -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude

COMMON_SRC_ALL=$(wildcard src/common/*.c)
COMMON_SRC=$(filter-out src/common/test.c,$(COMMON_SRC_ALL))
AOC_SRC=$(wildcard src/*.c src/day0[1-9]/day*.c src/day1[0-2]/day*.c) $(COMMON_SRC)

# build main binary
aoc: $(AOC_SRC:.c=.o)
	$(CC) $(LDFLAGS) $^ -o $@

# build tests (do NOT link day files except those under test)
TEST_DAY_FILES=$(wildcard tests/test_day*.c)
TEST_DAY_STEMS=$(patsubst tests/test_%.c,%,$(TEST_DAY_FILES))
TEST_DAY_SRC=$(foreach d,$(TEST_DAY_STEMS),src/$(d)/$(d).c)
MISSING_TEST_DAY_SRC=$(filter-out $(wildcard $(TEST_DAY_SRC)),$(TEST_DAY_SRC))
TEST_SRC=$(wildcard tests/*.c) src/common/test.c $(TEST_DAY_SRC)
test: CFLAGS=$(CFLAGS_DEBUG)
test: LDFLAGS=$(CFLAGS_DEBUG)
test: test_check $(COMMON_SRC:.c=.o) $(TEST_SRC:.c=.o)
	$(CC) $(LDFLAGS) $(filter %.o,$^) -o $@

test_check:
	@if [ -n "$(MISSING_TEST_DAY_SRC)" ]; then \
		echo "Missing day source files for tests: $(MISSING_TEST_DAY_SRC)"; \
		exit 1; \
	fi

.PHONY: test_check

runtest: test
	./test

clean:
	rm -f $(AOC_SRC:.c=.o) $(COMMON_SRC:.c=.o) $(TEST_SRC:.c=.o) aoc test
