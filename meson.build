project(
  'OpenGL',
  'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++17',
    'buildtype=debug',
    'b_pch=false',
  ],
)

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language: 'cpp')
endif

glad_dep = dependency('glad', fallback: ['glad', 'glad_dep'])

glm_dep = dependency('glm', fallback: ['glm', 'glm_dep'])

cmake = import('cmake')
glfw_opts = cmake.subproject_options()
glfw_opts.add_cmake_defines(
  {
    'GLFW_BUILD_EXAMPLES': 'OFF',
    'GLFW_BUILD_TESTS': 'OFF',
    'GLFW_BUILD_DOCS': 'OFF',
  },
)
glfw_proj = cmake.subproject('glfw', options: glfw_opts)
glfw_dep = glfw_proj.dependency('glfw')

assimp_proj = cmake.subproject('assimp')
assimp_dep = assimp_proj.dependency('assimp')

common_deps = [glad_dep, glfw_dep, glm_dep, assimp_dep]
inc = include_directories('r-include')

fs = import('fs')

subdir('r-src')

subdir('p-src')

foreach source_file : p_sources
  base_name = fs.stem(source_file)

  executable(
    base_name,
    source_file,
    link_with: common_lib,
    include_directories: inc,
    dependencies: common_deps,
  )
endforeach
