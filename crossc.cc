#include <cstdlib>
#include <cstring>
#include "SPIRV-Cross/spirv_glsl.hpp"
#include "crossc-priv.h"

void crossc_set_flip_vert_y(crossc_compiler *comp, bool flip_vert_y)
{
	auto glsl = (spirv_cross::CompilerGLSL*)comp->c;
	auto opts = glsl->get_options();
	opts.vertex.flip_vert_y = flip_vert_y;
	glsl->set_options(opts);
}

char *crossc_compile(struct crossc_compiler *comp)
{
	auto str = comp->c->compile();
	char *c_str = reinterpret_cast<char*>(std::malloc(str.size() + 1));
	std::memcpy(c_str, str.c_str(), str.size() + 1);
	return c_str;
}

void crossc_destroy(struct crossc_compiler *comp)
{
	if (!comp)
		return;
	delete comp->c;
	std::free(comp);
}
