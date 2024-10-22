#ifndef CRM_COMMON_IO_H
#define CRM_COMMON_IO_H

#include "../../vendor/log.c/src/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * For the time of me writing this, I don't see a usecase for files
 * with more than a KB. If needed in the future, this should be
 * refactored.
 */
#define FILE_BUF_CAP 1024

void crm_slurp_file(const char *filename, char *dest);

#endif
