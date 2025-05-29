#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D {
public:
    double x, y, z;

    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator*(double scalar) const;

    double dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;
    double norm() const;
    Vector3D normalized() const;
};

#endif // VECTOR3D_H