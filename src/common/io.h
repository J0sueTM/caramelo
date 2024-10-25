#ifndef CRM_COMMON_IO_H
#define CRM_COMMON_IO_H

#include "../../vendor/log.c/src/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// WARNING: It's your job to free the dest buffers after usage.

/*
 * Get home dir and return the dir str length.
 */
long crm_home_dir(char **dest);

/*
 * Get resources dir and return the dir str length.
 */
long crm_rsrcs_dir(char **dest);

#define FILE_BUF_CAP 1024 * 1000

long crm_slurp_file(const char *filename, char **dest);

#endif
