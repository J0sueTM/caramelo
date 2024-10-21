#include "../src/caramelo.h"
#include <unistd.h>

int main(void) {
  CrmWindow win = {0};
  win.w = 800;
  win.h = 500;
  crm_init_window(&win);

  crm_deinit_window(&win);
}
