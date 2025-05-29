#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class Material {
public:
    Color color;
    double diffuse;
    double specular;
    double exponent;
};

#endif // MATERIAL_H