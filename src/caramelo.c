#include "./caramelo.h"

void crm_init_window(CrmWindow *win) {
  win->xdisplay = XOpenDisplay(NULL);
  if (!win->xdisplay) {
    log_fatal("failed to start X display");
  }
  win->xscreen = DefaultScreen(win->xdisplay);
  log_debug("X screen = %d", win->xscreen);

  win->xwin = XCreateSimpleWindow(
    win->xdisplay,
    RootWindow(win->xdisplay, win->xscreen),
    win->x, win->y,
    win->w, win->h,
    1, win->border_color, win->bg_color
  );

  XSelectInput(win->xdisplay, win->xwin, ExposureMask);
  XMapWindow(win->xdisplay, win->xwin);

  win->wm_delete = XInternAtom(
    win->xdisplay,
    "WM_DELETE_WINDOW",
    false
  );
  XSetWMProtocols(win->xdisplay, win->xwin, &win->wm_delete, 1);

  log_info("started X display");
}

void crm_deinit_window(CrmWindow *win) {
  XDestroyWindow(win->xdisplay, win->xwin);
  XCloseDisplay(win->xdisplay);
}
