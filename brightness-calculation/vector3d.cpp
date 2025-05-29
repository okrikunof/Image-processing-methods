#include "vector3d.h"
#include <cmath>

Vector3D::Vector3D(const double x_, const double y_, const double z_) noexcept
    : x(x_), y(y_), z(z_) {}

Vector3D Vector3D::operator+(const Vector3D& other) const noexcept {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3D Vector3D::operator-(const Vector3D& other) const noexcept {
    return {x - other.x, y - other.y, z - other.z};
}

Vector3D Vector3D::operator*(double scalar) const noexcept {
    return {x * scalar, y * scalar, z * scalar};
}

double Vector3D::dot(const Vector3D& other) const noexcept {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::cross(const Vector3D& other) const noexcept {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

double Vector3D::norm() const noexcept {
    return std::hypot(x, y, z);
}

Vector3D Vector3D::normalized() const noexcept {
    const double n = norm();
    return (n != 0.0) ? (*this) * (1.0 / n) : *this;
}