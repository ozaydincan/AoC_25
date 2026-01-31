#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>

void aoc_fatal(const char *msg)
{
  fprintf(stderr, "fatal: %s\n", msg ? msg : "(null)");
  exit(1);
}
