#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP


#include <cmath>

#include "essentials.hpp"


class Transform {
public:
    vec3 position;
    vec4 rotation;
    vec3 scale;

    vec3 EuluerAngles;

    Transform() {
        position = vec3(0.0);
        rotation = vec4(0.0);
        scale = vec3(1.0);

        EuluerAngles = rotation.xyz;
    }

};


#endif // TRANSFORM_HPP