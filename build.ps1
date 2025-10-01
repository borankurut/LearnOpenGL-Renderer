meson setup build --backend ninja --buildtype=release
meson compile -C build

copy ./shaders ./build/ -Recurse -Force
copy ./textures ./build/ -Recurse -Force
copy ./models ./build/ -Recurse -Force
copy ./build/subprojects/assimp/libassimp.dll ./build/ -Recurse -Force
