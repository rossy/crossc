#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "crossc-priv.h"

void crossc_set_flip_vert_y(crossc_compiler *comp, bool flip_vert_y)
{
	auto opts = comp->c->get_options();
	opts.vertex.flip_vert_y = flip_vert_y;
	comp->c->set_options(opts);
}

char *crossc_compile(crossc_compiler *comp)
{
	try {
		auto str = comp->c->compile();
		char *c_str = (char*)std::malloc(str.size() + 1);
		std::memcpy(c_str, str.c_str(), str.size() + 1);
		return c_str;

	} catch (const std::exception &e) {
		size_t what_len = std::strlen(e.what()) + 1;
		char *c_str = (char*)std::malloc(what_len);
		std::memcpy(c_str, e.what(), what_len);
		comp->error = c_str;

		return nullptr;
	}
}

void crossc_destroy(crossc_compiler *comp)
{
	if (!comp)
		return;
	if (comp->c)
		delete comp->c;
	std::free(comp->error);
	std::free(comp);
}

const char *crossc_strerror(crossc_compiler *comp)
{
	return comp->error;
}
