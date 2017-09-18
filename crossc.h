#ifndef CROSSC_H_
#define CROSSC_H_

#include <inttypes.h>
#include <stddef.h>

typedef struct crossc_compiler crossc_compiler;

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

// Create a new instance of the compiler targeting HLSL. words should point to
// a valid SPIR-V binary, which is copied and parsed.
crossc_compiler *crossc_hlsl_create(uint32_t *words, size_t words_len);

// Set the target HLSL shader model. The format is major*10 + minor, so 50 for
// a ps_5_0 shader.
void crossc_hlsl_set_shader_model(crossc_compiler *comp, int model);

// Invert gl_Position.y. Only valid for a vertex shader.
void crossc_set_flip_vert_y(crossc_compiler *comp, bool flip_vert_y);

// Translate the input SPIR-V to source code in the target language. On
// success, this will return the source as a malloc() allocated string, which
// must be freed by the caller. On failure, returns NULL, in which case,
// crossc_strerror() can be used to get an error message.
char *crossc_compile(crossc_compiler *comp);

// Free all resources associated with a compiler instance
void crossc_destroy(crossc_compiler *comp);

// If crossc_compile() returned NULL, this will return an error message. The
// error message string is valid for the lifetime of the crossc_compiler.
const char *crossc_strerror(crossc_compiler *comp);

#ifdef __cplusplus
}
#endif

#endif
