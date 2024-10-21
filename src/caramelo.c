#include "./caramelo.h"

bool crm_is_glx_version_ok(CrmWindow *win) {
  GLint major, minor = 0;
	glXQueryVersion(win->xdisplay, &major, &minor);
	if (major <= 1 && minor < 2) {
		log_fatal(
      "GLX v%d.%d outdated. Requires at least v1.2",
      major, minor
    );
		return false;
	}
  log_debug("GLX v%d.%d available", major, minor);

  return true;
}

GLXFBConfig crm_get_best_glx_fb_cfg(
  CrmWindow *win,
  GLXFBConfig *fb_cfgs,
  int fb_cfg_count
) {
	int best_fb_cfg_id = 0, best_sample_count;
	for (int i = 0; i < fb_cfg_count; ++i) {
		XVisualInfo *visual_info = glXGetVisualFromFBConfig(
      win->xdisplay,
      fb_cfgs[i]
    );
		if (!visual_info) {
			int sample_buf_count, sample_count;
			glXGetFBConfigAttrib(
        win->xdisplay,
        fb_cfgs[i],
        GLX_SAMPLE_BUFFERS,
        &sample_buf_count
      );
			glXGetFBConfigAttrib(
        win->xdisplay,
        fb_cfgs[i],
        GLX_SAMPLES,
        &sample_count
      );

			if (sample_buf_count && sample_count > best_sample_count) {
				best_fb_cfg_id = i;
				best_sample_count = sample_count;
			}
		}
    XFree(visual_info);
	}

	return fb_cfgs[best_fb_cfg_id];
}

bool crm_setup_glx(CrmWindow *win) {
  const GLint glx_attribs[] = {
    GLX_X_RENDERABLE,  true,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE,      8,
    GLX_GREEN_SIZE,    8,
    GLX_BLUE_SIZE,     8,
    GLX_ALPHA_SIZE,    8,
    GLX_DEPTH_SIZE,    24,
    GLX_STENCIL_SIZE,  8,
    GLX_DOUBLEBUFFER,  true,
    0
  };

  int fb_count;
	GLXFBConfig *fb_cfgs = glXChooseFBConfig(
    win->xdisplay,
    win->xscreen,
    glx_attribs,
    &fb_count
  );
	if (!fb_cfgs) {
		log_fatal("Failed to retrieve framebuffer config");
		return false;
	}

  win->glx_fb_cfg = crm_get_best_glx_fb_cfg(
    win,
    fb_cfgs,
    fb_count
  );
  if (win->glx_fb_cfg <= 0) {
    log_fatal("Failed to get best framebuffer config");
    return false;
  }
  XFree(fb_cfgs);
  log_debug("Selected framebuffer config %d", win->glx_fb_cfg);

  win->xvisual_info = glXGetVisualFromFBConfig(
    win->xdisplay,
    win->glx_fb_cfg
  );
	if (!win->xvisual_info) {
    log_fatal(
      "Failed to get visual info for framebuffer: %d",
      win->glx_fb_cfg
    );
		return false;
	} else if (win->xscreen != win->xvisual_info->screen) {
    log_fatal(
      "Visual info from framebuffer %d has invalid screen id %d, \
expected %d",
      win->glx_fb_cfg,
      win->xvisual_info->screen,
      win->xscreen
    );
    return false;
	}
  
  return true;
}

bool crm_setup_window(CrmWindow *win) {
	XSetWindowAttributes win_attribs;
	win_attribs.border_pixel = BlackPixel(win->xdisplay, win->xscreen);
	win_attribs.background_pixel = WhitePixel(
    win->xdisplay,
    win->xscreen
  );
	win_attribs.override_redirect = true;
	win_attribs.colormap = XCreateColormap(
    win->xdisplay,
    RootWindow(win->xdisplay, win->xscreen),
    win->xvisual_info->visual,
    AllocNone
  );
	win_attribs.event_mask = ExposureMask;
	win->xwin = XCreateWindow(
    win->xdisplay,
    RootWindow(win->xdisplay, win->xscreen),
    win->x, win->y,
    win->w, win->h,
    0,
    win->xvisual_info->depth,
    InputOutput,
    win->xvisual_info->visual,
    CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
    &win_attribs
  );
  if (!win->xwin) {
    log_fatal("Failed to create window");
    return false;
  }

  XStoreName(win->xdisplay, win->xwin, win->title);

  XSelectInput(win->xdisplay, win->xwin, ExposureMask);
  XMapWindow(win->xdisplay, win->xwin);

  win->wm_delete = XInternAtom(
    win->xdisplay,
    "WM_DELETE_WINDOW",
    false
  );
  XSetWMProtocols(win->xdisplay, win->xwin, &win->wm_delete, 1);

  log_info("Created window");
  return true;
}

bool crm_setup_glx_ctx(CrmWindow *win) {
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB =
    (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
      (const GLubyte *)"glXCreateContextAttribsARB"
    );
	 
	const char *glx_exts = glXQueryExtensionsString(
    win->xdisplay,
    win->xscreen
  );
  const bool is_ctx_ext_supported = strstr(
    glx_exts,
    "GLX_ARB_create_context"
  );
	if (!is_ctx_ext_supported) {
    log_warn("GLXARB_create_context extension not supported");
		win->glx_ctx = glXCreateNewContext(
      win->xdisplay,
      win->glx_fb_cfg,
      GLX_RGBA_TYPE,
      0,
      true
    );
	} else {
    const int ctx_attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
      GLX_CONTEXT_MINOR_VERSION_ARB, 6,
      GLX_CONTEXT_FLAGS_ARB,
      GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
      0
    };
	 
		win->glx_ctx = glXCreateContextAttribsARB(
      win->xdisplay,
      win->glx_fb_cfg,
      0,
      true,
      ctx_attribs
    );
	}
	XSync(win->xdisplay, false);

  log_debug(
    "Acquired %s GLX context",
    (glXIsDirect(win->xdisplay, win->glx_ctx)) ? "Direct" : "Indirect"
  );
  glXMakeCurrent(win->xdisplay, win->xwin, win->glx_ctx);

  return true;
}

void crm_init_window(CrmWindow *win) {
  win->xdisplay = XOpenDisplay(NULL);
  if (!win->xdisplay) {
    log_fatal("Failed to start X display");
    return;
  }
  win->xscreen = DefaultScreen(win->xdisplay);
  log_debug("Started X display at screen %d", win->xscreen);

  if (!crm_is_glx_version_ok(win)) goto force_win_cleanup;
  if (!crm_setup_glx(win))         goto force_win_cleanup;
  if (!crm_setup_window(win))      goto force_win_cleanup;
  if (!crm_setup_glx_ctx(win))     goto force_win_cleanup;
 
  win->is_open = true;
  XEvent evt;
  while (win->is_open) {
    XNextEvent(win->xdisplay, &evt);
    if (
      evt.type == ClientMessage &&
      evt.xclient.data.l[0] == win->wm_delete
    ) {
      win->is_open = false;
    }
  }

  return;

force_win_cleanup:
  log_warn("Forcefully closing window");
  crm_deinit_window(win);
}

void crm_deinit_window(CrmWindow *win) {
  glXMakeCurrent(win->xdisplay, 0, 0);
  glXDestroyContext(win->xdisplay, win->glx_ctx);

  XDestroyWindow(win->xdisplay, win->xwin);
  XCloseDisplay(win->xdisplay);
  log_info("Closed window");
}
