#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <vector>
#include "vector3d.h"
#include "color.h"
#include "light.h"
#include "material.h"

/**
 * @brief Check if a point is on the same side of a plane as a reference point
 * 
 * This is used to determine if a light source is on the visible side
 * of a surface (for backface culling in lighting calculations).
 * 
 * @param point Point to check
 * @param planePoint A point on the plane
 * @param normal Normal vector of the plane
 * @param reference Reference point (typically view direction)
 * @return true if point and reference are on the same side of the plane
 */
bool isSameSide(const Vector3D& point, const Vector3D& planePoint,
                const Vector3D& normal, const Vector3D& reference) noexcept;

/**
 * @brief Calculate illumination from a single light source
 * 
 * Computes the illumination (irradiance) at a point on a triangular surface
 * from a single light source, considering distance attenuation and angles.
 * 
 * @param light Light source
 * @param P0 First vertex of the triangle
 * @param P1 Second vertex of the triangle
 * @param P2 Third vertex of the triangle
 * @param x Local coordinate along edge P0->P1
 * @param y Local coordinate along edge P0->P2
 * @param viewDir View direction vector
 * @return RGB illumination at the point
 */
Color calculateIllumination(const Light& light,
                           const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                           double x, double y, const Vector3D& viewDir) noexcept;

/**
 * @brief Calculate total brightness with Phong shading model
 * 
 * Computes the final brightness (color) at a point on a triangular surface
 * illuminated by multiple light sources, using the Phong reflection model
 * with diffuse and specular components.
 * 
 * @param lights Vector of light sources
 * @param P0 First vertex of the triangle
 * @param P1 Second vertex of the triangle
 * @param P2 Third vertex of the triangle
 * @param x Local coordinate along edge P0->P1
 * @param y Local coordinate along edge P0->P2
 * @param viewDir View direction vector
 * @param material Material properties of the surface
 * @return Final RGB brightness at the point
 */
Color calculateBrightness(const std::vector<Light>& lights,
                         const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                         double x, double y, const Vector3D& viewDir,
                         const Material& material) noexcept;

#endif // ILLUMINATION_H