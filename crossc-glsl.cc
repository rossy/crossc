#include "SPIRV-Cross/spirv_glsl.hpp"
#include "crossc-priv.h"

crossc_compiler *crossc_glsl_create(const uint32_t *words, size_t word_count)
{
	crossc_compiler *comp = new crossc_compiler;
	try {
		comp->c = std::make_unique<spirv_cross::CompilerGLSL>(words, word_count);
	} catch (const std::exception &e) {
		comp->error = std::string { e.what() };
	}
	return comp;
}

void crossc_glsl_set_version(crossc_compiler *comp, int version,
                             crossc_glsl_profile profile)
{
	if (!comp->c)
		return;
	auto glsl = comp->c.get();
	auto opts = glsl->get_common_options();
	opts.version = version;
	opts.es = (profile == CROSSC_GLSL_PROFILE_ES);
	glsl->set_common_options(opts);
}
