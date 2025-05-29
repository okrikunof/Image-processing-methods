#include "illumination.h"
#include <cmath>
#include <algorithm>

bool isSameSide(const Vector3D& point, const Vector3D& planePoint,
                const Vector3D& normal, const Vector3D& reference) noexcept {
    const Vector3D vecToPoint = point - planePoint;
    const Vector3D vecToRef = reference - planePoint;
    const double dotPoint = vecToPoint.dot(normal);
    const double dotRef = vecToRef.dot(normal);
    return (dotPoint * dotRef) > 0;
}

Color calculateIllumination(const Light& light,
                           const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                           const double x, const double y, const Vector3D& viewDir) noexcept {
    const Vector3D edge1 = (P1 - P0).normalized();
    const Vector3D edge2 = (P2 - P0).normalized();
    const Vector3D PT = P0 + edge1 * x + edge2 * y;

    const Vector3D N = (P2 - P0).cross(P1 - P0).normalized();

    if (!isSameSide(light.position, PT, N, viewDir)) {
        return {};
    }

    const Vector3D s_vec = PT - light.position;
    const double R2 = s_vec.norm() * s_vec.norm();

    const Vector3D s_normalized = s_vec.normalized();
    const Vector3D lightDirNormalized = light.direction.normalized();

    const double cos_alpha = std::max(0.0, s_normalized.dot(N));
    const double cos_theta = std::max(0.0, s_normalized.dot(lightDirNormalized));

    return light.intensity * (cos_theta * cos_alpha / R2);
}

Color calculateBrightness(const std::vector<Light>& lights,
                         const Vector3D& P0, const Vector3D& P1, const Vector3D& P2,
                         const double x, const double y, const Vector3D& viewDir,
                         const Material& material) noexcept {
    const Vector3D edge1 = (P1 - P0).normalized();
    const Vector3D edge2 = (P2 - P0).normalized();
    const Vector3D PT = P0 + edge1 * x + edge2 * y;

    Vector3D N = (P2 - P0).cross(P1 - P0).normalized();
    if (viewDir.dot(N) < 0) {
        N = N * -1.0;
    }

    Color totalBrightness;
    for (const auto& light : lights) {
        Color E = calculateIllumination(light, P0, P1, P2, x, y, viewDir);
        Vector3D s = (light.position - PT).normalized();
        Vector3D h = (viewDir + s).normalized();

        const double diffuse = material.diffuse;
        const double specular = material.specular * std::pow(std::max(0.0, h.dot(N)), material.exponent);

        const Color contribution = E * material.color * (diffuse + specular) * (1.0 / M_PI);
        totalBrightness += contribution;
    }

    return totalBrightness;
}