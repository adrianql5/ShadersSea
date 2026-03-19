#ifndef SHADERSSEA_GLAD_H
#define SHADERSSEA_GLAD_H

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <KHR/khrplatform.h>
#include <GL/glcorearb.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*GLADloadproc)(const char* name);

static inline int gladLoadGLLoader(GLADloadproc /*load*/) {
    return 1;
}

#ifdef __cplusplus
}
#endif

#endif
