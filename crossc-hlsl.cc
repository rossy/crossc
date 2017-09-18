#include <cstdlib>
#include "SPIRV-Cross/spirv_hlsl.hpp"
#include "crossc-priv.h"

crossc_compiler *crossc_hlsl_create(uint32_t *words, size_t words_len)
{
	crossc_compiler *comp = (crossc_compiler*)std::calloc(sizeof *comp, 1);

	std::vector<uint32_t> wordsv { words, words + words_len };
	comp->c = new spirv_cross::CompilerHLSL(std::move(wordsv));
	return comp;
}

void crossc_hlsl_set_shader_model(crossc_compiler *comp, int model)
{
	auto hlsl = (spirv_cross::CompilerHLSL*)comp->c;
	auto opts = hlsl->get_options();
	opts.shader_model = model;
	hlsl->set_options(opts);
}
