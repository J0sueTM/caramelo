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

bool crm_load_shaders(CrmShader *shaders) {
  // TODO
  char foobar[FILE_BUF_CAP];
  crm_slurp_file(
    "/home/jtm/dev/caramelo/resources/shaders/default.shader",
    foobar
  );

  log_debug("file = \n%s", foobar);

  return true;
}

bool crm_init_rndr(CrmRndr *rndr) {
  if (!gladLoadGLLoader((GLADloadproc)glXGetProcAddress)) {
    log_fatal("Failed to initialize GLAD");
    return false;
  }
  log_info("Initialized GLAD");
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_msg_callback, 0);

  if (!crm_load_shaders(rndr->shaders)) return false;

  log_info("Initialized OpenGL renderer");
  return true;
}

void crm_deinit_rndr(CrmRndr *rndr) {}
