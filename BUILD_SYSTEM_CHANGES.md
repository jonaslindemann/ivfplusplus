# Build System Changes

This document describes the modernization of the Ivf++ build system to match the ivf2 library structure.

## Key Changes

### 1. Automatic vcpkg Detection

The build system now automatically detects vcpkg from multiple locations:

- **VCPKG_ROOT environment variable** (recommended)
- **Windows**: `c:/vcpkg` or `e:/vcpkg`
- **Linux/macOS**: `$HOME/vcpkg` or `/usr/local/vcpkg`

No need to specify toolchain file manually in most cases!

#### Setting VCPKG_ROOT (Recommended)

**Windows (Command Prompt):**
```cmd
set VCPKG_ROOT=C:\path\to\vcpkg
```

**Windows (PowerShell):**
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
```

**Linux/macOS:**
```bash
export VCPKG_ROOT=/path/to/vcpkg
```

### 2. Unified Interface Library

A new `ivfplusplus::ivfplusplus` interface library bundles all Ivf++ libraries and dependencies for easier linking:

```cmake
# In your CMakeLists.txt
target_link_libraries(my_app PRIVATE ivfplusplus::ivfplusplus)
```

This automatically includes:
- Core libraries (ivf, ivfmath, ivfctl, ivffile, etc.)
- All dependencies (OpenGL, GLEW, PNG, JPEG, etc.)
- Proper include directories

### 3. Simplified Example Building

The new `add_ivfplusplus_example()` helper function simplifies example creation:

```cmake
# Simple example
add_ivfplusplus_example(my_example 
    LIBS ivf ivfglut glad
    DEBUG_LIBS ivfd ivfglutd gladd
)

# With multiple source files
add_ivfplusplus_example(complex_example 
    SOURCES main.cpp utils.cpp
    LIBS ivf ivfglut glad
    DEBUG_LIBS ivfd ivfglutd gladd
)
```

### 4. Automatic Debug Postfix

All targets now automatically append 'd' suffix in debug builds:
- `ivf.lib` → `ivfd.lib`
- `example.exe` → `exampled.exe`

### 5. Updated CMake Presets

CMakePresets.json now includes:
- `default`: Release build (auto-detects vcpkg)
- `debug`: Debug build with IVF_DEBUG=ON
- `linux`: Linux release build
- `linux-debug`: Linux debug build

No hardcoded paths - relies on automatic detection!

### 6. Python Scripts are Optional

The Python utilities in `utils/` are **optional** and not required for building:
- `ivfappgen.py`: Generate application templates
- `ivfclassgen.py`: Generate class boilerplate
- `update_license.py`: Update license headers

Use them if needed: `python utils/ivfappgen.py`

## Building

### Quick Start (Windows)

```cmd
# Option 1: Using the build script
ivfbuild.cmd

# Option 2: Manual CMake
cmake -B build-debug -S . -G "Visual Studio 17 2022"
cmake --build build-debug --config Debug

cmake -B build-release -S . -G "Visual Studio 17 2022"
cmake --build build-release --config Release
```

### Quick Start (Linux/macOS)

```bash
# Configure
cmake -B build-debug -S . -DCMAKE_BUILD_TYPE=Debug
cmake -B build-release -S . -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build-debug
cmake --build build-release
```

## Build Output

- **Libraries**: `lib/Debug/` and `lib/Release/`
  - Core: `ivf[d].lib`, `ivfmath[d].lib`, etc.
  - Note: 'd' suffix indicates debug builds

- **Examples**: `bin/Debug/` and `bin/Release/`

## Migration Guide

### For Existing Applications

If you have existing applications using Ivf++:

**Before:**
```cmake
target_link_libraries(my_app 
    ivf ivfmath ivfctl glad 
    PNG::PNG JPEG::JPEG ZLIB::ZLIB 
    OpenGL::GL GLEW::GLEW
)
```

**After:**
```cmake
target_link_libraries(my_app PRIVATE ivfplusplus::ivfplusplus)
```

### For Custom vcpkg Locations

If vcpkg is in a non-standard location:

```bash
# Set environment variable
export VCPKG_ROOT=/custom/path/to/vcpkg

# Or pass directly to CMake
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/custom/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Troubleshooting vcpkg Issues

**Issue**: CMake reports "Could NOT find PNG/ZLIB/JPEG" even though vcpkg is detected

**Cause**: Visual Studio's bundled vcpkg was detected but doesn't have packages installed

**Solution**: Install a local vcpkg instance (recommended):

```cmd
# Windows
git clone https://github.com/Microsoft/vcpkg.git c:\vcpkg
cd c:\vcpkg
.\bootstrap-vcpkg.bat
set VCPKG_ROOT=c:\vcpkg
vcpkg install zlib libpng libjpeg-turbo glew freeglut glfw3 fltk --triplet=x64-windows
```

```bash
# Linux/macOS
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg
vcpkg install zlib libpng libjpeg-turbo glew freeglut glfw3 fltk
```

## Benefits

1. **No Python required** for building the library
2. **Automatic vcpkg detection** - no manual configuration
3. **Simplified linking** with interface library
4. **Consistent debug/release naming** with automatic postfix
5. **Modern CMake practices** matching ivf2
6. **Cross-platform** build configuration

## Compatibility

- CMake 3.20 or higher required
- Visual Studio 2022 (Windows)
- GCC/Clang (Linux/macOS)
- vcpkg for dependency management
