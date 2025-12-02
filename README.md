# Ivf++ - Interactive Visualization Framework

Ivf++ is a C++ wrapper library for OpenGL designed to simplify 3D graphics programming. The library provides an object-oriented interface to OpenGL with focus on ease of use, extendibility, reusability, and portability.

## Features

- Object-oriented OpenGL wrapper
- Scene graph management
- Multiple UI integrations (FLTK, GLUT, Win32)
- Image loading (JPEG, PNG, TGA, BMP)
- 3D file format support (Ac3D, DXF)
- Geometry manipulation and culling
- Lighting and materials
- Texture mapping
- Event handling system

## Build System (Modern - 2024)

The build system has been modernized to match the ivf2 library structure with automatic dependency management via vcpkg.

### Prerequisites

- **CMake 3.20 or higher**
- **vcpkg** for dependency management ([Installation Guide](https://learn.microsoft.com/vcpkg/get_started/overview))
- **C++ Compiler**:
  - Windows: Visual Studio 2022
  - Linux: GCC/Clang
  - macOS: Clang

### Dependencies (Automatically managed by vcpkg)

- OpenGL
- GLEW
- FreeGLUT
- GLFW3
- FLTK
- PNG
- JPEG
- ZLIB
- GLE (tubing and extrusion)

### vcpkg Setup

The build system automatically detects vcpkg from:

1. **VCPKG_ROOT environment variable** (recommended)
2. **Windows**: `c:/vcpkg` or `e:/vcpkg`
3. **Linux/macOS**: `$HOME/vcpkg` or `/usr/local/vcpkg`
4. **Fallback**: Visual Studio bundled vcpkg (requires manual package installation)

> **⚠️ Important**: If using Visual Studio's bundled vcpkg, you must install packages manually:
> ```cmd
> vcpkg install zlib libpng libjpeg-turbo glew freeglut glfw3 fltk --triplet=x64-windows
> ```
> **Recommended**: Install vcpkg locally at `c:/vcpkg` - it will be detected automatically and packages can be installed once.

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

### Building

#### Quick Start (Windows)

```cmd
# Using the build script (builds both debug and release)
ivfbuild.cmd

# Or manually with CMake
cmake -B build-debug -S . -G "Visual Studio 17 2022"
cmake --build build-debug --config Debug

cmake -B build-release -S . -G "Visual Studio 17 2022"
cmake --build build-release --config Release
```

#### Quick Start (Linux/macOS)

```bash
# Using the build script (builds both debug and release)
./ivfbuild

# Or manually with CMake
cmake -B build-debug -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug

cmake -B build-release -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

### Build Output

- **Libraries**: `lib/Debug/` and `lib/Release/`
  - Core: `ivf[d].lib`, `ivfmath[d].lib`, `ivfctl[d].lib`, etc.
  - Note: 'd' suffix indicates debug builds

- **Examples**: `bin/Debug/` and `bin/Release/`

### Build Options

Configure build options with CMake:

```cmake
option(IVF_UI "Build user interface integration libraries." ON) 
option(IVF_UI_FLTK "Build FLTK integration." ON)
option(IVF_UI_WIN32 "Build Win32 integration." OFF)
option(IVF_UI_GLUT "Build GLUT integration." ON)
option(IVF_IMAGE "Build image support." ON)
option(IVF_FONT "Build font support." OFF)
option(IVF_EXAMPLES "Build examples." ON)
option(IVF_DEBUG "Build with debug information." OFF)
option(IVF_SHARED "Build as shared libraries." OFF)
```

Example:
```bash
cmake -B build -S . -DIVF_EXAMPLES=OFF -DIVF_UI_WIN32=ON
```

### Troubleshooting

**Problem**: CMake fails with "Could NOT find PNG/ZLIB/JPEG" errors

**Solution**: Your vcpkg installation doesn't have the required packages. Options:

1. **Use local vcpkg (recommended)**:
   ```cmd
   # Install vcpkg locally
   git clone https://github.com/Microsoft/vcpkg.git c:\vcpkg
   cd c:\vcpkg
   .\bootstrap-vcpkg.bat
   
   # Set environment variable
   set VCPKG_ROOT=c:\vcpkg
   
   # Install packages
   vcpkg install zlib libpng libjpeg-turbo glew freeglut glfw3 fltk --triplet=x64-windows
   
   # Now configure ivf++
   cd c:\Users\Jonas Lindemann\Development\ivfplusplus
   cmake -B build-debug -S . -G "Visual Studio 17 2022"
   ```

2. **Use VS bundled vcpkg**: Install packages manually in the VS vcpkg location, or better yet, use option 1.

## Using Ivf++ in Your Projects

### CMake Integration

The new unified interface library makes linking simple:

```cmake
# Find or add Ivf++ to your project
add_subdirectory(path/to/ivfplusplus)

# Link to your application
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ivfplusplus::ivfplusplus)
```

This automatically includes all necessary libraries and dependencies!

### Legacy Method (Still Supported)

```cmake
target_link_libraries(my_app 
    ivf ivfmath ivfctl ivfglut glad
    PNG::PNG JPEG::JPEG ZLIB::ZLIB 
    OpenGL::GL GLEW::GLEW FreeGLUT::freeglut
)
```

## Python Utilities (Optional)

Python scripts in `utils/` are **optional** and not required for building:
- `ivfappgen.py`: Generate application templates
- `ivfclassgen.py`: Generate class boilerplate
- `update_license.py`: Update license headers

Use them if needed:
```bash
python utils/ivfappgen.py
```

## Documentation

- **Build System Changes**: See [BUILD_SYSTEM_CHANGES.md](BUILD_SYSTEM_CHANGES.md)
- **Examples**: Located in `examples/` directory
- **Legacy README**: See [README](README) file

## Platform Support

- Windows (Visual Studio 2022)
- Linux (GCC/Clang)
- macOS (Clang)

Tested on:
- Windows 11 with Visual Studio 2022
- Ubuntu 20.04+ with GCC
- macOS 12+ with Xcode

## License

Ivf++ is available under the terms of the GNU Library General Public License, Version 2 (LGPL) with exceptions that allow:

1. Static linking without requiring application source disclosure
2. Subclassing without creating derivative works
3. Platform-specific modifications without LGPL requirements

See [LICENSE](LICENSE) and [COPYING](COPYING) for details.

## Copyright

Ivf++ is copyright (C) 1999-2024 by Structural Mechanics, Lund University

## Authors

See [AUTHORS](AUTHORS) file for contributor information.

## Contributing

Contributions are welcome! Please send contributions to the project maintainers.

## Acknowledgments

- IvfTgaImage and IvfBmpImage are copyright (C) 2002- John Aspras
- Uses the GLE (tubing and extrusion) library
- Uses FLTK, GLUT, and other open-source libraries

## Links

- Project Homepage: [Ivf++ on SourceForge](http://sourceforge.net/projects/ivfplusplus)
- Related Project: [ivf2](https://github.com/jonaslindemann/ivf2) - Next-generation version
