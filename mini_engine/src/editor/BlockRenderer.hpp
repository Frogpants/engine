#ifndef BLOCKRENDERER_HPP
#define BLOCKRENDERER_HPP

#include "Block.hpp"
#include <imgui.h>
#include <imgui_internal.h>

// Draw the top puzzle notch for stack blocks
inline void DrawPuzzleTop(ImDrawList* dl, ImVec2 p, float w, ImU32 col)
{
    float tabW = 20;
    float tabH = 6;

    // main top rectangle
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + 30), col, 4);

    // puzzle tab
    dl->AddRectFilled(ImVec2(p.x + 40, p.y + 30), ImVec2(p.x + 40 + tabW, p.y + 30 + tabH), col, 2);
}

// Draw a block based on type
inline void DrawBlock(ImDrawList* dl, Block* b) {
    ImVec2 p = b->position;
    float w = 180;
    float h = 45;
    ImU32 bg = IM_COL32(0, 115, 255, 255); // default color
    ImU32 shadow = IM_COL32(0,0,0,80);

    // --- Draw shadow ---
    dl->AddRectFilled(ImVec2(p.x+2, p.y+2), ImVec2(p.x + w + 2, p.y + h + 2), shadow, 4);

    // --- SHAPE TYPES ---
    if (b->shape == BlockShape::Stack)
    {
        DrawPuzzleTop(dl, p, w, bg);
    }
    else if (b->shape == BlockShape::Boolean)
    {
        // hex-like boolean shape
        ImVec2 v0 = p;
        ImVec2 v1 = ImVec2(p.x + w, p.y);
        ImVec2 v2 = ImVec2(p.x + w - 20, p.y + h);
        ImVec2 v3 = ImVec2(p.x + 20, p.y + h);

        ImVec2 pts[4] = {v0,v1,v2,v3};
        dl->AddConvexPolyFilled(pts, 4, bg);
    }
    else if (b->shape == BlockShape::Reporter)
    {
        float r = 20; // rounded edges
        dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), bg, r);
    }

    // TEXT
    dl->AddText(ImVec2(p.x + 10, p.y + 15), IM_COL32_BLACK, b->text.c_str());

}

#endif // BLOCKRENDERER_HPP