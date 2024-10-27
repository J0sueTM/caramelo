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

bool fd_load_shaders(FDShader *shaders) {
  fd_assert(shaders, { return false; });
  // char *home_dir;
  // long home_dir_len = fd_home_dir(&home_dir);
  // log_debug("%s len = %d", home_dir, home_dir_len);
  // free(home_dir);
  //  
  // char *rsrcs_dir;
  // long rsrcs_dir_len = fd_rsrcs_dir(&rsrcs_dir);
  // log_debug("%s len = %d", rsrcs_dir, rsrcs_dir_len);
  // free(rsrcs_dir);
  //  
  // char *foobar = 0;
  // fd_slurp_file(
  //   "/home/jtm/dev/caramelo/resources/shaders/default.shader",
  //   &foobar
  // );
  //
  // log_debug("file = \n%s", foobar);
  // free(foobar);

  return true;
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

  // fd_assert(fd_load_shaders(rndr->shaders), { return false; });

  // load shaders
  {
    
  }

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
