#include "./renderer.h"

void gl_msg_callback(
  GLenum src,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar *msg,
  const void *user_param
) {
  (void)length;
  (void)user_param;
  log_error(
    "=== GL Callback %s ===\n"
    "Source:   0x%x\n"
    "Type:     0x%x\n"
    "ID:       0x%x\n"
    "Severity: 0x%x\n"
    "Message:  %s",
    src, type, id, severity, msg
  );
}

bool fd_init_rndr(FDRndr *rndr) {
  fd_assert(rndr, { return false; });

  fd_assert_msg(
    gladLoadGLLoader((GLADloadproc)glXGetProcAddress),
    { return false; },
    "Failed to initialize GLAD"
  );
  log_info("Initialized GLAD");
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_msg_callback, 0);

  // load shaders
  {
    char *rsrcs_dir;
    uint8_t rsrc_dir_len = fd_rsrcs_dir(&rsrcs_dir);
    fd_assert(rsrc_dir_len > 0, { goto fail_load_shaders; })

    // rsrcs_dir + "/shaders"
    uint8_t shader_dir_len = rsrc_dir_len + 8;
    fd_assert_msg(
      shader_dir_len < (FILENAME_BUF_CAP - 1),
      { goto fail_load_shaders; },
      "Shaders dir name exceeds the limit of 256 chars. "
    );

    char *rsrc_fnames = 0;
    uint16_t fcount = fd_load_dir(rsrcs_dir, &rsrc_fnames, 0);
    fd_assert_noop(fcount > 0);
    for (int i = 0; i < fcount; ++i) {
      char *shader_fname = (rsrc_fnames + (i * FILENAME_BUF_CAP));

      log_debug("file n. %d: %s", i, shader_fname);
    }

    free(rsrc_fnames);
    free(rsrcs_dir);
    
    goto ok;

fail_load_shaders:
    log_error("Failed to load shaders");
    if (rsrc_fnames) free(rsrc_fnames);
    if (rsrcs_dir) free(rsrcs_dir);
    return false;
  }

ok:
  log_info("Initialized OpenGL renderer");
  return true;
}

void fd_deinit_rndr(FDRndr *rndr) {
  fd_assert_noop(rndr);
}

bool fd_render(FDRndr *rndr) {
  fd_assert(rndr, { return false; });

  return true;
}
