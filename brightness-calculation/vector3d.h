#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D {
public:
    double x, y, z;

    Vector3D(double x_ = 0, double y_ = 0, double z_ = 0) noexcept;

    Vector3D operator+(const Vector3D& other) const noexcept;
    Vector3D operator-(const Vector3D& other) const noexcept;
    Vector3D operator*(double scalar) const noexcept;

    double dot(const Vector3D& other) const noexcept;
    Vector3D cross(const Vector3D& other) const noexcept;
    
    double norm() const noexcept;
    Vector3D normalized() const noexcept;
};

#endif // VECTOR3D_H