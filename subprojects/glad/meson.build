project('glad', 'c')

inc = include_directories('include')

glad_lib = static_library('glad', 'src/glad.c', include_directories: inc)

glad_dep = declare_dependency(link_with: glad_lib, include_directories: inc)
