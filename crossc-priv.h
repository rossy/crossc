#ifndef CROSSC_PRIV_H_
#define CROSSC_PRIV_H_

#include <string>
#include <memory>
#include "SPIRV-Cross/spirv_glsl.hpp"
#include "crossc.h"

struct crossc_compiler {
	std::unique_ptr<spirv_cross::CompilerGLSL> c;
	std::string output;
	std::string error;
};

#endif
