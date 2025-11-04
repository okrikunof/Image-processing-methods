#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <array>
#include "vector3d.h"

/**
 * @brief Calculate illumination at a point on a triangular surface
 * 
 * Computes the illumination (irradiance) at a point on a triangle
 * from a single directional light source, considering:
 * - Distance-based attenuation (inverse square law)
 * - Angle of incidence (cosine law)
 * - Light source directionality
 * 
 * @param I0 Light source intensity as RGB array [R, G, B]
 * @param O Direction vector of the light source axis
 * @param PL Position of the light source in 3D space
 * @param P0 First vertex of the triangle
 * @param P1 Second vertex of the triangle
 * @param P2 Third vertex of the triangle
 * @param x Local coordinate along edge P0->P1
 * @param y Local coordinate along edge P0->P2
 * @return RGB illumination at the point as array [R, G, B]
 */
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