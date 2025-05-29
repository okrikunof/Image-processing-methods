#ifndef LIGHT_H
#define LIGHT_H

#include "vector3d.h"
#include "color.h"

class Light {
public:
    Vector3D position;
    Vector3D direction;
    Color intensity;
};

#endif // LIGHT_H