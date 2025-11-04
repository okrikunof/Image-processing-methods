# Image Processing Methods

A collection of C++ implementations for various image processing and computer graphics algorithms, including illumination calculations, brightness computations, and 3D scene rendering.

## 📋 Table of Contents

- [Overview](#overview)
- [Repository Structure](#repository-structure)
- [Dependencies](#dependencies)
- [Build Instructions](#build-instructions)
- [Modules](#modules)
  - [Brightness Calculation](#brightness-calculation)
  - [Illuminance Calculation](#illuminance-calculation)
  - [Scene Rendering](#scene-rendering)
- [Usage Examples](#usage-examples)
- [Contributing](#contributing)
- [License](#license)

## Overview

This repository contains three independent C++ projects focused on computer graphics and image processing:

1. **Brightness Calculation** - Computes surface brightness with multiple light sources using Phong illumination model
2. **Illuminance Calculation** - Calculates point illumination from a single directional light source
3. **Scene Rendering** - 3D scene ray tracing using Intel Embree library

Each module is self-contained with its own CMake build configuration.

## Repository Structure

```
Image-processing-methods/
├── brightness-calculation/     # Multi-light source brightness calculation
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── vector3d.h / .cpp      # 3D vector operations
│   ├── color.h / .cpp         # RGB color representation
│   ├── light.h                # Light source structure
│   ├── material.h             # Material properties
│   ├── illumination.h / .cpp  # Illumination algorithms
│   └── input.txt              # Example input data
│
├── illuminance-calculation/   # Single light source illumination
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── vector3d.h / .cpp      # 3D vector operations
│   ├── illumination.h / .cpp  # Illumination algorithms
│
├── scene-rendering/           # Ray tracing with Intel Embree
│   ├── CMakeLists.txt
│   └── main.cpp              # Scene setup and rendering
│
└── README.md                  # This file
```

## Dependencies

### Common Requirements

- **CMake** >= 3.20
- **C++ Compiler** with C++20 support (GCC 10+, Clang 10+, MSVC 2019+)
- **Make** or **Ninja** build system

### Module-Specific Dependencies

#### Brightness Calculation & Illuminance Calculation
- No external dependencies (uses standard C++ library only)

#### Scene Rendering
- **[Intel Embree](https://github.com/RenderKit/embree)** v4.4.0 or higher
  - High-performance ray tracing kernels library
  - Installation instructions: https://github.com/RenderKit/embree#installation

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install cmake build-essential

# For scene-rendering module:
# Install Embree from source or package manager
```

**macOS:**
```bash
brew install cmake

# For scene-rendering module:
brew install embree
```

**Windows:**
- Install CMake from https://cmake.org/download/
- Install Visual Studio with C++ support
- For Embree: Download from https://github.com/RenderKit/embree/releases

## Build Instructions

### General Build Process

Each module can be built independently. Follow these steps for any module:

1. Navigate to the module directory:
   ```bash
   cd brightness-calculation  # or illuminance-calculation or scene-rendering
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure with CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   make
   # Or for Windows: cmake --build .
   ```

5. Run the executable:
   ```bash
   ./brightness-calculation  # Adjust name based on module
   ```

### Quick Build All Modules

To build all modules at once from the repository root:

```bash
# Brightness Calculation
cd brightness-calculation && mkdir -p build && cd build && cmake .. && make && cd ../..

# Illuminance Calculation
cd illuminance-calculation && mkdir -p build && cd build && cmake .. && make && cd ../..

# Scene Rendering (requires Embree)
cd scene-rendering && mkdir -p build && cd build && cmake .. && make && cd ../..
```

## Modules

### Brightness Calculation

Calculates the brightness (luminance) of a point on a triangular surface illuminated by multiple light sources using the Phong reflection model.

📖 **[Full Documentation](brightness-calculation/README.md)**

**Features:**
- Multiple light sources support
- Diffuse and specular reflection
- Phong shading model
- File-based input configuration

**Input Format (input.txt):**
```
<number_of_lights>
<light1_position_x> <light1_position_y> <light1_position_z> <light1_direction_x> <light1_direction_y> <light1_direction_z> <light1_intensity_r> <light1_intensity_g> <light1_intensity_b>
...
<triangle_P0_x> <triangle_P0_y> <triangle_P0_z>
<triangle_P1_x> <triangle_P1_y> <triangle_P1_z>
<triangle_P2_x> <triangle_P2_y> <triangle_P2_z>
<material_color_r> <material_color_g> <material_color_b> <diffuse_coef> <specular_coef> <exponent>
<local_coord_x> <local_coord_y> <view_direction_x> <view_direction_y> <view_direction_z>
```

**Output:**
```
Point brightness: (R, G, B)
```

### Illuminance Calculation

Computes the illumination (illuminance) at a point on a triangular surface from a single directional light source.

📖 **[Full Documentation](illuminance-calculation/README.md)**

**Features:**
- Single directional light source
- RGB illumination calculation
- Interactive console input
- Considers surface orientation and distance

**Usage:**
Run the program and follow the prompts to enter:
1. Light source intensity (R G B)
2. Light direction vector
3. Light position
4. Triangle vertices (P0, P1, P2)
5. Local coordinates on the triangle

**Output:**
```
Point illumination: (R, G, B)
```

### Scene Rendering

Advanced ray tracing implementation using Intel Embree for rendering 3D scenes with realistic lighting, shadows, and reflections.

📖 **[Full Documentation](scene-rendering/README.md)**

**Features:**
- Hardware-accelerated ray tracing (Intel Embree)
- Multiple geometry support (triangles, cubes)
- Point and directional light sources
- Diffuse and specular reflection
- Recursive ray tracing for reflections
- Shadow calculation
- PPM image output

**Output:**
Generates `output.ppm` image file (800x800 pixels)

**Scene Configuration:**
Scene is defined in source code with:
- Camera position and orientation
- Light sources
- Geometry objects with materials
- Reflection coefficients

## Usage Examples

### Example 1: Calculate Brightness with Multiple Lights

```bash
cd brightness-calculation/build
./brightness-calculation
# Reads from input.txt and outputs brightness values
```

### Example 2: Calculate Illumination Interactively

```bash
cd illuminance-calculation/build
./illuminance-calculation
# Follow interactive prompts:
Enter the light source intensity (R G B separated by spaces): 1.0 1.0 1.0
Enter the direction of the light source axis (x y z separated by spaces): 0 0 -1
...
```

### Example 3: Render a 3D Scene

```bash
cd scene-rendering/build
./scene-rendering
# Outputs: output.ppm
```

View the generated PPM image with image viewers like:
- **GIMP**: Open with GIMP
- **ImageMagick**: `convert output.ppm output.png`
- **Online viewers**: Upload to online PPM viewers

## Contributing

Contributions are welcome! Please follow these guidelines:

1. Maintain consistent code style with existing code
2. Add comments for complex algorithms
3. Update documentation for new features
4. Test your changes before submitting

## License

This project is available for educational and research purposes. Please check individual source files for specific licensing information.
