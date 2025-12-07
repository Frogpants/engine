#ifndef BLOCKSNAPPING_HPP
#define BLOCKSNAPPING_HPP

#include <cmath>

#include "EditorState.hpp"

inline bool Near(ImVec2 a, ImVec2 b, float dist=12.f)
{
    return fabs(a.x - b.x) < dist && fabs(a.y - b.y) < dist;
}

inline void UpdateSnapping(Block* b)
{
    for (Block* other : g_editor.blocks)
    {
        if (other == b) continue;

        ImVec2 bottom = ImVec2(b->position.x, b->position.y + 70);
        ImVec2 top = other->position;

        if (Near(bottom, top))
            b->next = other;
    }
}

#endif // BLOCKSNAPPING_HPP