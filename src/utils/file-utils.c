#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 *   file_utils_exists
 * Checks that the provided filename exists on the host filesystem
 *   @return true if the file exists, false otherwise
 */
bool file_utils_exists(const char *filename)
{
  bool exists;
  FILE *f = fopen(filename, "r");
  if (f != NULL) {
    exists = true;
    fclose(f);
  }
  else {
    exists = false;
  }

  return exists;
}
