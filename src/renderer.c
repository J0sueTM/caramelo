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
    "Source:   0x%x"
    "Type:     0x%x\n"
    "ID:       0x%x"
    "Severity: 0x%s\n"
    "Message:  %s",
    src, type, id, severity, msg
  );
}

bool crm_init_rndr() {
  if (!gladLoadGLLoader((GLADloadproc)glXGetProcAddress)) {
    log_fatal("Failed to initialize GLAD");
    return false;
  }
  log_info("Initialized GLAD");
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_msg_callback, 0);

  log_info("Initialized OpenGL renderer");
  return true;
}

void crm_deinit_rndr() {}