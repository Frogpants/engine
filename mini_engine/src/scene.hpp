#ifndef SCENE_HPP
#define SCENE_HPP

#pragma once
#include <vector>
#include <memory>
#include <string>

#include "SceneObject.hpp"
#include "mesh.hpp"

class Scene {
public:
    std::vector<std::unique_ptr<SceneObject>> objects;
    Mesh backdrop;

    SceneObject* CreateObject(const std::string& name);
};

Scene scene;


#endif // SCENE_HPP