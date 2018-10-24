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

#ifndef CROSSC_PRIV_H_
#define CROSSC_PRIV_H_

#include <string>
#include <memory>
#include "SPIRV-Cross/spirv_glsl.hpp"
#include "crossc.h"

struct crossc_compiler {
	std::unique_ptr<spirv_cross::CompilerGLSL> c;
	std::string output;
	std::string error;
};

#endif
