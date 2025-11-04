#include "vector3d.h"
#include <cmath>

// Constructor implementation
Vector3D::Vector3D(const double x_, const double y_, const double z_) noexcept
    : x(x_), y(y_), z(z_) {}

// Vector addition: returns the sum of two vectors
Vector3D Vector3D::operator+(const Vector3D& other) const noexcept {
    return {x + other.x, y + other.y, z + other.z};
}

// Vector subtraction: returns the difference of two vectors
Vector3D Vector3D::operator-(const Vector3D& other) const noexcept {
    return {x - other.x, y - other.y, z - other.z};
}

// Scalar multiplication: scales vector by a scalar value
Vector3D Vector3D::operator*(double scalar) const noexcept {
    return {x * scalar, y * scalar, z * scalar};
}

// Dot product: returns the scalar product of two vectors
double Vector3D::dot(const Vector3D& other) const noexcept {
    return x * other.x + y * other.y + z * other.z;
}

// Cross product: returns a vector perpendicular to both input vectors
Vector3D Vector3D::cross(const Vector3D& other) const noexcept {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

// Calculate Euclidean norm (magnitude) of the vector using hypot for numerical stability
double Vector3D::norm() const noexcept {
    return std::hypot(x, y, z);
}

// Return a normalized (unit length) version of the vector
// Returns original vector if it has zero length to avoid division by zero
Vector3D Vector3D::normalized() const noexcept {
    const double n = norm();
    return (n != 0.0) ? (*this) * (1.0 / n) : *this;
}