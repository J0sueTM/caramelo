#ifndef FD_COMMON_IO_H
#define FD_COMMON_IO_H

#include "../../vendor/log.c/src/log.h"
#include "./debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>

#define FILE_CAP UINT8_MAX
// taken from dirent's fname buf cap
#define FILENAME_BUF_CAP UINT8_MAX

// Using 256 as size limit to comply with dirent later on.
uint8_t fd_home_dir(char **dest);
uint8_t fd_rsrcs_dir(char **dest);

// Returns the qtt. of inner files found.
// Padding is the current file padding, which needs to be known since
// it recurses.
uint16_t fd_load_dir(
  const char *dirname,
  char **dest,
  uint8_t pad 
);
 
// Don't see a reason to have bigger files. If needed, refactor this
// in the future.
#define FILE_BUF_CAP UINT32_MAX

/**
 * Returns the size of the slurpped file.
 */
uint32_t fd_slurp_file(const char *filename, char **dest);

#endif
