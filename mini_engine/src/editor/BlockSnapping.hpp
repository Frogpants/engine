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
    // Clear pending
    b->pendingNext = nullptr;
    b->pendingInside = nullptr;

    const float W = 220.0f;
    const float H = 36.0f;

    for (Block* other : g_editor.blocks)
    {
        if (other == b) continue;

        // compute key points
        ImVec2 bTopCenter = ImVec2(b->position.x + W * 0.5f, b->position.y);
        ImVec2 otherBottomCenter = ImVec2(other->position.x + W * 0.5f, other->position.y + H);

        // candidate for next (stack) attach: top of b near bottom of other (attach below other)
        if (Near(bTopCenter, otherBottomCenter, 18.0f)) {
            b->pendingNext = other;
        }

        // candidate for inside attach if other is a CBlock: check near the inside area
        if (other->shape == BlockShape::CBlock) {
            ImVec2 insidePoint = ImVec2(other->position.x + 20.0f, other->position.y + 40.0f);
            if (Near(ImVec2(b->position.x, b->position.y), insidePoint, 28.0f)) {
                b->pendingInside = other;
                // inside is a stronger candidate, we can break
                break;
            }
        }

        if (b->pendingNext && !b->pendingInside) {
            // if we found a next and no inside, break early
            break;
        }
    }
}

#endif // BLOCKSNAPPING_HPP