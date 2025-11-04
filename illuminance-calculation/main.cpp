/**
 * @file main.cpp
 * @brief Interactive illumination calculator for triangular surfaces
 * 
 * This program calculates the illumination (irradiance) at a point on a
 * triangular surface from a single directional light source. The calculation
 * considers distance-based attenuation and the angle of incidence.
 * 
 * The user interactively inputs:
 * - Light source intensity (RGB)
 * - Light direction and position
 * - Triangle vertices
 * - Local coordinates on the triangle
 */

#include <iostream>
#include <array>
#include "vector3d.h"
#include "illumination.h"

int main() {
    std::array<double, 3> I0{};  // Light source intensity (RGB)
    Vector3D O{};                // Direction of the light source axis
    Vector3D PL{};               // Coordinates of the light source
    Vector3D P0{};               // Coordinates of the first vertex of the triangle
    Vector3D P1{};               // Coordinates of the second vertex of the triangle
    Vector3D P2{};               // Coordinates of the third vertex of the triangle
    double x, y;                 // Local coordinates x and y on the triangle

    // Input the light source intensity (RGB)
    std::cout << "Enter the light source intensity (R G B separated by spaces): ";
    std::cin >> I0[0] >> I0[1] >> I0[2];

    // Input the direction of the light source axis
    std::cout << "Enter the direction of the light source axis (x y z separated by spaces): ";
    std::cin >> O.x >> O.y >> O.z;

    // Input the coordinates of the light source
    std::cout << "Enter the coordinates of the light source (x y z separated by spaces): ";
    std::cin >> PL.x >> PL.y >> PL.z;

    // Input the coordinates of the triangle vertices
    std::cout << "Enter the coordinates of the first vertex of the triangle (x y z separated by spaces): ";
    std::cin >> P0.x >> P0.y >> P0.z;

    std::cout << "Enter the coordinates of the second vertex of the triangle (x y z separated by spaces): ";
    std::cin >> P1.x >> P1.y >> P1.z;

    std::cout << "Enter the coordinates of the third vertex of the triangle (x y z separated by spaces): ";
    std::cin >> P2.x >> P2.y >> P2.z;

    // Input local coordinates on the triangle
    std::cout << "Enter the local coordinate x: ";
    std::cin >> x;

    std::cout << "Enter the local coordinate y: ";
    std::cin >> y;

    // Calculate illumination at the specified point
    const std::array<double, 3> E = calculateIllumination(I0, O, PL, P0, P1, P2, x, y);

    // Output the result
    std::cout << "Point illumination: (" << E[0] << ", " << E[1] << ", " << E[2] << ")\n";

    return 0;
}
