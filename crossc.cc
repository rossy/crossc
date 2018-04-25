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
