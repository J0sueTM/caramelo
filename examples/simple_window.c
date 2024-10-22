#include <stdlib.h>

#include "../src/caramelo.h"

int main(void) {
  CrmWindow *win = crm_init_window(800, 500);
  if (!win) {
    log_fatal("Failed to start camarelo :(");
    exit(1);
  }

  crm_deinit_window(win);
}
