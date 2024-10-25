#include "./io.h"

long crm_home_dir(char **dest) {
  *dest = getenv("CRM_HOME");
  if (!*dest) {
#define CWD_BUF_CAP 128
    *dest = calloc(CWD_BUF_CAP, sizeof(char));
    if (!getcwd(*dest, sizeof(char) * (CWD_BUF_CAP - 1))) {
      **dest = '.';
    }
  }

  long dir_len = strlen(*dest);
  if (dir_len <= 0) {
    log_warn("Home dir has 0 bytes in length");
  }

  return dir_len;
}

long crm_rsrcs_dir(char **dest) {
  char *home_dir;
  long home_dir_len = crm_home_dir(&home_dir);
  if (home_dir_len <= 0) {
    return 0;
  }

  /* home_dir + /resources + \0 */
  long dir_len = home_dir_len + 10 + 1;
  *dest = calloc(dir_len, sizeof(char));
  memcpy(*dest, home_dir, home_dir_len);
  strcat(*dest, "/resources");

  free(home_dir);

  return dir_len;
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

long crm_slurp_file(const char *filename, char **dest) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    log_slurp_error("opening", filename, fp);
    return 0;
  }

  // Go to the end of file
  if (fseek(fp, 0L, SEEK_END)) {
    log_slurp_error("seeking end", filename, fp);
    return 0;
  }

  const long fsize = ftell(fp);
  const long max_fsize = FILE_BUF_CAP - 1; // for \0
  const bool fits_into_buf_cap = fsize < max_fsize;
  if (!fits_into_buf_cap) {
    log_error(
      "File %s has size of %d bytes, but max capacity is %d bytes",
      filename,
      fsize,
      FILE_BUF_CAP
    );
    log_slurp_error("checking size", filename, fp);
    return 0;
  } else if (fsize < 0) {
    log_slurp_error("checking size", filename, fp);
    return 0;
  }

  *dest = calloc(fsize, sizeof(char));
  if (!*dest) {
    log_error("Failed to allocate buffer for file %s", filename);
    return 0;
  }

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
