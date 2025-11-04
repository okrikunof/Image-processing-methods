#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

/**
 * @brief Material properties for surface rendering
 * 
 * Defines the appearance of a surface using the Phong reflection model,
 * including diffuse color, specular highlight, and shininess.
 */
class Material {
public:
    Color color;       ///< Base color (diffuse color) of the material
    double diffuse;    ///< Diffuse reflection coefficient (typically 0.0-1.0)
    double specular;   ///< Specular reflection coefficient (controls highlight intensity)
    double exponent;   ///< Specular exponent (controls highlight sharpness/shininess)
};

#endif // MATERIAL_H