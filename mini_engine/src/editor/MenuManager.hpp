#pragma once

#include <vector>
#include <string>

#include <algorithm>
#include <iostream>
#include <iterator>


class Menu {
public:
    std::vector<std::string> list;

    Menu() {
        list = {};
    }

    void add(std::string& element) {
        list.push_back(element);
    }

    void remove(int id) {
        list.erase(list.begin() + id);
    }

    int grabID(std::string& element) {
        auto it = std::find(list.begin(), list.end(), element);

        if (it != list.end()) {
            return std::distance(list.begin(), it);
        } else {
            std::cout << "Value " << element << " not found in the vector." << std::endl;
        }
    }
};