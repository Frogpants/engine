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

    Transform parent;

    bool isStatic;

    std::string name;
    std::string type;

    SceneObject() {
        transform.position = transform.position + parent.position;
        transform.rotation = transform.rotation + parent.rotation;

        isStatic = true;

        name = "GameObject";
        type = "empty";
    }

    SceneObject(std::string name_, std::string type_) {
        transform.position = transform.position + parent.position;
        transform.rotation = transform.rotation + parent.rotation;

        isStatic = true;

        name = name_;
        type = type_;
    }

    void clone() {
        
    }

};


#endif // SCENEOBJECT_HPP