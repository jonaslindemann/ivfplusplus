# VBO Test Program

## Purpose
This test program verifies that VBO (Vertex Buffer Object) rendering is properly integrated and working in the GLE library.

## What It Does
- Draws a colored spiral using GLE's polycylinder functions
- Allows toggling between immediate mode (traditional) and VBO rendering
- Provides visual confirmation that both modes produce identical output
- Demonstrates the VBO API usage

## How to Build
The test is automatically built with the other examples when building the project:
```powershell
cd build-release
cmake --build . --config Release
```

## How to Run
```powershell
.\bin\Release\vbo_test.exe
```

## Controls
- **V** - Toggle VBO mode on/off
- **I** - Show current VBO mode status and info
- **Q** or **ESC** - Quit

## What to Look For

### Visual Verification
The spiral should look identical whether VBO mode is enabled or disabled. If you see any visual differences, there may be a bug in the VBO implementation.

### Performance Verification
With VBO enabled, you may notice:
- Smoother animation (higher frame rate)
- Same visual quality
- Reduced CPU usage

### Console Output
The program prints the current VBO mode when you toggle it:
```
VBO Mode: ENABLED
```
or
```
VBO Mode: DISABLED
```

## API Usage Example
This test demonstrates the proper VBO initialization sequence:

```cpp
// Initialize VBO cache (call once at startup)
gleInitVBOCache(10000);  // Pre-allocate for 10K vertices

// Initialize tessellator cache (call once at startup)
gleInitTessCache();

// Enable VBO rendering
gleSetVBOMode(1);  // 1 = enabled, 0 = disabled

// ... draw your GLE geometry ...
glePolyCylinder(num_points, points, colors, radius);

// Cleanup (call at shutdown)
gleDestroyVBOCache();
gleDestroyTessCache();
```

## Troubleshooting

### VBO mode has no effect
1. Check that `gleInitVBOCache()` was called before rendering
2. Verify OpenGL 1.5+ or ARB_vertex_buffer_object extension is available
3. Check console for any error messages

### Program crashes when enabling VBO
1. Ensure `gleInitVBOCache()` is called before `gleSetVBOMode(1)`
2. Check that you're using the latest build of the gleivf library

### No visual output
1. Verify OpenGL and GLUT are properly installed
2. Check that lighting is enabled (the example uses GL_LIGHTING)

## Technical Details

### What Gets Accelerated
When VBO mode is enabled, the following drawing operations use VBOs:
- Segment edge drawing with normals
- Binormal segment edge drawing
- All polycylinder/polycone rendering with normals

### Fallback Behavior
If VBOs are not available or not initialized:
- Functions automatically fall back to immediate mode rendering
- No errors are generated
- Visual output remains identical

### Performance Expectations
VBO rendering typically provides:
- 2-5x performance improvement for complex extrusions
- Better performance with high vertex counts
- Reduced CPU overhead
- Better GPU utilization

## See Also
- `examples/gleextrusion/` - More GLE rendering examples
- `docs/PERFORMANCE_OPTIMIZATION.md` - Performance optimization guide
- `include/GL/gle.h` - VBO API documentation
