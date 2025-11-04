#ifndef VECTOR3D_H
#define VECTOR3D_H

/**
 * @brief 3D vector class for geometric calculations
 * 
 * Provides basic vector operations including arithmetic operations,
 * dot product, cross product, normalization, and magnitude calculation.
 */
class Vector3D {
public:
    double x, y, z; ///< Vector components in 3D space

    /**
     * @brief Vector subtraction
     * @param other Vector to subtract
     * @return Difference of the two vectors
     */
    Vector3D operator-(const Vector3D& other) const;

    /**
     * @brief Vector addition
     * @param other Vector to add
     * @return Sum of the two vectors
     */
    Vector3D operator+(const Vector3D& other) const;

    /**
     * @brief Scalar multiplication
     * @param scalar Scalar value to multiply by
     * @return Scaled vector
     */
    Vector3D operator*(double scalar) const;

    /**
     * @brief Compute dot product with another vector
     * @param other Vector to compute dot product with
     * @return Dot product (scalar value)
     */
    double dot(const Vector3D& other) const;

    /**
     * @brief Compute cross product with another vector
     * @param other Vector to compute cross product with
     * @return Cross product vector (perpendicular to both inputs)
     */
    Vector3D cross(const Vector3D& other) const;

    /**
     * @brief Calculate the magnitude (length) of the vector
     * @return Euclidean norm of the vector
     */
    double norm() const;

    /**
     * @brief Get a normalized (unit length) version of the vector
     * @return Normalized vector with magnitude 1
     */
    Vector3D normalized() const;
};

#endif // VECTOR3D_H