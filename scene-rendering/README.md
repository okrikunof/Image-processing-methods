# Scene Rendering Module

## Overview

This module is a complete 3D scene ray tracer built using **Intel Embree v4.4.0**, a high-performance ray tracing library. It renders realistic images with multiple light sources, shadows, reflections, and physically-based materials.

## Features

- **Hardware-Accelerated Ray Tracing**: Uses Intel Embree for optimal performance
- **Multiple Geometry Types**: Supports triangles, cubes, and sphere primitives
- **Realistic Lighting**: Point and directional light sources
- **Phong Shading**: Diffuse and specular reflection with controllable parameters
- **Recursive Reflections**: Supports reflective materials with configurable depth
- **Shadow Calculation**: Accurate shadow casting and occlusion testing
- **PPM Image Output**: Generates standard PPM format images

## File Structure

```
scene-rendering/
├── main.cpp           # Complete ray tracer implementation
└── CMakeLists.txt     # Build configuration with Embree
```

## Dependencies

### Required

- **[Intel Embree](https://github.com/RenderKit/embree)** v4.4.0 or higher
- C++20 compiler
- CMake >= 3.20

### Installing Embree

**Ubuntu/Debian:**
```bash
# Option 1: Build from source (recommended for latest version)
git clone https://github.com/RenderKit/embree.git
cd embree
mkdir build && cd build
cmake ..
make -j
sudo make install

# Option 2: Use package manager (may be older version)
sudo apt-get install libembree-dev
```

**macOS:**
```bash
brew install embree
```

**Windows:**
Download pre-built binaries from:
https://github.com/RenderKit/embree/releases

## Building

```bash
cd scene-rendering
mkdir build
cd build
cmake ..
make
```

**Note**: If CMake cannot find Embree, specify its location:
```bash
cmake -Dembree_DIR=/path/to/embree/lib/cmake/embree-4.4.0 ..
```

## Usage

Simply run the executable to render the scene:

```bash
./scene-rendering
```

The program will:
1. Initialize the Embree ray tracing device
2. Create the 3D scene with geometry and materials
3. Set up lights and camera
4. Render the image (800x800 pixels)
5. Save the result to `output.ppm`

### Viewing the Output

The output is a PPM (Portable Pixmap) image file. To view it:

**Using ImageMagick:**
```bash
convert output.ppm output.png
display output.png  # or open output.png on macOS
```

**Using GIMP:**
```bash
gimp output.ppm
```

**Online PPM Viewer:**
Upload `output.ppm` to any online PPM viewer.

## Scene Configuration

The scene is defined in the `main()` function. You can customize:

### Camera Settings

```cpp
Vector3D eye(1, 2, 5);        // Camera position
Vector3D center(1, 2, 0);     // Look-at point
Vector3D up(0, 1, 0);         // Up direction
double distance = 8.0;         // Distance to screen
double screen_width = 15.0;    // Virtual screen width
double screen_height = 15.0;   // Virtual screen height
int image_width = 800;         // Output image width
int image_height = 800;        // Output image height
```

### Materials

Materials use the Phong reflection model:

```cpp
Material myMaterial = {
    Color(r, g, b),      // Base color (diffuse)
    0.7,                 // Diffuse coefficient (0.0-1.0)
    0.3,                 // Specular coefficient
    10.0,                // Specular exponent (shininess)
    Color(1, 1, 1),      // Specular highlight color
    0.1                  // Reflectivity (0.0 = none, 1.0 = mirror)
};
```

### Light Sources

**Point Light** (omnidirectional):
```cpp
PointLight plight(
    Vector3D(1, 3, 3),      // Position
    Color(200, 200, 200)    // Intensity (RGB)
);
lights.push_back(&plight);
```

**Directional Light** (like sunlight):
```cpp
DirectionalLight dlight(
    Vector3D(-1, -1, -1),   // Direction
    Color(200, 200, 200)    // Intensity (RGB)
);
lights.push_back(&dlight);
```

### Adding Geometry

**Floor (Triangles):**
```cpp
RTCGeometry floor = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
float vertices[] = {
    -20, 0, -20,
    20, 0, -20,
    20, 0, 20,
    -20, 0, 20
};
unsigned indices[] = {0, 2, 1, 0, 3, 2};
// Set buffers and commit...
rtcSetGeometryUserData(floor, &floorMaterial);
```

**Cube:**
```cpp
RTCGeometry cube = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
float cubeVertices[] = {
    // 8 vertices defining a cube
    // ... vertex coordinates ...
};
unsigned cubeIndices[] = {
    // 12 triangles (2 per face × 6 faces)
    // ... triangle indices ...
};
// Set buffers and commit...
rtcSetGeometryUserData(cube, &cubeMaterial);
```

## Rendering Pipeline

The ray tracer follows this process for each pixel:

1. **Ray Generation**: Calculate ray direction through pixel
2. **Ray-Scene Intersection**: Use Embree to find closest hit
3. **Shading**: If hit found, compute color:
   - Get hit point, normal, and material
   - For each light source:
     - Check if light is occluded (shadow)
     - Calculate diffuse component (Lambert)
     - Calculate specular component (Phong)
     - Accumulate contribution
   - If material is reflective:
     - Cast reflection ray recursively
     - Add reflected color scaled by reflectivity
4. **Output**: Write final color to image buffer

## Algorithm Details

### Phong Reflection Model

**Diffuse Component:**
```
diffuse = material.color × material.diffuse × max(0, N·L)
```

**Specular Component:**
```
specular = material.specular_color × material.specular × (H·N)^exponent
```

Where:
- N = surface normal
- L = direction to light
- H = half-vector between view and light direction
- exponent = shininess parameter

### Recursive Reflections

```cpp
if (material->reflectivity > 0.0) {
    Vector3D reflectedDir = incident - normal * (2.0 * (normal·incident));
    Color reflectedColor = shade(reflectedRay, scene, lights, reflectedDir, depth + 1);
    totalColor = totalColor + reflectedColor * material->reflectivity;
}
```

Maximum recursion depth is set to 50 to prevent infinite loops.

### Shadow Calculation

Embree's occlusion testing is used to determine if a light is blocked:

```cpp
bool isOccluded(const Vector3D &point, RTCScene scene) {
    RTCRay ray;
    // Set ray from point towards light
    rtcOccluded1(scene, &ray);
    return ray.tfar < 0;  // Occluded if ray was terminated
}
```

## Performance Considerations

- **Image Resolution**: Higher resolution = longer render time (quadratic)
- **Reflection Depth**: More bounces = exponentially longer
- **Geometry Complexity**: Embree efficiently handles millions of primitives
- **Light Count**: Linear impact on render time

**Typical render times** (800x800, 2 lights, 5 objects):
- CPU: 10-60 seconds depending on processor

## Customization Examples

### Example 1: Add a Mirror Sphere

```cpp
Material mirrorMaterial = {
    Color(0.9, 0.9, 0.9),  // Slight color tint
    0.1,                    // Low diffuse
    50.0,                   // High specular
    200.0,                  // Very sharp highlights
    Color(1, 1, 1),         // White specular
    0.9                     // High reflectivity (mirror)
};

RTCGeometry sphere = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_SPHERE_POINT);
float sphereData[] = {0, 1, 0, 1};  // x, y, z, radius
rtcSetSharedGeometryBuffer(sphere, RTC_BUFFER_TYPE_VERTEX, 0, 
                           RTC_FORMAT_FLOAT4, sphereData, 0, 
                           4 * sizeof(float), 1);
rtcCommitGeometry(sphere);
rtcAttachGeometry(scene, sphere);
rtcSetGeometryUserData(sphere, &mirrorMaterial);
```

### Example 2: Change Image Resolution

```cpp
int image_width = 1920;   // Full HD width
int image_height = 1080;  // Full HD height
```

### Example 3: Add Colored Lighting

```cpp
PointLight redLight(Vector3D(-5, 3, 0), Color(255, 50, 50));
PointLight blueLight(Vector3D(5, 3, 0), Color(50, 50, 255));
lights.push_back(&redLight);
lights.push_back(&blueLight);
```

## Troubleshooting

**Embree not found during build:**
```bash
cmake -Dembree_DIR=/usr/local/lib/cmake/embree-4.4.0 ..
```

**Embree error at runtime:**
Check that Embree libraries are in your library path:
```bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH  # Linux
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH  # macOS
```

**Black image output:**
- Check light positions are not inside geometry
- Verify camera is positioned correctly (not inside objects)
- Ensure light intensities are not zero

**Long render times:**
- Reduce image resolution
- Decrease MAX_DEPTH for reflections
- Simplify geometry

## Advanced Topics

### Adding New Geometry Types

Embree supports various geometry types:
- RTC_GEOMETRY_TYPE_TRIANGLE
- RTC_GEOMETRY_TYPE_QUAD
- RTC_GEOMETRY_TYPE_SPHERE_POINT
- RTC_GEOMETRY_TYPE_CONE_LINEAR_CURVE
- And more...

See [Embree documentation](https://www.embree.org/api.html) for details.

### Implementing Different Shading Models

You can modify the `shade()` function to implement:
- Cook-Torrance BRDF
- Oren-Nayar diffuse
- Anisotropic reflections
- Subsurface scattering

### Adding Textures

Store texture data and sample it in the shade function using UV coordinates from the ray hit.

## References

- [Intel Embree Documentation](https://www.embree.org/)
- [Phong Reflection Model](https://en.wikipedia.org/wiki/Phong_reflection_model)
- [Ray Tracing in One Weekend](https://raytracing.github.io/)

## See Also

- [Main Repository README](../README.md)
- [Brightness Calculation Module](../brightness-calculation/README.md)
- [Illuminance Calculation Module](../illuminance-calculation/README.md)
