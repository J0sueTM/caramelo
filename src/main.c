#include "../vendor/log.c/src/log.h"

int sum(int a, int b) {
  int res = a + b;
  log_info("summing %d + %d = %d!", a, b, res);
  return res;
}
