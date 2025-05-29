#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <vector>
#include "vector3d.h"
#include "color.h"
#include "light.h"
#include "material.h"

bool isSameSide(const Vector3D& point, const Vector3D& planePoint,
                const Vector3D& normal, const Vector3D& reference) noexcept;

Color calculateIllumination(const Light& light,
                           const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                           double x, double y, const Vector3D& viewDir) noexcept;

Color calculateBrightness(const std::vector<Light>& lights,
                         const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                         double x, double y, const Vector3D& viewDir,
                         const Material& material) noexcept;

#endif // ILLUMINATION_H