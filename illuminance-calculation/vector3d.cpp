#include "vector3d.h"
#include <cmath>

// Vector subtraction: returns the difference of two vectors
Vector3D Vector3D::operator-(const Vector3D& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

// Vector addition: returns the sum of two vectors
Vector3D Vector3D::operator+(const Vector3D& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

// Scalar multiplication: scales vector by a scalar value
Vector3D Vector3D::operator*(const double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

// Dot product: returns the scalar product of two vectors
double Vector3D::dot(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

// Cross product: returns a vector perpendicular to both input vectors
Vector3D Vector3D::cross(const Vector3D& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

// Calculate Euclidean norm (magnitude) of the vector
double Vector3D::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Return a normalized (unit length) version of the vector
Vector3D Vector3D::normalized() const {
    const double n = norm();
    return {x / n, y / n, z / n};
}