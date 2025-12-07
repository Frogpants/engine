#ifndef BLOCKARGUMENTSLOTS_HPP
#define BLOCKARGUMENTSLOTS_HPP

#include "Block.hpp"
#include <imgui.h>

inline void DrawArgumentSlots(ImDrawList* dl, Block* b)
{
    float x = b->position.x + 100;
    float y = b->position.y + 10;

    for (int i = 0; i < b->args.size(); i++)
    {
        BlockArgument& a = b->args[i];

        ImVec2 p(x, y + i * 22);

        ImVec2 s1(p.x, p.y);
        ImVec2 s2(p.x + 70, p.y + 20);

        ImU32 col = IM_COL32(255, 255, 255, 255);

        if (a.type == ArgType::Boolean)
            dl->AddRectFilled(s1, s2, IM_COL32(100,255,100,255), 4);
        else
            dl->AddRectFilled(s1, s2, IM_COL32(255,255,255,255), 4);

        dl->AddText(ImVec2(p.x+6, p.y+4), IM_COL32_BLACK, a.value.c_str());
    }
}

#endif // BLOCKARGUMENTSLOTS_HPP