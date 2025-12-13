#ifndef BLOCKCONNECTIONS_HPP
#define BLOCKCONNECTIONS_HPP

#include "Block.hpp"
#include "BlockSnapping.hpp"

inline void UpdateBlockDragging(Block* b)
{
    if (ImGui::IsMouseDown(0))
    {
        if (!b->isDragging)
        {
            ImVec2 mp = ImGui::GetMousePos();
            ImVec2 p = b->position;

            if (mp.x > p.x && mp.x < p.x+180 &&
                mp.y > p.y && mp.y < p.y+60)
            {
                b->isDragging = true;
                b->dragOffset = ImVec2(mp.x - p.x, mp.y - p.y);

                // detach from any current attachments so the block can be moved freely
                b->next = nullptr;
                for (Block* other : g_editor.blocks) {
                    if (other->inside == b) other->inside = nullptr;
                }
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
        b->isDragging = false;
    }
}

// Move a block and recursively translate any attached blocks (next/inside)
inline void MoveBlockWithAttachments(Block* b, const ImVec2& delta)
{
    if (!b) return;
    if (delta.x == 0.0f && delta.y == 0.0f) return;

    if (b->next) {
        b->next->position.x += delta.x;
        b->next->position.y += delta.y;
        b->next->lastPosition = b->next->position;
        MoveBlockWithAttachments(b->next, delta);
    }

    if (b->inside) {
        b->inside->position.x += delta.x;
        b->inside->position.y += delta.y;
        b->inside->lastPosition = b->inside->position;
        MoveBlockWithAttachments(b->inside, delta);
    }
}

#endif // BLOCKCONNECTIONS_HPP