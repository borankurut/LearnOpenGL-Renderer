meson setup build_debug --backend vs --buildtype=debug
meson compile -C build_debug

copy ./shaders ./build_debug/ -Recurse -Force
copy ./textures ./build_debug/ -Recurse -Force
copy ./models ./build_debug/ -Recurse -Force
copy ./build_debug/subprojects/assimp/libassimp.dll ./build_debug/ -Recurse -Force
