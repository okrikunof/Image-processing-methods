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
     * @brief Construct a 3D vector with given coordinates
     * @param x_ X-component (default: 0)
     * @param y_ Y-component (default: 0)
     * @param z_ Z-component (default: 0)
     */
    Vector3D(double x_ = 0, double y_ = 0, double z_ = 0) noexcept;

    /**
     * @brief Vector addition
     * @param other Vector to add
     * @return Sum of the two vectors
     */
    Vector3D operator+(const Vector3D& other) const noexcept;

    /**
     * @brief Vector subtraction
     * @param other Vector to subtract
     * @return Difference of the two vectors
     */
    Vector3D operator-(const Vector3D& other) const noexcept;

    /**
     * @brief Scalar multiplication
     * @param scalar Scalar value to multiply by
     * @return Scaled vector
     */
    Vector3D operator*(double scalar) const noexcept;

    /**
     * @brief Compute dot product with another vector
     * @param other Vector to compute dot product with
     * @return Dot product (scalar value)
     */
    double dot(const Vector3D& other) const noexcept;

    /**
     * @brief Compute cross product with another vector
     * @param other Vector to compute cross product with
     * @return Cross product vector (perpendicular to both inputs)
     */
    Vector3D cross(const Vector3D& other) const noexcept;
    
    /**
     * @brief Calculate the magnitude (length) of the vector
     * @return Euclidean norm of the vector
     */
    double norm() const noexcept;

    /**
     * @brief Get a normalized (unit length) version of the vector
     * @return Normalized vector with magnitude 1 (or original if zero length)
     */
    Vector3D normalized() const noexcept;
};

#endif // VECTOR3D_H