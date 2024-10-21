#include <stdlib.h>

#include "../src/caramelo.h"

int main(void) {
  CrmWindow win = {0};
  win.w = 800;
  win.h = 500;
  if (!crm_init_window(&win)) {
    log_fatal("Failed to start camarelo :(");
    exit(1);
  }

  crm_deinit_window(&win);
}
