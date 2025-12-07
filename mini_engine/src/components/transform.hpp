#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP


#include <cmath>

#include "../essentials.hpp"
#include "../SceneObject.hpp"


class Transform {
public:

    vec3 position;
    vec4 rotation;
    vec3 scale;

    vec3 euluerAngles;

    vec3 up;
    vec3 forward;
    vec3 right;

    Transform() {
        position = vec3(0.0);
        rotation = vec4(0.0);
        scale = vec3(1.0);

        euluerAngles = rotation.xyz;

        forward = vec3(sin(rotation.x), sin(rotation.y), cos(rotation.x));
        up = vec3(sin(rotation.z), cos(rotation.y), sin(rotation.y));
        right = vec3(cos(rotation.x), cos(rotation.z), sin(rotation.x));
    }

    float pointAt(const vec2& v) {
        vec2 dir = v - position.xy;
        return atan2(dir.y, dir.x);
    }

    vec3 pointAt(const vec3& v) {
        vec3 dir = v - position;
        float len = length(dir.xz);
        return vec3(atan2(dir.y, len), atan2(dir.x, dir.z), rotation.z);
    }

};


#endif // TRANSFORM_HPP