#ifndef CRM_RENDERER_H
#define CRM_RENDERER_H

#include "../vendor/log.c/src/log.h"
#include "./glad/glad.h"
#include "./common/io.h"

#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glx.h>

#define SHADER_NAME_CAP 128
typedef struct {
  char name[SHADER_NAME_CAP];
  char vtx_src[FILE_BUF_CAP];
  char frag_src[FILE_BUF_CAP];
} CrmShader;

#define SHADER_CAP 32
typedef struct {
  CrmShader shaders[SHADER_CAP];
} CrmRndr;

bool crm_init_rndr(CrmRndr *rndr);
void crm_deinit_rndr(CrmRndr *rndr);

#endif
