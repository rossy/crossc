#include "SPIRV-Cross/spirv_hlsl.hpp"
#include "crossc-priv.h"

namespace {
	auto get_hlsl(crossc_compiler *comp)
	{
		return (spirv_cross::CompilerHLSL*)comp->c.get();
	}
}

crossc_compiler *crossc_hlsl_create(const uint32_t *words, size_t word_count)
{
	crossc_compiler *comp = new crossc_compiler;
	try {
		comp->c = std::make_unique<spirv_cross::CompilerHLSL>(words, word_count);
	} catch (const std::exception &e) {
		comp->error = std::string { e.what() };
	}
	return comp;
}

void crossc_hlsl_set_shader_model(crossc_compiler *comp, int model)
{
	if (!comp->c)
		return;
	auto hlsl = get_hlsl(comp);
	auto opts = hlsl->get_hlsl_options();
	opts.shader_model = model;
	hlsl->set_hlsl_options(opts);
}
