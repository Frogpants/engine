#ifndef SCENE_HPP
#define SCENE_HPP

#pragma once
#include <vector>
#include <memory>
#include <string>

#include "SceneObject.hpp"

class Scene {
public:
    std::vector<std::unique_ptr<SceneObject>> objects;

    SceneObject* CreateObject(const std::string& name);
};


#endif // SCENE_HPP