#ifndef FD_RENDERER_H
#define FD_RENDERER_H

#include "../vendor/log.c/src/log.h"
#include "./glad/glad.h"
#include "./common/debug.h"
#include "./common/io.h"

#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glx.h>

#define SHADER_BUF_CAP UINT16_MAX
#define SHADER_NAME_CAP UINT8_MAX
typedef struct {
  char name[SHADER_NAME_CAP];
  char vtx_src[SHADER_BUF_CAP];
  char frag_src[SHADER_BUF_CAP];
} FDShader;

#define SHADER_CAP UINT8_MAX
typedef struct {
  FDShader shaders[SHADER_CAP];
} FDRndr;

bool fd_init_rndr(FDRndr *rndr);
void fd_deinit_rndr(FDRndr *rndr);
bool fd_render(FDRndr *rndr);

#endif
