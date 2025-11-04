#ifndef LIGHT_H
#define LIGHT_H

#include "vector3d.h"
#include "color.h"

/**
 * @brief Light source representation
 * 
 * Describes a light source with position, direction, and intensity.
 * Used for calculating illumination on surfaces.
 */
class Light {
public:
    Vector3D position;  ///< Position of the light source in 3D space
    Vector3D direction; ///< Direction vector of the light source
    Color intensity;    ///< RGB intensity/color of the light
};

#endif // LIGHT_H