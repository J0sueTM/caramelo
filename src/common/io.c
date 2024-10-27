#include "./io.h"

// Using 256b as size limit to comply with dirent later on.
uint8_t fd_home_dir(char **dest) {
#ifdef CRM_HOME
  uint8_t home_dir_len = strlen(CRM_HOME);
  fd_assert_noop(home_dir_len > 0);

  *dest = calloc(home_dir_len + 1, sizeof(char));
  fd_assert_noop(*dest)
  strcpy(*dest, CRM_HOME);

  return strlen(CRM_HOME);
#else
  *dest = getenv("CRM_HOME");
  if (!*dest) {
#define CWD_BUF_CAP 128
    *dest = calloc(CWD_BUF_CAP, sizeof(char));
    fd_assert_noop(*dest);
    if (!getcwd(*dest, sizeof(char) * (CWD_BUF_CAP - 1))) {
      **dest = '.';
    }
  }

  uint8_t dir_len = strlen(*dest);
  if (dir_len <= 0) {
    log_warn("Home dir has 0 bytes in length");
  }

  return dir_len;
#endif
}

uint8_t fd_rsrcs_dir(char **dest) {
  char *home_dir;
  uint8_t home_dir_len = fd_home_dir(&home_dir);
  if (home_dir_len <= 0) {
    return 0;
  }

  // home_dir + /resources + \0
  uint8_t dir_len = home_dir_len + 10 + 1;
  fd_assert_msg_noop(
    (dir_len < 256),
    "Resource dir length %d exceeds limit of 256 bytes",
    dir_len
  );
  *dest = calloc(dir_len, sizeof(char));
  memcpy(*dest, home_dir, home_dir_len);
  strcat(*dest, "/resources");

  free(home_dir);

  return dir_len;
}

uint16_t fd_load_dir(
  const char *dirname,
  char *dest[FILE_CAP][FILENAME_BUF_CAP],
  uint8_t pad
) {
  fd_assert_noop(dirname);
  fd_assert_noop(*dest);

  DIR *dir = opendir(dirname);
  fd_assert_noop(dir);
  struct dirent *ent;
  uint16_t fcount = 0;
  while (fcount < FILE_CAP) {
    ent = readdir(dir);
    if (!ent) {
      break;
    } else if (
      // Skip curdir and updir.
      strcmp(ent->d_name, ".")  == 0 ||
      strcmp(ent->d_name, "..") == 0
    ) {
      continue;
    }

    // Recurse dir
    if (ent->d_type == DT_DIR) {
      // dirname + '/' + inner_dirname
      char inner_dirname[FILENAME_BUF_CAP];
      strcpy(inner_dirname, dirname);
      *(inner_dirname + strlen(dirname)) = '/';
      strcpy((inner_dirname + strlen(dirname) + 1), ent->d_name);

      log_debug("recursing inner dir %s", inner_dirname);
      fcount += fd_load_dir(inner_dirname, dest, fcount);
      
      continue;
    }

    log_debug("Found file %s", ent->d_name);
    char *fname_buf = *dest[fcount + pad];
    strcpy(fname_buf, ent->d_name);

    ++fcount;
  }
  closedir(dir);

  log_info("Loaded %d files from dir %s", fcount, dirname);
  return fcount;
}

void log_slurp_error(
  const char *reason,
  const char *filename,
  FILE *fp
) {
  log_error(
    "Failed to slurp file %s while %s: %d",
    filename,
    reason,
    ferror(fp)
  );
  fclose(fp);
}

uint32_t fd_slurp_file(const char *filename, char **dest) {
  fd_assert_noop(filename);

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    log_slurp_error("opening", filename, fp);
    return 0;
  }

  // Go to the end of file.
  if (fseek(fp, 0L, SEEK_END)) {
    log_slurp_error("seeking end", filename, fp);
    return 0;
  }

  // Get and check file size.
  const uint32_t fsize = ftell(fp);
  const uint32_t max_fsize = FILE_BUF_CAP - 1; // for \0
  const bool fits_into_buf_cap = fsize < max_fsize;
  fd_assert_msg(
    fits_into_buf_cap,
    {
      log_slurp_error("checking size", filename, fp);
      return 0;
    },
    "File %s has size of %d bytes, but max capacity is %d bytes"
  );
  fd_assert(
    (fsize > 0),
    {
      log_slurp_error("checking size", filename, fp);
      return 0;
    }
  );

  *dest = calloc(fsize, sizeof(char));
  fd_assert_msg_noop(
    *dest,
    "Failed to allocate buffer for file %s", filename
  );

  // go to the start of file
  if (fseek(fp, 0L, SEEK_SET)) {
    log_slurp_error("seeking start", filename, fp);
    return 0;
  }

  size_t bytes_read = fread(*dest, sizeof(char), fsize, fp);
  if (bytes_read <= 0) {
    log_slurp_error("reading", filename, fp);
    return 0;
  }

  fclose(fp);
  log_debug("Read %d bytes from file %s", bytes_read, filename);

  return bytes_read;
}
