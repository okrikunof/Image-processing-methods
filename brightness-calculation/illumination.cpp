#include "illumination.h"
#include <cmath>
#include <algorithm>

/**
 * Helper function to check if a point and reference are on the same side of a plane.
 * Used for backface culling in lighting calculations.
 */
bool isSameSide(const Vector3D& point, const Vector3D& planePoint,
                const Vector3D& normal, const Vector3D& reference) noexcept {
    const Vector3D vecToPoint = point - planePoint;
    const Vector3D vecToRef = reference - planePoint;
    const double dotPoint = vecToPoint.dot(normal);
    const double dotRef = vecToRef.dot(normal);
    // Both should have the same sign (both positive or both negative)
    return (dotPoint * dotRef) > 0;
}

/**
 * Calculate illumination from a single light source.
 * Uses distance-based attenuation and considers the angle of incidence.
 */
Color calculateIllumination(const Light& light,
                           const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                           const double x, const double y, const Vector3D& viewDir) noexcept {
    // Convert local coordinates to global 3D position on the triangle
    const Vector3D edge1 = (P1 - P0).normalized();
    const Vector3D edge2 = (P2 - P0).normalized();
    const Vector3D PT = P0 + edge1 * x + edge2 * y;

    // Calculate surface normal using cross product of triangle edges
    const Vector3D N = (P2 - P0).cross(P1 - P0).normalized();

    // Check if light source is on the visible side of the surface
    if (!isSameSide(light.position, PT, N, viewDir)) {
        return {}; // Return black color if light is behind the surface
    }

    // Vector from surface point to light source
    const Vector3D s_vec = PT - light.position;
    const double R2 = s_vec.norm() * s_vec.norm(); // Distance squared for inverse square law

    const Vector3D s_normalized = s_vec.normalized();
    const Vector3D lightDirNormalized = light.direction.normalized();

    // Calculate angles for illumination
    const double cos_alpha = std::max(0.0, s_normalized.dot(N));      // Angle between surface normal and light direction
    const double cos_theta = std::max(0.0, s_normalized.dot(lightDirNormalized)); // Angle of light cone

    // Apply inverse square law and angular attenuation
    return light.intensity * (cos_theta * cos_alpha / R2);
}

/**
 * Calculate total brightness using the Phong reflection model.
 * Combines diffuse and specular components from all light sources.
 */
Color calculateBrightness(const std::vector<Light>& lights,
                         const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                         const double x, const double y, const Vector3D& viewDir,
                         const Material& material) noexcept {
    // Convert local coordinates to global position
    const Vector3D edge1 = (P1 - P0).normalized();
    const Vector3D edge2 = (P2 - P0).normalized();
    const Vector3D PT = P0 + edge1 * x + edge2 * y;

    // Calculate surface normal
    Vector3D N = (P2 - P0).cross(P1 - P0).normalized();
    
    // Flip normal if it points away from the view direction
    if (viewDir.dot(N) < 0) {
        N = N * -1.0;
    }

    Color totalBrightness;
    
    // Accumulate contributions from all light sources
    for (const auto& light : lights) {
        // Get illumination from this light source
        Color E = calculateIllumination(light, P0, P1, P2, x, y, viewDir);
        
        // Direction from surface to light
        Vector3D s = (light.position - PT).normalized();
        
        // Half-vector for specular calculation (Blinn-Phong model)
        Vector3D h = (viewDir + s).normalized();

        // Calculate diffuse component (Lambertian reflectance)
        const double diffuse = material.diffuse;
        
        // Calculate specular component (Phong specular highlight)
        const double specular = material.specular * std::pow(std::max(0.0, h.dot(N)), material.exponent);

        // Combine illumination with material properties
        const Color contribution = E * material.color * (diffuse + specular) * (1.0 / M_PI);
        totalBrightness += contribution;
    }

    return totalBrightness;
}