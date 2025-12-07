#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "BlockTypes.hpp"
#include <string>
#include <vector>
#include <../imgui/imgui.h>

struct Block {
    int id;
    BlockShape shape;
    ImVec2 position;
    std::string text;

    std::vector<BlockArgument> args;
    Block* next = nullptr;      // next block in stack
    Block* inside = nullptr;    // first child for CBlock type
    std::vector<Block*> children; // all children blocks for easy iteration

    bool dragging = false;
    ImVec2 dragOffset;

};

#endif // BLOCK_HPP
