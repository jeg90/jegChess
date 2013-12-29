#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * This function checks whether a file at the given
 * filename exists and is openable.  Returns true if
 * the file exists, false otherwise.
 */
bool file_utils_exists(const char *filename);
#endif
