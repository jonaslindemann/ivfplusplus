# Building Ivf++

The build system has been modernized to use CMake multi-config generators, eliminating the need for special build scripts and CMAKE_BUILD_TYPE.

## Prerequisites

- CMake 3.20 or later
- vcpkg for dependency management (set `VCPKG_ROOT` environment variable or install to `c:/vcpkg` on Windows or `~/vcpkg` on Linux)
- A C++ compiler (MSVC, GCC, or Clang)
- Ninja (recommended) or Visual Studio

## Quick Start

### Windows (Ninja Multi-Config)

```cmd
# Configure once (vcpkg auto-detected)
cmake -B build -G "Ninja Multi-Config"

# Or use preset if you prefer
cmake --preset default

# Build Debug
cmake --build build --config Debug

# Build Release  
cmake --build build --config Release

# Or build both
cmake --build build --config Debug
cmake --build build --config Release
```

### Windows (Visual Studio)

```cmd
# Configure once (vcpkg auto-detected, Visual Studio is multi-config by default)
cmake -B build

# Or use preset
cmake --preset vs2022

# Build Debug
cmake --build build --config Debug

# Build Release
cmake --build build --config Release
```

### Linux

```bash
# Configure once (vcpkg auto-detected)
cmake -B build -G "Ninja Multi-Config"

# Or use preset if you prefer
cmake --preset linux

# Build Debug
cmake --build build --config Debug

# Build Release
cmake --build build --config Release
```

## vcpkg Detection

CMake will automatically detect vcpkg from:

1. `VCPKG_ROOT` environment variable (recommended)
2. `c:/vcpkg` on Windows
3. `~/vcpkg` on Linux/macOS

Alternatively, you can specify it manually:

```cmd
cmake -B build -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

## Build Outputs

Libraries will be built with automatic debug postfix:

- Release: `ivf.lib`, `ivfmath.lib`, etc.
- Debug: `ivfd.lib`, `ivfmathd.lib`, etc.

Executables and libraries are placed in:

- `bin/` - executables
- `lib/` - libraries

## Build Options

You can customize the build with these options:

```cmd
cmake --preset default -DIVF_EXAMPLES=OFF -DIVF_UI_FLTK=OFF
```

Available options:

- `IVF_UI` - Build user interface integration libraries (default: ON)
- `IVF_UI_FLTK` - Build FLTK integration (default: ON)
- `IVF_UI_WIN32` - Build Win32 integration (default: OFF)
- `IVF_UI_GLUT` - Build GLUT integration (default: ON)
- `IVF_IMAGE` - Build image support (default: ON)
- `IVF_FONT` - Build font support (default: OFF)
- `IVF_EXAMPLES` - Build examples (default: ON)
- `IVF_SHARED` - Build shared libraries (default: OFF)

## Installing

```cmd
cmake --install build --config Release
cmake --install build --config Debug
```

## Notes

- The build system now properly supports multi-configuration generators
- Debug and Release builds can coexist in the same build directory
- No need to specify CMAKE_BUILD_TYPE at configure time
- The old build scripts (ivfbuild, ivfbuild.cmd) are no longer needed
