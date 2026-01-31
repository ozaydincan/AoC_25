#include "test_utils.h"
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_fixture(const char *filename)
{
  if (!filename) {
    return NULL;
  }

  const char *prefix = "tests/fixtures/";
  size_t len = strlen(prefix) + strlen(filename) + 1;
  char *path = (char *)malloc(len);
  if (!path) {
    return NULL;
  }
  snprintf(path, len, "%s%s", prefix, filename);

  char *data = read_file(path);
  free(path);
  return data;
}
