#ifndef BLOCKCONNECTIONS_HPP
#define BLOCKCONNECTIONS_HPP

#include "Block.hpp"
#include "BlockSnapping.hpp"

inline void UpdateBlockDragging(Block* b)
{
    if (ImGui::IsMouseDown(0))
    {
        if (!b->dragging)
        {
            ImVec2 mp = ImGui::GetMousePos();
            ImVec2 p = b->position;

            if (mp.x > p.x && mp.x < p.x+180 &&
                mp.y > p.y && mp.y < p.y+60)
            {
                b->dragging = true;
                b->dragOffset = ImVec2(mp.x - p.x, mp.y - p.y);
            }
        }
        else
        {
            ImVec2 mp = ImGui::GetMousePos();
            b->position = ImVec2(mp.x - b->dragOffset.x, mp.y - b->dragOffset.y);
            UpdateSnapping(b);
        }
    }
    else
    {
        b->dragging = false;
    }
}

#endif // BLOCKCONNECTIONS_HPP