/*
 * Copyright 2018, crossc contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
