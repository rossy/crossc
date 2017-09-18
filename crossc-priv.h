#ifndef CROSSC_PRIV_H_

#include "crossc.h"
#include "SPIRV-Cross/spirv_cross.hpp"

struct crossc_compiler {
	spirv_cross::Compiler *c;
};

#endif
