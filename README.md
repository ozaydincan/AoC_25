# 🎄 Advent of Code 2025 (C11)

This repository contains my **Advent of Code 2025** solutions written in **pure C (C11)**.  
I am intentionally solving AoC in C to practice low-level, systems-style programming:

- Manual memory management
- Custom data structures
- No C++
- Reusable infrastructure
- Strong focus on correctness and structure

This is **not** a collection of quick scripts.  
It is a growing C codebase that happens to solve Advent of Code.

---

## Motivation

Most AoC solutions use high-level languages.  
I chose C to:

- Strengthen pointer and memory discipline
- Practice building containers (vector, hashmap)
- Control allocation strategies (arena allocator)
- Write predictable, transparent code
- Build reusable infrastructure

---

## Repository Layout

```
AoC_25
├── aoc
├── build
├── include
│   ├── aoc.h
│   ├── arena.h
│   ├── common.h
│   ├── day01.h
│   ├── fileio.h
│   ├── hashmap.h
│   ├── string_utils.h
│   ├── test.h
│   └── vector.h
├── inputs
│   └── day01.txt
├── Makefile
├── src
│   ├── common
│   │   ├── aoc.c
│   │   ├── arena.c
│   │   ├── fileio.c
│   │   ├── hashmap.c
│   │   ├── string_utils.c
│   │   ├── test.c
│   │   └── vector.c
│   ├── day01
│   │   └── day01.c
│   ├── day02
│   │   └── day02.c
│   ├── ...
│   ├── day12
│   │   └── day12.c
│   └── main.c
├── tests
│   ├── fixtures
│   │   └── day01_sample.txt
│   ├── test_day01.c
│   ├── test_main.c
│   └── test_utils.c
```

---

## Build Requirements

- clang or gcc  
- make  
- Target standard: **C11**

---

## Build

```bash
make
```

Clean build:

```bash
make clean
make
```

---

## Running

Run a specific day:

```bash
./aoc 1
./aoc 7
./aoc 12
```

Run all implemented days:

```bash
./aoc all
```

Day dispatch logic lives in:

- `include/aoc.h`
- `src/common/aoc.c`

---

## Inputs

Puzzle inputs go in:

```
inputs/day01.txt
inputs/day02.txt
...
```

Each day loads its own file using `fileio`.

---

## Tests

Run tests:

```bash
make test
```

Fixtures live in:

```
tests/fixtures/
```

---

## Shared Modules

**Arena Allocator**  
Fast bump allocator for short-lived allocations.  
`arena.h / arena.c`

**Vector**  
Resizable dynamic array.  
`vector.h / vector.c`

**Hashmap**  
General-purpose hash table.  
`hashmap.h / hashmap.c`

**File I/O**  
Load entire files or lines.  
`fileio.h / fileio.c`

**String Utilities**  
Parsing helpers.  
`string_utils.h / string_utils.c`

**Test Framework**  
Minimal unit test helpers.  
`test.h / test.c`

---

## Design Rules

- No hidden allocations  
- No global mutable state  
- Explicit ownership  
- Small functions  
