#ifndef MESH_HPP
#define MESH_HPP


#include <cmath>

#include "../essentials.hpp"


class Mesh {
public:
    int frame;
    bool visible;
    int layer;


    Mesh() {
        frame = 1;
        visible = true;
        layer = 1;
    }

};


#endif // MESH_HPP