#ifndef CRM_H
#define CRM_H

#include "../vendor/log.c/src/log.h"
#include "../vendor/x11/include/X11/Xlib.h"

typedef unsigned long CrmColor;
typedef XEvent CrmEvent;

typedef struct {
  Window xwin;
  Display *xdisplay;
  int xscreen;
  Atom wm_delete;

  char *title;
  int w, h;
  int x, y;

  CrmColor border_color;
  CrmColor bg_color;
} CrmWindow;

void crm_init_window(CrmWindow *win);
void crm_deinit_window(CrmWindow *win);

#endif
