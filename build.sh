rm -rf build
meson -Dbuild_tests=true build || exit 1
cd build && meson test -v