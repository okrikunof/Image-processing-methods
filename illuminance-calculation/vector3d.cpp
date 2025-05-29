#include "vector3d.h"
#include <cmath>

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3D Vector3D::operator*(const double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

double Vector3D::dot(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::cross(const Vector3D& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

double Vector3D::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalized() const {
    const double n = norm();
    return {x / n, y / n, z / n};
}