#include "../../../vendor/munit/munit.h"

#ifndef FD_TEST_COMMON_IO_IMPL
#define FD_TEST_COMMON_IO_IMPL

#include "../../../src/common/io.h"

MunitResult test_fd_home_dir(
  const MunitParameter params[],
  void *user_data
) {
  (void)params;
  (void)user_data;

  setenv("FD_HOME", "/opt/fossdaw", 1);
  
  char *dir;
  long dir_len = fd_home_dir(&dir);
  munit_assert_long(dir_len, >, 0);
  munit_assert_string_equal(dir, "/opt/fossdaw");
  free(dir);

  return MUNIT_OK;
}

MunitTest common_io_tests[] = {
  {
    .name       = (char *)"home_dir",
    .test       = test_fd_home_dir,
    .setup      = NULL,
    .tear_down  = NULL,
    .options    = MUNIT_TEST_OPTION_NONE,
    .parameters = NULL
  },
  {0}
};

const MunitSuite common_io_suite = {
  .prefix     = (char *)"/common/io/fd_",
  .tests      = common_io_tests,
  .suites     = NULL,
  .iterations = 1,
  .options    = MUNIT_SUITE_OPTION_NONE
};

#ifndef FD_TEST_NO_MAIN
int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&common_io_suite, (void *)"", argc, argv);
}
#endif

#endif
