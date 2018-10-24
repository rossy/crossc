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

#include <stdexcept>
#include "crossc-priv.h"

uint32_t crossc_version()
{
	return CROSSC_VERSION;
}

bool crossc_has_valid_program(crossc_compiler *comp)
{
	return !!comp->c;
}

void crossc_set_flip_vert_y(crossc_compiler *comp, bool flip_vert_y)
{
	if (!comp->c)
		return;
	auto opts = comp->c->get_common_options();
	opts.vertex.flip_vert_y = flip_vert_y;
	comp->c->set_common_options(opts);
}

const char *crossc_compile(crossc_compiler *comp)
{
	if (!comp->c)
		return nullptr;
	try {
		comp->output = comp->c->compile();
		return comp->output.c_str();
	} catch (const std::exception &e) {
		comp->error = std::string { e.what() };
		return nullptr;
	}
}

void crossc_destroy(crossc_compiler *comp)
{
	if (!comp)
		return;
	delete comp;
}

const char *crossc_strerror(crossc_compiler *comp)
{
	if (comp->error.empty())
		return nullptr;
	return comp->error.c_str();
}
