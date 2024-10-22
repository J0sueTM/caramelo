#include "../vendor/munit/munit.h"

#define CRM_TEST_NO_MAIN
#include "./unit/common/io.c"

MunitSuite suites[] = {
  common_io_suite,
  {0}
};

static const MunitSuite suite = {
  .prefix     = (char *)"/all",
  .tests      = 0,
  .suites     = suites,
  .iterations = 1,
  .options    = MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&suite, (void *)"", argc, argv);
}
