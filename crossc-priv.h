#ifndef CROSSC_PRIV_H_

#include "crossc.h"
#include "SPIRV-Cross/spirv_glsl.hpp"

struct crossc_compiler {
	spirv_cross::CompilerGLSL *c;
	char *error;
};

#endif
