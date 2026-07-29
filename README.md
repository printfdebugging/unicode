# unicode

A small C library for encoding/decoding UTF-8. The API is quite simple
and well documented, have a look at [unicode/unicode.h](./include/unicode/unicode.h).


# Build Instructions

This library doesn't use anything but the C standard library. So you should be
able to just include `unicode.c` and `unicode.h` in your projects without much
hassle.

> [!NOTE]
> If you want to add some other build system, like meson, or autotools, you
> are welcome to create a PR for that.

## Building with CMake
```bash
cmake -B build
cmake --build build
```