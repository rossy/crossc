#ifndef CROSSC_PRIV_H_
#define CROSSC_PRIV_H_

#include "SPIRV-Cross/spirv_glsl.hpp"
#include "crossc.h"

struct crossc_compiler {
	spirv_cross::CompilerGLSL *c;
	char *error;
};

#endif
