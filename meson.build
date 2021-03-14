project('multiload',
    ['c', 'cpp'],
    default_options : ['cpp_std=c++17'])

multiload_inc = include_directories('include')

dl_dep = meson.get_compiler('c').find_library('dl')

multiload_lib = library('multiload',
    'src/multiload.cpp',
    include_directories : multiload_inc,
    dependencies : dl_dep,
    install : true)

install_headers('include/multiload.h')

multiload_dep = declare_dependency(
    link_with : multiload_lib,
    include_directories : multiload_inc)

if get_option('enable_tests')
    subdir('test')
endif
