#include "./io.h"

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

void crm_slurp_file(const char *filename, char *dest) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    log_slurp_error("opening", filename, fp);
    return;
  }

  // Go to the end of file
  if (fseek(fp, 0L, SEEK_END)) {
    log_slurp_error("seeking end", filename, fp);
    return;
  }

  memset(dest, 0, FILE_BUF_CAP);
  const long fsize = ftell(fp);
  const int max_buf_size = FILE_BUF_CAP - 1;
  const bool fits_into_buf_cap = fsize < max_buf_size;
  if (!fits_into_buf_cap) {
    log_error(
      "File %s has size of %d bytes, but max capacity is %d bytes",
      filename,
      fsize
    );
    log_slurp_error("checking size", filename, fp);
    return;
  } else if (fsize < 0) {
    log_slurp_error("checking size", filename, fp);
    return;
  }

  // go to the start of file
  if (fseek(fp, 0L, SEEK_SET)) {
    log_slurp_error("seeking start", filename, fp);
    return;
  }

  size_t bytes_read = fread(dest, sizeof(char), fsize, fp);
  if (bytes_read ) {
    log_slurp_error("reading", filename, fp);
    return;
  }

  fclose(fp);
  log_debug("Read %d bytes from file %s", bytes_read, filename);
}
