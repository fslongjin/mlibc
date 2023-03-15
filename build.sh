rm -rf build system

mkdir system
meson --cross=./dragonos-crossfile.txt -Dheaders_only=True  -Ddefault_library=static --prefix=/usr build || exit 1
# meson -Dbuild_tests=true build || exit 1
ROOT_DIR=$(pwd)
export DESTDIR=${ROOT_DIR}/system
cd build && ninja install

cd ${ROOT_DIR}

rm -rf build

meson --cross=./dragonos-crossfile.txt -Dmlibc_no_headers=true -Ddefault_library=static --prefix=/usr build || exit 1
cd build && ninja install

