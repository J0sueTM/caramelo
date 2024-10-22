#ifndef CRM_H
#define CRM_H

#include "../vendor/log.c/src/log.h"
#include "../vendor/x11/include/X11/Xlib.h"
#include "./renderer.h"

#include <stdbool.h>
#include <stdlib.h>
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
  Colormap xcolormap;

  CrmColor border_color;
  CrmColor bg_color;

  char *title;
  int w, h;
  int x, y;
  bool is_open;

  CrmRndr rndr;
} CrmWindow;

typedef GLXContext (*glXCreateContextAttribsARBProc)(
  Display*,
  GLXFBConfig,
  GLXContext,
  Bool,
  const int*
);

CrmWindow *crm_init_window(int width, int height);
void crm_deinit_window(CrmWindow *win);
bool crm_is_glx_version_ok(CrmWindow *win);
void crm_resize_window(CrmWindow *win, int w, int h);

#endif
