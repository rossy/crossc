#ifndef CROSSC_H_

#include <inttypes.h>
#include <stddef.h>

typedef struct crossc_compiler crossc_compiler;

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

crossc_compiler *crossc_hlsl_create(uint32_t *words, size_t words_len);
void crossc_hlsl_set_shader_model(crossc_compiler *comp, int model);

void crossc_set_flip_vert_y(crossc_compiler *comp, bool flip_vert_y);

char *crossc_compile(crossc_compiler *comp);
void crossc_destroy(crossc_compiler *comp);

#ifdef __cplusplus
}
#endif

#endif
