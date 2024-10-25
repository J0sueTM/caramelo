#include "../../../vendor/munit/munit.h"

#ifndef CRM_TEST_COMMON_IO_IMPL
#define CRM_TEST_COMMON_IO_IMPL

#include "../../../src/common/io.h"

MunitResult test_crm_home_dir(
  const MunitParameter params[],
  void *user_data
) {
  (void)params;
  (void)user_data;

  setenv("CRM_HOME", "/opt/caramelo", 1);
  
  char *dir;
  long dir_len = crm_home_dir(&dir);
  munit_assert_long(dir_len, >, 0);
  munit_assert_string_equal(dir, "/opt/caramelo");
  free(dir);

  return MUNIT_OK;
}

MunitTest common_io_tests[] = {
  {
    .name       = (char *)"home_dir",
    .test       = test_crm_home_dir,
    .setup      = NULL,
    .tear_down  = NULL,
    .options    = MUNIT_TEST_OPTION_NONE,
    .parameters = NULL
  },
  {0}
};

const MunitSuite common_io_suite = {
  .prefix     = (char *)"/common/io/crm_",
  .tests      = common_io_tests,
  .suites     = NULL,
  .iterations = 1,
  .options    = MUNIT_SUITE_OPTION_NONE
};

#ifndef CRM_TEST_NO_MAIN
int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&common_io_suite, (void *)"", argc, argv);
}
#endif

#endif
