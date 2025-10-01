common_sources = files(
  'camera.cpp',
  'gl_debug.cpp',
  'light.cpp',
  'model.cpp',
  #'post_process.cpp',
  'particle_system.cpp',
  'render_target.cpp',
  'screen_quad.cpp',
  'shader.cpp',
  'texture.cpp',
  'utils.cpp',
)

common_lib = static_library(
  'common',
  common_sources,
  include_directories: inc,
  dependencies: common_deps,
)
