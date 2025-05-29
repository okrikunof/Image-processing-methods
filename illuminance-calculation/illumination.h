#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <array>
#include "vector3d.h"

std::array<double, 3> calculateIllumination(
    const std::array<double, 3>& I0,
    const Vector3D& O,
    const Vector3D& PL,
    const Vector3D& P0,
    const Vector3D& P1,
    const Vector3D& P2,
    double x,
    double y
);

#endif // ILLUMINATION_H