#ifndef INPUT_HPP
#define INPUT_HPP


#include <iostream>


#include "essentials.hpp"
#include "components/mesh.hpp"
#include "SceneObject.hpp"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include <cctype>

#include "../../imgui/imgui.h"

class KeyMapper {
public:
    KeyMapper() = default;

    void build(const std::string& chars) {
        for (char c : chars) {
            char upper = std::toupper(c);

            // A–Z
            if (upper >= 'A' && upper <= 'Z') {
                map[c] = GLFW_KEY_A + (upper - 'A');
            }
            // 0–9
            else if (c >= '0' && c <= '9') {
                map[c] = GLFW_KEY_0 + (c - '0');
            }
            else {
                switch (c) {
                    case ' ': map[c] = GLFW_KEY_SPACE; break;
                    case '-': map[c] = GLFW_KEY_MINUS; break;
                    case '=': map[c] = GLFW_KEY_EQUAL; break;
                    case '[': map[c] = GLFW_KEY_LEFT_BRACKET; break;
                    case ']': map[c] = GLFW_KEY_RIGHT_BRACKET; break;
                    case '\\': map[c] = GLFW_KEY_BACKSLASH; break;
                    case ';': map[c] = GLFW_KEY_SEMICOLON; break;
                    case '\'': map[c] = GLFW_KEY_APOSTROPHE; break;
                    case ',': map[c] = GLFW_KEY_COMMA; break;
                    case '.': map[c] = GLFW_KEY_PERIOD; break;
                    case '/': map[c] = GLFW_KEY_SLASH; break;
                }
            }
        }
    }

    int operator[](char c) const { return map.at(c); }
    bool contains(char c) const { return map.count(c) > 0; }

private:
    std::unordered_map<char, int> map;
};


class InputManager {
public:

    int lastKeyPressed;
    bool keyPressed;

    GLFWwindow* window;

    std::string allKeys;

    KeyMapper Keys;

    ImVec2 mouse;

    bool leftDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    bool rightDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);

    InputManager(GLFWwindow* window_, SceneObject sprite, SceneObject camera) {
        lastKeyPressed = 0;
        keyPressed = false;

        window = window_;

        allKeys = "abcdefghijklmnopqrstuvwxyz0123456789 -=[]\\;',./";

        Keys.build(allKeys);

        mouse = ImGui::GetIO().MousePos;
    }

    bool detectKey(int k) {
        if (glfwGetKey(window, k) == GLFW_PRESS) {
            return true;
        }
        return false;
    }

    bool detectKey(char k) {
        int key = Keys[k];
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            return true;
        }
        return false;
    }

    // bool intersectTriangle(const vec3& orig, const vec3& dir, const vec3& v0, const vec3& v1, const vec3& v2, float& t) {
    //     const float EPSILON = 0.0000001f;
    //     vec3 edge1 = v1 - v0;
    //     vec3 edge2 = v2 - v0;
    //     vec3 h = cross(dir, edge2);
    //     float a = dot(edge1, h);
    //     if (fabs(a) < EPSILON) return false;

    //     float f = 1.0f / a;
    //     vec3 s = orig - v0;
    //     float u = f * dot(s, h);
    //     if (u < 0.0 || u > 1.0) return false;

    //     vec3 q = cross(s, edge1);
    //     float v = f * dot(dir, q);
    //     if (v < 0.0 || u + v > 1.0) return false;

    //     t = f * dot(edge2, q);
    //     return t > EPSILON;
    // }

    // vec3 getMouseRay(float mouseX, float mouseY, float screenWidth, float screenHeight, const mat4& projection, const mat4& view, const vec3& cameraPos) {
    //     // NDC
    //     float x = (2.0f * mouseX) / screenWidth - 1.0f;
    //     float y = 1.0f - (2.0f * mouseY) / screenHeight;
    //     vec4 ray_clip(x, y, -1.0f, 1.0f);

    //     // Eye space
    //     vec4 ray_eye = inverse(projection) * ray_clip;
    //     ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    //     // World space
    //     vec3 ray_world = normalize((inverse(view) * ray_eye).xyz);

    //     return ray_world;
    // }


    // bool touchingMouse() {
    //     bool hit = false;
    //     closestT = std::numeric_limits<float>::max();

    //     for (size_t i = 0; i < mesh.indices.size(); i += 3) {
    //         vec3 v0 = mesh.vertices[mesh.indices[i]];
    //         vec3 v1 = mesh.vertices[mesh.indices[i+1]];
    //         vec3 v2 = mesh.vertices[mesh.indices[i+2]];

    //         float t;
    //         if (intersectTriangle(rayOrigin, rayDir, v0, v1, v2, t)) {
    //             if (t < closestT) {
    //                 closestT = t;
    //                 hit = true;
    //             }
    //         }
    //     }

    //     return hit;
    // }
};


#endif // INPUT_HPP