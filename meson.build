project('crossc', 'cpp',
    version : '1.4.0',
    default_options : ['warning_level=3', 'cpp_std=c++14']
)

if not meson.is_subproject()
    error('These build rules are for use as a subproject only.')
endif

crossc_sources = files(
    'crossc-glsl.cc',
    'crossc-hlsl.cc',
    'crossc.cc',
)

_spirvcross_sources = [
    'spirv_cfg.cpp',
    'spirv_cpp.cpp',
    'spirv_cross.cpp',
    'spirv_cross_util.cpp',
    'spirv_glsl.cpp',
    'spirv_hlsl.cpp',
    'spirv_msl.cpp',
    'spirv_reflect.cpp',
]

spirvcross_sources = files()
foreach file : _spirvcross_sources
    spirvcross_sources += files(join_paths('SPIRV-Cross', file))
endforeach

crossc_lib = static_library('crossc',
    spirvcross_sources, crossc_sources,
    install : false,
    pic : true
)

crossc_dep = declare_dependency(
    include_directories : include_directories('.'),
    link_with : crossc_lib,
    version : meson.project_version()
)
