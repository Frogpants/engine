#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP


#include <cmath>

#include "essentials.hpp"
#include "components/transform.hpp"
#include "components/mesh.hpp"
#include "input.hpp"


#pragma once
#include <string>
#include <vector>

class Scene;

class SceneObject {
public:
    Transform transform;
    Mesh mesh;
    InputManager Input;

    bool isClone = false;

    std::string name;
    Scene* scene = nullptr;  

    SceneObject* parent = nullptr;
    std::vector<SceneObject*> children;

    SceneObject(const std::string& n);

    SceneObject();

    SceneObject* CreateChild(const std::string& name);
    void AddChild(SceneObject* child);
};


// class SceneObject {
// public:
//     Transform transform;
//     Mesh mesh;

//     SceneObject* parent = nullptr;

//     bool isStatic;

//     std::string name;
//     std::string type;

//     SceneObject() {
//         transform.position = transform.position + parent.position;
//         transform.rotation = transform.rotation + parent.rotation;

//         isStatic = true;

//         name = "GameObject";
//         type = "empty";
//     }

//     SceneObject(std::string name_, std::string type_) {
//         transform.position = transform.position + parent.position;
//         transform.rotation = transform.rotation + parent.rotation;

//         isStatic = true;

//         name = name_;
//         type = type_;
//     }

//     void clone() {
        
//     }

// };


#endif // SCENEOBJECT_HPP