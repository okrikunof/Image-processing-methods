#include "illumination.h"
#include "vector3d.h"
#include <array>

std::array<double, 3> calculateIllumination(
    const std::array<double, 3>& I0,
    const Vector3D& O,
    const Vector3D& PL,
    const Vector3D& P0,
    const Vector3D& P1,
    const Vector3D& P2,
    const double x,
    const double y
) {
    // Converting local coordinates to global coordinates
    const Vector3D edge1 = (P1 - P0).normalized();
    const Vector3D edge2 = (P2 - P0).normalized();
    const Vector3D PT = P0 + edge1 * x + edge2 * y;

    // Calculation of the normal to the plane
    const Vector3D N = (P2 - P0).cross(P1 - P0).normalized();

    // Vector from the point to the light source
    const Vector3D s = PT - PL;
    const double R2 = s.norm() * s.norm();

    // Angle calculation
    const double cos_alpha = std::abs(s.dot(N) / s.norm());
    const double cos_theta = s.dot(O) / s.norm();

    // Light intensity
    const std::array I = {
        I0[0] * cos_theta,
        I0[1] * cos_theta,
        I0[2] * cos_theta
    };

    // Point illumination
    const std::array E = {
        (I[0] * cos_alpha) / R2,
        (I[1] * cos_alpha) / R2,
        (I[2] * cos_alpha) / R2
    };

    return E;
}