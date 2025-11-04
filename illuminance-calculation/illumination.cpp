#include "illumination.h"
#include "vector3d.h"
#include <array>
#include <cmath>

/**
 * Calculate illumination at a point on a triangular surface.
 * Uses the inverse square law for distance attenuation and
 * considers both the angle of incidence and light directionality.
 */
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
    // Convert local coordinates (x, y) to global coordinates on the triangle
    const Vector3D edge1 = (P1 - P0).normalized();
    const Vector3D edge2 = (P2 - P0).normalized();
    const Vector3D PT = P0 + edge1 * x + edge2 * y;

    // Calculate the normal vector to the triangle plane
    const Vector3D N = (P2 - P0).cross(P1 - P0).normalized();

    // Vector from the point on the surface to the light source
    const Vector3D s = PT - PL;
    const double R2 = s.norm() * s.norm(); // Distance squared (for inverse square law)

    // Calculate angle of incidence (angle between surface normal and light direction)
    // Using absolute value to handle both sides of the surface
    const double cos_alpha = std::abs(s.dot(N) / s.norm());
    
    // Calculate directionality factor (how aligned the light is with its axis)
    const double cos_theta = s.dot(O) / s.norm();

    // Calculate effective light intensity considering directionality
    const std::array I = {
        I0[0] * cos_theta,
        I0[1] * cos_theta,
        I0[2] * cos_theta
    };

    // Calculate final illumination using inverse square law and cosine law
    const std::array E = {
        (I[0] * cos_alpha) / R2,
        (I[1] * cos_alpha) / R2,
        (I[2] * cos_alpha) / R2
    };

    return E;
}