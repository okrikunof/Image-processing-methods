# Brightness Calculation Module

## Overview

This module calculates the brightness (luminance) of a point on a triangular surface illuminated by multiple light sources using the **Phong reflection model**. It combines diffuse and specular reflection components to produce realistic lighting effects.

## Features

- **Multiple Light Sources**: Supports any number of point/directional lights
- **Phong Shading**: Implements the classic Phong reflection model
- **Diffuse Reflection**: Lambertian reflectance for matte surfaces
- **Specular Highlights**: Controllable shininess and highlight intensity
- **File-Based Input**: Easy configuration via input text file
- **RGB Color Output**: Separate red, green, and blue brightness values

## File Structure

```
brightness-calculation/
├── main.cpp            # Main program with file I/O
├── vector3d.h / .cpp   # 3D vector mathematics
├── color.h / .cpp      # RGB color operations
├── light.h             # Light source structure
├── material.h          # Material properties
├── illumination.h / .cpp # Lighting calculations
├── input.txt           # Example input data
└── CMakeLists.txt      # Build configuration
```

## Building

```bash
cd brightness-calculation
mkdir build
cd build
cmake ..
make
```

## Usage

1. Create or modify `input.txt` with your scene parameters
2. Run the executable:
   ```bash
   ./brightness-calculation
   ```
3. The program will output the brightness values to the console

## Input Format

The `input.txt` file should contain (all values space-separated):

```
<number_of_lights>
<light1_pos_x> <light1_pos_y> <light1_pos_z> <light1_dir_x> <light1_dir_y> <light1_dir_z> <light1_r> <light1_g> <light1_b>
<light2_pos_x> <light2_pos_y> <light2_pos_z> <light2_dir_x> <light2_dir_y> <light2_dir_z> <light2_r> <light2_g> <light2_b>
...
<triangle_P0_x> <triangle_P0_y> <triangle_P0_z>
<triangle_P1_x> <triangle_P1_y> <triangle_P1_z>
<triangle_P2_x> <triangle_P2_y> <triangle_P2_z>
<material_color_r> <material_color_g> <material_color_b> <diffuse_coef> <specular_coef> <exponent>
<local_x> <local_y> <view_dir_x> <view_dir_y> <view_dir_z>
```

### Parameter Descriptions

- **number_of_lights**: Integer count of light sources
- **light position**: (x, y, z) coordinates in 3D space
- **light direction**: Direction vector (dx, dy, dz) - normalized automatically
- **light intensity**: RGB values (r, g, b) - can be > 1.0 for bright lights
- **triangle vertices**: Three 3D points (P0, P1, P2) defining the triangle
- **material color**: Base RGB color of the surface
- **diffuse_coef**: Diffuse reflection coefficient (typically 0.0-1.0)
- **specular_coef**: Specular reflection coefficient (controls highlight strength)
- **exponent**: Specular exponent (higher = sharper highlights, typical range: 5-100)
- **local_x, local_y**: Coordinates on the triangle (0.0-1.0 typically)
- **view direction**: Direction from which the surface is viewed

## Example Input

```
2
1.0 1.0 1.0   0.0 0.0 -1.0   1.0 1.0 1.0
-1.0 1.0 1.0  0.0 0.0 -1.0   2.0 2.0 2.0
0.0 0.0 0.0
1.0 0.0 0.0
0.0 1.0 0.0
1.0 1.0 1.0   0.7 0.3 10.0
0.5 0.5   0.0 0.0 1.0
```

This configuration:
- Uses 2 light sources with different positions and intensities
- Defines a triangle in the XY plane
- Uses a white material with moderate diffuse and specular properties
- Queries the brightness at the center of the triangle (0.5, 0.5)
- Views from the +Z direction

## Output

The program outputs the calculated brightness as RGB values:

```
Point brightness: (0.234567, 0.234567, 0.234567)
```

## Algorithm

The brightness calculation follows these steps:

1. **Convert local coordinates** to global 3D position on the triangle
2. **Calculate surface normal** from the triangle vertices
3. **For each light source**:
   - Calculate illumination (distance attenuation + angle factors)
   - Compute diffuse component (Lambertian: N·L)
   - Compute specular component (Phong: (H·N)^exponent)
   - Combine with material properties
4. **Sum contributions** from all lights

## Mathematical Model

**Diffuse Component**: `Kd * (N · L) * I / π`

**Specular Component**: `Ks * (H · N)^n * I / π`

Where:
- `Kd` = diffuse coefficient
- `Ks` = specular coefficient
- `N` = surface normal
- `L` = light direction
- `H` = half-vector between view and light direction
- `n` = specular exponent
- `I` = light intensity (with distance attenuation)

## Dependencies

- C++20 compiler
- CMake >= 3.20
- Standard C++ library only (no external dependencies)

## See Also

- [Main Repository README](../README.md)
- [Illuminance Calculation Module](../illuminance-calculation/README.md)
- [Scene Rendering Module](../scene-rendering/README.md)
