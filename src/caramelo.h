#ifndef CRM_H
#define CRM_H

#include "../vendor/log.c/src/log.h"
#include "../vendor/x11/include/X11/Xlib.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glx.h>

typedef unsigned long CrmColor;

typedef struct {
  Window xwin;
  Display *xdisplay;
  int xscreen;
  XVisualInfo *xvisual_info;
  Atom wm_delete;
  GLXFBConfig glx_fb_cfg;
  GLXContext glx_ctx;

  char *title;
  int w, h;
  int x, y;

  CrmColor border_color;
  CrmColor bg_color;

  bool is_open;
} CrmWindow;

typedef GLXContext (*glXCreateContextAttribsARBProc)(
  Display*,
  GLXFBConfig,
  GLXContext,
  Bool,
  const int*
);

void crm_init_window(CrmWindow *win);
void crm_deinit_window(CrmWindow *win);
bool crm_is_glx_version_ok(CrmWindow *win);

#endif
