#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <cmath>

#include "../essentials.hpp"
#include "../SceneObject.hpp"


class Camera {
public:

    SceneObject* parent = nullptr;

    Camera() {
        
    }

};


#endif // CAMERA_HPP