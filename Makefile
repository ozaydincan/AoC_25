CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude
CFLAGS_DEBUG=-Wall -Wextra -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude

COMMON_SRC_ALL=$(wildcard src/common/*.c)
COMMON_SRC=$(filter-out src/common/test.c,$(COMMON_SRC_ALL))
AOC_SRC=$(wildcard src/*.c src/day0[1-9]/day*.c src/day1[0-2]/day*.c) $(COMMON_SRC)

# build main binary
aoc: $(AOC_SRC:.c=.o)
	$(CC) $^ -o $@

# build tests (do NOT link day files)
TEST_SRC=$(wildcard tests/*.c) src/common/test.c src/day01/day01.c
test: CFLAGS=$(CFLAGS_DEBUG)
test: LDFLAGS=$(CFLAGS_DEBUG)
test: $(COMMON_SRC:.c=.o) $(TEST_SRC:.c=.o)
	$(CC) $(LDFLAGS) $^ -o $@

runtest: test
	./test

clean:
	rm -f $(AOC_SRC:.c=.o) $(COMMON_SRC:.c=.o) $(TEST_SRC:.c=.o) aoc test
