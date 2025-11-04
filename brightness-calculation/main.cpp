/**
 * @file main.cpp
 * @brief Brightness calculation on a triangular surface with multiple light sources
 * 
 * This program calculates the brightness (luminance) at a point on a triangular
 * surface illuminated by multiple light sources using the Phong reflection model.
 * 
 * Input is read from 'input.txt' file with the following format:
 * - Number of light sources
 * - For each light: position (x,y,z), direction (dx,dy,dz), intensity (r,g,b)
 * - Triangle vertices: P0, P1, P2 coordinates
 * - Material properties: color (r,g,b), diffuse coefficient, specular coefficient, exponent
 * - Query point: local coordinates (x,y) and view direction (dx,dy,dz)
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "vector3d.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "illumination.h"

int main() {
    // Open input file
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error: Failed to open 'input.txt'.\n";
        return 1;
    }

    // Read number of light sources
    int lightCount = 0;
    if (!(inputFile >> lightCount) || lightCount <= 0) {
        std::cerr << "Error: Invalid number of light sources.\n";
        return 1;
    }

    // Read light source data
    std::vector<Light> lights;
    for (int i = 0; i < lightCount; ++i) {
        double lx, ly, lz, ldx, ldy, ldz, lr, lg, lb;
        if (!(inputFile >> lx >> ly >> lz >> ldx >> ldy >> ldz >> lr >> lg >> lb)) {
            std::cerr << "Error: Invalid data for light source #" << i + 1 << ".\n";
            return 1;
        }

        lights.push_back({
            Vector3D(lx, ly, lz),     // Light position
            Vector3D(ldx, ldy, ldz),  // Light direction
            Color(lr, lg, lb)         // Light intensity/color
        });
    }

    // Read triangle vertices
    Vector3D P0, P1, P2;
    if (!(inputFile >> P0.x >> P0.y >> P0.z
                   >> P1.x >> P1.y >> P1.z
                   >> P2.x >> P2.y >> P2.z)) {
        std::cerr << "Error: Invalid triangle coordinates.\n";
        return 1;
    }

    // Read material properties
    Color color;
    double kd, ks, ke;
    if (!(inputFile >> color.r >> color.g >> color.b >> kd >> ks >> ke)) {
        std::cerr << "Error: Invalid material parameters.\n";
        return 1;
    }

    Material material = {color, kd, ks, ke};

    // Read query point and view direction
    double x, y;
    Vector3D viewDir;
    if (!(inputFile >> x >> y >> viewDir.x >> viewDir.y >> viewDir.z)) {
        std::cerr << "Error: Invalid point or view direction data.\n";
        return 1;
    }

    // Calculate brightness at the specified point
    Color brightness = calculateBrightness(lights, P0, P1, P2, x, y, viewDir, material);

    // Output result with fixed precision
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Point brightness: ("
              << brightness.r << ", "
              << brightness.g << ", "
              << brightness.b << ")\n";

    return 0;
}