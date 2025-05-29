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
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error: Failed to open 'input.txt'.\n";
        return 1;
    }

    int lightCount = 0;
    if (!(inputFile >> lightCount) || lightCount <= 0) {
        std::cerr << "Error: Invalid number of light sources.\n";
        return 1;
    }

    std::vector<Light> lights;
    for (int i = 0; i < lightCount; ++i) {
        double lx, ly, lz, ldx, ldy, ldz, lr, lg, lb;
        if (!(inputFile >> lx >> ly >> lz >> ldx >> ldy >> ldz >> lr >> lg >> lb)) {
            std::cerr << "Error: Invalid data for light source #" << i + 1 << ".\n";
            return 1;
        }

        lights.push_back({
            Vector3D(lx, ly, lz),
            Vector3D(ldx, ldy, ldz),
            Color(lr, lg, lb)
        });
    }

    Vector3D P0, P1, P2;
    if (!(inputFile >> P0.x >> P0.y >> P0.z
                   >> P1.x >> P1.y >> P1.z
                   >> P2.x >> P2.y >> P2.z)) {
        std::cerr << "Error: Invalid triangle coordinates.\n";
        return 1;
    }

    Color color;
    double kd, ks, ke;
    if (!(inputFile >> color.r >> color.g >> color.b >> kd >> ks >> ke)) {
        std::cerr << "Error: Invalid material parameters.\n";
        return 1;
    }

    Material material = {color, kd, ks, ke};

    double x, y;
    Vector3D viewDir;
    if (!(inputFile >> x >> y >> viewDir.x >> viewDir.y >> viewDir.z)) {
        std::cerr << "Error: Invalid point or view direction data.\n";
        return 1;
    }

    Color brightness = calculateBrightness(lights, P0, P1, P2, x, y, viewDir, material);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Point brightness: ("
              << brightness.r << ", "
              << brightness.g << ", "
              << brightness.b << ")\n";

    return 0;
}