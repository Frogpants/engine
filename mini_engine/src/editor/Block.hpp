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
    std::string name;
    std::string text;

    std::vector<BlockArgument> args;
    Block* next = nullptr;      // next block in stack
    Block* inside = nullptr;    // first child for CBlock type
    std::vector<Block*> children; // all children blocks for easy iteration

    bool isDragging = false;    // Track if block is being dragged
    ImVec2 dragOffset;          // Offset from mouse to block top-left

    // Palette/category index (e.g. Motion=0, Looks=1, ...)
    int category = 0;
    
    // Transient pending snap targets while dragging (not yet attached)
    Block* pendingNext = nullptr;
    Block* pendingInside = nullptr;
    
    // Last known position (used to compute movement delta for attached blocks)
    ImVec2 lastPosition = ImVec2(0,0);


};

#endif // BLOCK_HPP
