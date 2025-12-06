#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP


#include <cmath>

#include "essentials.hpp"
#include "transform.hpp"
#include "mesh.hpp"


class SceneObject {
public:
    Transform transform;
    Mesh mesh;

    SceneObject() {
        
    }

};


#endif // SCENEOBJECT_HPP