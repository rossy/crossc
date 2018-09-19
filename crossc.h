#ifndef CROSSC_H_
#define CROSSC_H_

typedef struct crossc_compiler crossc_compiler;

#ifdef __cplusplus
#include <cstddef>
#include <cinttypes>
using std::size_t;
using std::uint32_t;
extern "C" {
#else
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#endif

#define CROSSC_VERSION_MAJOR 1
#define CROSSC_VERSION_MINOR 5
#define CROSSC_VERSION_PATCH 0

#define CROSSC_MAKE_VERSION(maj, min, pat) (((uint32_t)(maj) << 24) | \
                                            ((uint32_t)(min) << 16) | \
                                             (uint32_t)(pat))

// Use this to get the version of crossc that has been compiled against
#define CROSSC_VERSION CROSSC_MAKE_VERSION(CROSSC_VERSION_MAJOR, \
                                           CROSSC_VERSION_MINOR, \
                                           CROSSC_VERSION_PATCH)

#define CROSSC_VERSION_GET_MAJOR(ver) ((ver) >> 24)
#define CROSSC_VERSION_GET_MINOR(ver) (((ver) >> 16) & 0xff)
#define CROSSC_VERSION_GET_PATCH(ver) ((ver) & 0xffff)

typedef enum crossc_glsl_profile {
	CROSSC_GLSL_PROFILE_CORE,
	CROSSC_GLSL_PROFILE_ES,
} crossc_glsl_profile;

// Use this to get the version of crossc that has been linked against
uint32_t crossc_version(void);

// Create a new instance of the compiler targeting GLSL. words should point to
// a valid SPIR-V binary, which is copied and parsed. After creation,
// crossc_has_valid_program() should be called to check if the SPIR-V binary
// was parsed correctly.
crossc_compiler *crossc_glsl_create(const uint32_t *words, size_t word_count);

// Set the target GLSL version. The format is the same as specified in a
// #version directive. Profile should be one of the crossc_glsl_profile values.
void crossc_glsl_set_version(crossc_compiler *comp, int version,
                             crossc_glsl_profile profile);

// Create a new instance of the compiler targeting HLSL. words should point to
// a valid SPIR-V binary, which is copied and parsed. After creation,
// crossc_has_valid_program() should be called to check if the SPIR-V binary
// was parsed correctly.
crossc_compiler *crossc_hlsl_create(const uint32_t *words, size_t word_count);

// Set the target HLSL shader model. The format is major*10 + minor, so 50 for
// a ps_5_0 shader.
void crossc_hlsl_set_shader_model(crossc_compiler *comp, int model);

// Invert gl_Position.y. Only valid for a vertex shader.
void crossc_set_flip_vert_y(crossc_compiler *comp, bool flip_vert_y);

// Returns true if the SPIR-V binary passed to crossc_*_create() was parsed
// correctly. If this returns false, crossc_strerror() can be used to get an
// error message.
bool crossc_has_valid_program(crossc_compiler *comp);

// Translate the input SPIR-V to source code in the target language. On
// success, this will return the translated source code, which is valid until
// the crossc_compiler is destroyed or until crossc_compile() is called again.
// Returns NULL on failure, in which case crossc_strerror() can be used to get
// an error message.
const char *crossc_compile(crossc_compiler *comp);

// Free all resources associated with a compiler instance. If comp is NULL,
// does nothing.
void crossc_destroy(crossc_compiler *comp);

// If crossc_has_valid_program() returned false or crossc_compile() returned
// NULL, this will return an error message. The error message string is valid
// until the crossc_compiler is destroyed or until another crossc function is
// called that generates an error message.
const char *crossc_strerror(crossc_compiler *comp);

#ifdef __cplusplus
}
#endif

#endif
