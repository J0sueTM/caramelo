#ifndef FD_RENDERER_H
#define FD_RENDERER_H

#include "../vendor/log.c/src/log.h"
#include "./glad/glad.h"
#include "./common/debug.h"
#include "./common/io.h"

#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glx.h>

/*
 * For the time of me writing this, I don't see a usecase for shader
 * files with more than a KB.
 */
#define SHADER_BUF_CAP 1024
#define SHADER_NAME_CAP 128
typedef struct {
  char name[SHADER_NAME_CAP];
  char vtx_src[SHADER_BUF_CAP];
  char frag_src[SHADER_BUF_CAP];
} FDShader;

#define SHADER_CAP 32
typedef struct {
  FDShader shaders[SHADER_CAP];
} FDRndr;

bool fd_init_rndr(FDRndr *rndr);
void fd_deinit_rndr(FDRndr *rndr);
bool fd_render(FDRndr *rndr);

#endif
