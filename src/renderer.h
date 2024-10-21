#ifndef CRM_RENDERER_H
#define CRM_RENDERER_H

#include "../vendor/log.c/src/log.h"
#include "./glad/glad.h"

#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glx.h>

bool crm_init_rndr();
void crm_deinit_rndr();

#endif
