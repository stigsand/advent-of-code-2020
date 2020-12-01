# [Advent of Code 2020](https://adventofcode.com/2020) - C++ Solutions

## Build Requirements for Linux
* [Conan](https://conan.io/) version 1.24 or higher
* [CMake](https://cmake.org/) version 3.18 or higher
* C++ compiler supporting the C++17 standard (tested with GCC 9.2.1)

## Linux Build Instructions
Below `SOURCE_DIR` is the path to the `c++` subdirectory of the checked-out repository, and `BUILD_DIR` is an arbitrary, empty directory.
```bash
conan install \
    --build missing \
    --install-folder "${BUILD_DIR}" \
    "${SOURCE_DIR}"
cmake \
    -G "Ninja" \
    -D CMAKE_BUILD_TYPE:STRING=Debug \
    -D CMAKE_TOOLCHAIN_FILE:FILEPATH=conan_paths.cmake \
    -S "${SOURCE_DIR}" \
    -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}" --target all
cd "${BUILD_DIR}" && ctest
```
The last command above runs a CTest project verifying the puzzle answers.
