# Illuminance Calculation Module

## Overview

This module calculates the **illumination (irradiance)** at a point on a triangular surface from a single directional light source. The calculation considers distance-based attenuation (inverse square law) and the angle of incidence (cosine law).

## Features

- **Single Light Source**: Focused calculation for one directional light
- **Interactive Input**: Console-based user input for all parameters
- **RGB Illumination**: Separate calculations for red, green, and blue channels
- **Physically-Based**: Uses inverse square law and Lambert's cosine law
- **No External Dependencies**: Pure C++ implementation

## File Structure

```
illuminance-calculation/
├── main.cpp            # Main program with interactive I/O
├── vector3d.h / .cpp   # 3D vector mathematics
├── illumination.h / .cpp # Illumination calculation
└── CMakeLists.txt      # Build configuration
```

## Building

```bash
cd illuminance-calculation
mkdir build
cd build
cmake ..
make
```

## Usage

Run the executable and follow the interactive prompts:

```bash
./illuminance-calculation
```

You will be asked to enter:

1. **Light source intensity** (R G B): RGB values for light color/intensity
2. **Light direction** (x y z): Direction of the light source axis
3. **Light position** (x y z): 3D coordinates of the light source
4. **Triangle vertices** (P0, P1, P2): Three sets of (x y z) coordinates
5. **Local coordinates** (x, y): Position on the triangle to query

## Example Session

```
Enter the light source intensity (R G B separated by spaces): 100 100 100
Enter the direction of the light source axis (x y z separated by spaces): 0 0 -1
Enter the coordinates of the light source (x y z separated by spaces): 0 5 10
Enter the coordinates of the first vertex of the triangle (x y z separated by spaces): -1 0 0
Enter the coordinates of the second vertex of the triangle (x y z separated by spaces): 1 0 0
Enter the coordinates of the third vertex of the triangle (x y z separated by spaces): 0 2 0
Enter the local coordinate x: 0.5
Enter the local coordinate y: 0.5
Point illumination: (1.234567, 1.234567, 1.234567)
```

## Input Parameters

### Light Source Intensity (I₀)
RGB values representing the light's color and power. Values can exceed 1.0 for bright lights.
- Example: `100 100 100` (white light with intensity 100)
- Example: `255 200 150` (warm white light)

### Light Direction (O)
Unit vector indicating the direction of the light beam. The program automatically normalizes this vector.
- Example: `0 0 -1` (pointing downward along negative Z-axis)
- Example: `1 1 -1` (diagonal direction)

### Light Position (PL)
3D coordinates of the light source in world space.
- Example: `0 5 10` (5 units up, 10 units away on Z-axis)

### Triangle Vertices (P0, P1, P2)
Three points defining a triangular surface in 3D space.
- The order matters for normal calculation
- Example: Triangle in XY plane from (-1,0,0) to (1,0,0) to (0,2,0)

### Local Coordinates (x, y)
Position on the triangle surface in local coordinate system:
- x: Distance along edge P0→P1 (normalized)
- y: Distance along edge P0→P2 (normalized)
- Values typically in range [0, 1]
- (0, 0) = vertex P0
- (1, 0) = vertex P1
- (0, 1) = vertex P2
- (0.5, 0.5) = center of triangle

## Output

The program outputs RGB illumination values:

```
Point illumination: (R, G, B)
```

These values represent the illuminance (light energy per unit area) at the specified point, taking into account:
- Distance from light source (inverse square law)
- Angle of incidence (Lambert's cosine law)
- Light directionality

## Algorithm

The illumination is calculated as:

```
E = (I₀ × cos(θ) × cos(α)) / R²
```

Where:
- **I₀** = Light source intensity (RGB)
- **θ** = Angle between light direction and vector to surface point
- **α** = Angle between surface normal and vector to light
- **R** = Distance from light to surface point

### Calculation Steps

1. **Convert local coordinates** (x, y) to global 3D position on triangle:
   ```
   PT = P0 + normalized(P1-P0) × x + normalized(P2-P0) × y
   ```

2. **Calculate surface normal**:
   ```
   N = normalized((P2-P0) × (P1-P0))
   ```

3. **Calculate vector from surface to light**:
   ```
   s = PT - PL
   R² = |s|²
   ```

4. **Calculate angle factors**:
   ```
   cos(α) = |s · N| / |s|
   cos(θ) = s · O / |s|
   ```

5. **Calculate effective intensity**:
   ```
   I = I₀ × cos(θ)
   ```

6. **Calculate final illumination**:
   ```
   E = I × cos(α) / R²
   ```

## Physical Principles

### Inverse Square Law
Light intensity decreases with the square of the distance from the source:
```
Intensity ∝ 1/R²
```

This models how light energy spreads out over a spherical surface as it travels.

### Lambert's Cosine Law
The apparent brightness of a surface depends on the angle of incident light:
```
Illumination ∝ cos(α)
```

Where α is the angle between the surface normal and the light direction.

### Light Directionality
The `cos(θ)` term models how light intensity varies based on the light's directional axis, simulating spotlights or directional sources.

## Use Cases

- **Lighting Analysis**: Study how light intensity varies across a surface
- **Computer Graphics**: Foundation for more complex lighting models
- **Education**: Learn about physically-based illumination
- **Optimization**: Calculate optimal light placement for even illumination
- **Architectural Planning**: Analyze natural or artificial lighting

## Dependencies

- C++20 compiler
- CMake >= 3.20
- Standard C++ library only (no external dependencies)

## Differences from Brightness Calculation

This module differs from the brightness-calculation module:

| Feature | Illuminance Calculation | Brightness Calculation |
|---------|------------------------|------------------------|
| Light Sources | Single | Multiple |
| Input Method | Interactive console | File-based |
| Shading Model | Basic illumination | Phong (diffuse + specular) |
| Material Properties | Not considered | Full material support |
| Output | Illuminance (E) | Brightness/Luminance (L) |

## See Also

- [Main Repository README](../README.md)
- [Brightness Calculation Module](../brightness-calculation/README.md)
- [Scene Rendering Module](../scene-rendering/README.md)
