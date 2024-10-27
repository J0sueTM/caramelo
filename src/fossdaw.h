#ifndef FD_H 
#define FD_H

#include "../vendor/log.c/src/log.h"
#include "../vendor/x11/include/X11/Xlib.h"
#include "./common/debug.h"
#include "./renderer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glx.h>

typedef unsigned long FDColor;

typedef struct {
  Window xwin;
  Display *xdisplay;
  int xscreen;
  XVisualInfo *xvisual_info;
  Atom wm_delete;
  GLXFBConfig glx_fb_cfg;
  GLXContext glx_ctx;
  Colormap xcolormap;

  FDColor border_color;
  FDColor bg_color;

  char *title;
  int w, h;
  int x, y;
  bool is_open;

  FDRndr rndr;
} FDWindow;

typedef GLXContext (*glXCreateContextAttribsARBProc)(
  Display *,
  GLXFBConfig,
  GLXContext,
  Bool,
  const int *
);

FDWindow *fd_init_window(int width, int height);
void fd_deinit_window(FDWindow *win);
bool fd_is_glx_version_ok(FDWindow *win);
void fd_resize_window(FDWindow *win, int w, int h);

#endif
