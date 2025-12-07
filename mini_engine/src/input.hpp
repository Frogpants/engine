#ifndef INPUT_HPP
#define INPUT_HPP


#include <iostream>

#include "essentials.hpp"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include <cctype>

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


class InputMapper {
public:

    int lastKeyPressed;
    bool keyPressed;

    GLFWwindow* window;

    std::string allKeys;

    KeyMapper Keys;

    InputMapper(GLFWwindow* window_) {
        lastKeyPressed = 0;
        keyPressed = false;

        window = window_;

        allKeys = "abcdefghijklmnopqrstuvwxyz0123456789 -=[]\\;',./";

        Keys.build(allKeys);
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
};


#endif // INPUT_HPP