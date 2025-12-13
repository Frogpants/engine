#pragma once
#include "BlockTypes.hpp"
#include "Block.hpp"
#include "EditorState.hpp"
#include "BlockSnapping.hpp"
#include "BlockConnections.hpp"
#include "../../imgui/imgui.h"
// #include "BlockManager.hpp"

// -----------------------------------------------------------
// Scratch-like block renderer
// -----------------------------------------------------------
// Category-based Scratch-like colors
ImVec4 GetBlockColor(int category) {
    switch (category) {
        case 0: // Motion
            return ImVec4(0.298f, 0.592f, 1.0f, 1.0f); // #4C97FF
        case 1: // Looks
            return ImVec4(0.6f, 0.4f, 1.0f, 1.0f); // purple
        case 2: // Sound
            return ImVec4(0.87f, 0.39f, 0.87f, 1.0f); // pink/purple
        case 3: // Events
            return ImVec4(1.0f, 0.6f, 0.0f, 1.0f); // yellow/orange
        case 4: // Control
            return ImVec4(1.0f, 0.67f, 0.10f, 1.0f); // orange
        case 5: // Sensing
            return ImVec4(0.36f, 0.71f, 0.84f, 1.0f); // light blue
        case 6: // Operators
            return ImVec4(0.45f, 0.82f, 0.33f, 1.0f); // green
        case 7: // Variables
            return ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // orange/darker
        case 8: // Lists
            return ImVec4(1.0f, 0.45f, 0.15f, 1.0f); // reddish-orange
        case 9: // My Blocks
            return ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // grey
        default:
            return ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    }
}

// -----------------------------------------------------------
// Scratch block connectors (top indent + bottom notch)
// -----------------------------------------------------------
void DrawTopIndent(ImDrawList* dl, ImVec2 pos, float width, ImU32 color)
{
    float notchW = 25.0f;
    float notchH = 6.0f;

    // Top indent shape
    dl->AddRectFilled(
        pos,
        ImVec2(pos.x + width, pos.y + notchH),
        color
    );

    // Carved-out center notch
    dl->AddRectFilled(
        ImVec2(pos.x + notchW, pos.y),
        ImVec2(pos.x + width - notchW, pos.y + notchH),
        IM_COL32(0, 0, 0, 0)
    );
}

void DrawBottomNotch(ImDrawList* dl, ImVec2 pos, float width, float height, ImU32 color)
{
    float notchW = 25.0f;
    float notchH = 6.0f;

    float y = pos.y + height - notchH;

    // Bottom block part
    dl->AddRectFilled(
        ImVec2(pos.x, y),
        ImVec2(pos.x + width, pos.y + height),
        color
    );

    // Rect left + right but NOT center
    dl->AddRectFilled(
        ImVec2(pos.x, y),
        ImVec2(pos.x + notchW, y + notchH),
        color
    );
    dl->AddRectFilled(
        ImVec2(pos.x + width - notchW, y),
        ImVec2(pos.x + width, y + notchH),
        color
    );
}

ImVec4 Brightness(const ImVec4& c, float amount = 0.75f)
{
    return ImVec4(
        c.x * amount,
        c.y * amount,
        c.z * amount,
        c.w
    );
}

void DrawBlock(ImDrawList* dl, const ImVec2& pos, const ImVec2& size, ImVec4 color, float outline) {
    ImU32 col = ImGui::ColorConvertFloat4ToU32(color);

    float x = pos.x;
    float y = pos.y;
    float w = size.x;
    float h = size.y;

    const float notchX = 15.0f;

    const float notchW = 40.0f;
    const float notchH = 4.5f;
    const float i = 4.5f;
    const float r = 4.0f;

    dl->PathClear();

    dl->PathLineTo(ImVec2(x + r, y));

    dl->PathLineTo(ImVec2(x + notchX, y));
    dl->PathLineTo(ImVec2(x + notchX, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW - i, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW, y));

    dl->PathLineTo(ImVec2(x + w - r, y));
    dl->PathArcTo(ImVec2(x + w - r, y + r), r, -1.5708f, 0.0f, 10);

    dl->PathLineTo(ImVec2(x + w, y + h - r));

    dl->PathArcTo(ImVec2(x + w - r, y + h - r), r, 0.0f, 1.5708f, 10);

    dl->PathLineTo(ImVec2(x + notchX, y + h));
    dl->PathLineTo(ImVec2(x + notchX + i, y + h + notchH));
    dl->PathLineTo(ImVec2(x + notchW - i, y + h + notchH));
    dl->PathLineTo(ImVec2(x + notchW, y + h));

    dl->PathLineTo(ImVec2(x + r, y + h));
    dl->PathArcTo(ImVec2(x + r, y + h - r), r, 1.5708f, 3.14159f, 10);

    dl->PathLineTo(ImVec2(x, y + r));
    dl->PathArcTo(ImVec2(x + r, y + r), r, 3.14159f, 4.71239f, 10);

    dl->PathFillConvex(col);

    ImU32 bg = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

    dl->PathClear();
    dl->PathLineTo(ImVec2(x + notchX, y));
    dl->PathLineTo(ImVec2(x + notchX + i, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW - i, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW, y));
    dl->PathFillConvex(bg);

    dl->PathClear();

    if (outline) {
        ImVec4 newColor = Brightness(color, 0.5);
        ImU32 outlineColor = ImGui::ColorConvertFloat4ToU32(newColor);

        dl->PathClear();

        dl->PathLineTo(ImVec2(x + r, y));

        dl->PathLineTo(ImVec2(x + notchX, y));
        dl->PathLineTo(ImVec2(x + notchX, y + notchH));
        dl->PathLineTo(ImVec2(x + notchW - i, y + notchH));
        dl->PathLineTo(ImVec2(x + notchW, y));

        dl->PathLineTo(ImVec2(x + w - r, y));
        dl->PathArcTo(ImVec2(x + w - r, y + r), r, -1.5708f, 0.0f, 10);

        dl->PathLineTo(ImVec2(x + w, y + h - r));

        dl->PathArcTo(ImVec2(x + w - r, y + h - r), r, 0.0f, 1.5708f, 10);

        dl->PathLineTo(ImVec2(x + notchX, y + h));
        dl->PathLineTo(ImVec2(x + notchX + i, y + h + notchH));
        dl->PathLineTo(ImVec2(x + notchW - i, y + h + notchH));
        dl->PathLineTo(ImVec2(x + notchW, y + h));

        dl->PathLineTo(ImVec2(x + r, y + h));
        dl->PathArcTo(ImVec2(x + r, y + h - r), r, 1.5708f, 3.14159f, 10);

        dl->PathLineTo(ImVec2(x, y + r));
        dl->PathArcTo(ImVec2(x + r, y + r), r, 3.14159f, 4.71239f, 10);

        dl->PathStroke(outlineColor, false, outline);
    }
}


void DrawScratchBlockBackground(ImDrawList* dl, const ImVec2& pos, const ImVec2& size, ImVec4 color, float outlineThickness)
{
    ImVec4 newColor = Brightness(color, 0.5);
    ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(newColor);
    ImU32 outlineColor = fillColor;    

    DrawBlock(dl, pos, size, color, 2);
}

bool DrawScratchBlock(Block* b, float width = 220.0f, float height = 36.0f)
{
    ImGui::SetCursorScreenPos(b->position);
    ImGui::InvisibleButton(b->text.c_str(), ImVec2(width, height));
    bool pressed = ImGui::IsItemClicked();

    ImVec2 pos = ImGui::GetItemRectMin();
    ImVec2 size = ImGui::GetItemRectSize();

    ImVec4 color = GetBlockColor(b->category);

    // Highlight running block
    bool isRunning = g_editor.running && g_editor.currentBlockIndex < g_editor.executionOrder.size() && g_editor.executionOrder[g_editor.currentBlockIndex] == b;

    if (isRunning) {
        color = ImVec4(0.1f, 1.0f, 0.1f, 1.0f);
    }

    // If block was marked dragging (e.g. created from palette while mouse held), follow the mouse
    if (b->isDragging && !ImGui::IsItemActive()) {
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        b->position = ImVec2(mousePos.x - b->dragOffset.x, mousePos.y - b->dragOffset.y);
    }

    // Determine draw list: foreground when dragging so block appears above all UI
    ImDrawList* dl = b->isDragging ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();

    // Handle drag when the item is active (clicked/held inside workspace)
    if (ImGui::IsItemActive()) {
        if (!b->isDragging) {
            b->isDragging = true;
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            b->dragOffset = ImVec2(mousePos.x - pos.x, mousePos.y - pos.y);
        }
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        ImVec2 newPos = ImVec2(mousePos.x - b->dragOffset.x, mousePos.y - b->dragOffset.y);
        ImVec2 delta = ImVec2(newPos.x - b->lastPosition.x, newPos.y - b->lastPosition.y);
        b->position = newPos;

        // move attached blocks together
        if ((delta.x != 0.0f || delta.y != 0.0f)) {
            MoveBlockWithAttachments(b, delta);
        }
        b->lastPosition = b->position;

        // update transient snapping while dragging
        UpdateSnapping(b);
    } else {
        // Finalize snap on mouse release (before clearing isDragging)
        if (ImGui::IsMouseReleased(0)) {
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            // if released over palette while the block was being dragged, delete the block
            bool wasPaletteDrop = (mousePos.x >= g_editor.paletteMin.x && mousePos.x <= g_editor.paletteMax.x &&
                mousePos.y >= g_editor.paletteMin.y && mousePos.y <= g_editor.paletteMax.y);
            if (wasPaletteDrop && (b->isDragging || g_editor.draggingNewBlock == b)) {
                // remove from blocks list and delete
                for (size_t ii = 0; ii < g_editor.blocks.size(); ++ii) {
                    if (g_editor.blocks[ii] == b) {
                        delete g_editor.blocks[ii];
                        g_editor.blocks.erase(g_editor.blocks.begin() + ii);
                        if (g_editor.draggingNewBlock == b) g_editor.draggingNewBlock = nullptr;
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
                        return false;
                    }
                }
            }
            // prefer inside snap over next
            if (b->pendingInside) {
                b->pendingInside->inside = b;
                // snap position to inside area
                ImVec2 newPos = ImVec2(b->pendingInside->position.x + 20.0f, b->pendingInside->position.y + 36.0f);
                ImVec2 delta = ImVec2(newPos.x - b->lastPosition.x, newPos.y - b->lastPosition.y);
                b->position = newPos;
                if ((delta.x != 0.0f || delta.y != 0.0f)) MoveBlockWithAttachments(b, delta);
                b->lastPosition = b->position;
            } else if (b->pendingNext) {
                b->next = b->pendingNext;
                // snap position to below the next block
                ImVec2 newPos = ImVec2(b->pendingNext->position.x, b->pendingNext->position.y + size.y + 4.0f);
                ImVec2 delta = ImVec2(newPos.x - b->lastPosition.x, newPos.y - b->lastPosition.y);
                b->position = newPos;
                if ((delta.x != 0.0f || delta.y != 0.0f)) MoveBlockWithAttachments(b, delta);
                b->lastPosition = b->position;
            }
            b->pendingInside = nullptr;
            b->pendingNext = nullptr;
        }

        // If the mouse button is not down, stop dragging
        if (!ImGui::GetIO().MouseDown[0])
            b->isDragging = false;
    }

    // Draw at current position
    DrawScratchBlockBackground(dl, b->position, size, color, 15.0);

    // Draw text on top
    dl->AddText(
        ImVec2(b->position.x + 10, b->position.y + 10),
        IM_COL32(255, 255, 255, 255),
        b->text.c_str()
    );

    // If dragging and there is a pending snap target, draw a highlight on the target
    if (b->isDragging) {
        if (b->pendingNext) {
            ImVec2 tp = b->pendingNext->position;
            ImU32 hcol = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.25f));
            dl->AddRectFilled(ImVec2(tp.x - 4, tp.y - 6), ImVec2(tp.x + 220 + 4, tp.y + 6), hcol, 4.0f);
        }
        if (b->pendingInside) {
            ImVec2 tp = b->pendingInside->position;
            ImU32 hcol = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.18f));
            dl->AddRectFilled(ImVec2(tp.x + 10, tp.y + 36), ImVec2(tp.x + 220 - 10, tp.y + 100), hcol, 6.0f);
        }
    }

    return pressed;
}


// -----------------------------------------------------------
// Draw blocks recursively in workspace
// -----------------------------------------------------------
void DrawBlockRecursive(Block* b) {
    if (!b) return;

    ImGui::PushID(b);

    bool isRunning = g_editor.running && g_editor.currentBlockIndex < g_editor.executionOrder.size() && g_editor.executionOrder[g_editor.currentBlockIndex] == b;

    if (isRunning)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 1.0f, 0.2f, 1.0f));

    if (DrawScratchBlock(b))
        g_editor.selected = b;

    if (isRunning)
        ImGui::PopStyleColor();

    for (int i = 0; i < b->args.size(); i++)
    {
        ImGui::Indent(20);
        BlockArgument& arg = b->args[i];

        ImGui::Text("%s:", arg.name.c_str());
        ImGui::SameLine();

        switch (arg.type)
        {
            case ArgType::Number:
            {
                // include block pointer in label so it's unique across all blocks
                std::string lbl = std::string("##num_") + std::to_string((uintptr_t)b) + "_" + std::to_string(i);
                ImGui::InputFloat(lbl.c_str(), &arg.numberValue);
            }
                break;
            case ArgType::Boolean:
            {
                std::string lbl = std::string("##bool_") + std::to_string((uintptr_t)b) + "_" + std::to_string(i);
                ImGui::Checkbox(lbl.c_str(), &arg.boolValue);
            }
                break;
            default:
                break;
        }

        ImGui::Unindent();
    }

    if (b->shape == BlockShape::CBlock && b->inside)
    {
        ImGui::Indent(40);
        DrawBlockRecursive(b->inside);
        ImGui::Unindent();
    }

    if (b->next)
    {
        ImGui::Indent(20);
        DrawBlockRecursive(b->next);
        ImGui::Unindent();
    }

    ImGui::PopID();
}

// -----------------------------------------------------------
// Left panel: Block palette (narrower, Scratch-style)
// -----------------------------------------------------------
void DrawBlockPalette() {
    ImGui::BeginChild("BlockPalette", ImVec2(200, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // store palette rect in editor state for deletion checks
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    g_editor.paletteMin = wp;
    g_editor.paletteMax = ImVec2(wp.x + ws.x, wp.y + ws.y);

    static int selectedTab = 0;
    char* tabs[] = { "Motion", "Looks", "Sound", "Events", "Control", "Sensing", "Operators", "Variables", "Lists", "My Blocks" };

    for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
        // draw a circular color swatch to the left of the tab name
        ImVec2 cur = ImGui::GetCursorScreenPos();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec4 colf = GetBlockColor(i);
        ImU32 colu = ImGui::ColorConvertFloat4ToU32(colf);
        float radius = 8.0f;
        float sw = (radius * 2.0f) + 6.0f;

        // compute selectable width so the name is not cut off
        ImVec2 avail = ImGui::GetContentRegionAvail();
        float selW = avail.x - sw - 4.0f;
        if (selW < 20.0f) selW = avail.x;

        dl->AddCircleFilled(ImVec2(cur.x + radius + 2, cur.y + radius + 2), radius, colu);

        // reserve space for the swatch so the selectable aligns
        ImGui::Dummy(ImVec2(sw, radius * 2.0f));
        ImGui::SameLine();

        // Selectable stretched horizontally using computed width
        if (ImGui::Selectable(tabs[i], selectedTab == i, 0, ImVec2(selW, radius * 2.0f + 4.0f))) {
            selectedTab = i;
        }
    }
    ImGui::Separator();
    ImGui::Text("%s Blocks:", tabs[selectedTab]);

    for (int j = 0; j < 5; j++) {
        std::string blockName = std::string(tabs[selectedTab]) + " Block " + std::to_string(j+1);
        // color the button using the category color
        ImVec4 btnCol = GetBlockColor(selectedTab);
        ImGui::PushStyleColor(ImGuiCol_Button, btnCol);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Brightness(btnCol, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Brightness(btnCol, 0.8f));

        ImGui::Button(blockName.c_str());

        ImGui::PopStyleColor(3);

        // start drag-create when user presses the button (hold)
        if (ImGui::IsItemActive() && !g_editor.draggingNewBlock) {
            Block* newBlock = new Block();
            newBlock->id = rand();
            newBlock->text = blockName;
            newBlock->shape = BlockShape::Stack;
            newBlock->category = selectedTab;
            // place at mouse pos and mark dragging
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            newBlock->position = ImVec2(mousePos.x - 20, mousePos.y - 10);
            newBlock->lastPosition = newBlock->position;
            newBlock->isDragging = true;
            newBlock->dragOffset = ImVec2(20, 10);
            g_editor.blocks.push_back(newBlock);
            g_editor.draggingNewBlock = newBlock;
            g_editor.selected = newBlock;
        }
    }

    ImGui::EndChild();
}


void DrawCodeWorkspace() {
    ImGui::BeginChild("Workspace", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    ImGui::BeginGroup();
    if (!g_editor.running) {
        if (ImGui::Button("Run")) { BuildExecutionOrder(); g_editor.running = true; g_editor.currentBlockIndex = 0; }
        ImGui::SameLine();
    }
    else {
        if (ImGui::Button("Stop")) { g_editor.running = false; g_editor.currentBlockIndex = 0; }
        ImGui::SameLine();
    }

    ImGui::EndGroup();
    ImGui::Separator();

    if (g_editor.running) {
        ImGui::Text("Executing %zu / %zu", g_editor.currentBlockIndex, g_editor.executionOrder.size());
    }
    g_editor.hoveredSocket = nullptr;
    g_editor.hoveredBoolean = nullptr;

    // Draw non-dragging blocks first
    for (Block* b : g_editor.blocks)
    {
        if (b->shape != BlockShape::Stack && b->shape != BlockShape::CBlock) {
            continue;
        }
        if (b->isDragging) continue;
        DrawBlockRecursive(b);
        ImGui::Separator();
    }

    // Then draw dragging blocks on top
    for (Block* b : g_editor.blocks)
    {
        if (b->shape != BlockShape::Stack && b->shape != BlockShape::CBlock) {
            continue;
        }
        if (!b->isDragging) continue;
        DrawBlockRecursive(b);
        ImGui::Separator();
    }

    ImGui::EndChild();
}


void DrawStageWithSprites() {
    // --- Stage Panel ---
    ImGui::BeginChild("StagePanel", ImVec2(0, -150), true);
    {
        ImVec2 stageSize = ImGui::GetContentRegionAvail();
        ImGui::Text("Stage / Sprite Area");
        ImGui::Dummy(stageSize);
    }
    ImGui::EndChild();

    // --- Sprite Panel ---
    ImGui::BeginChild("SpritePanel", ImVec2(0, 150), true);
    {
        ImGui::Text("Sprites:");

        // Adjustable sprite text size (persistent)
        static float spriteTextSize = 11.0f;

        ImVec2 panelSize = ImGui::GetContentRegionAvail();

        const float spriteW = 50.0f;
        const float spriteH = 50.0f;
        const float cornerR = 6.0f;
        const float nameHeight = 14.0f;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        float x = ImGui::GetCursorPosX();
        float y = ImGui::GetCursorPosY();

        for (size_t i = 0; i < g_editor.sprites.size(); i++) {

            ImVec2 pMin = ImGui::GetCursorScreenPos();
            ImVec2 pMax = ImVec2(pMin.x + spriteW, pMin.y + spriteH);

            ImU32 colBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.223f, 0.247f, 0.274f, 1));
            ImU32 colNameBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.984f, 0.376f, 0.384f, 1));

            // Draw body
            dl->AddRectFilled(pMin, pMax, colBg, cornerR);

            // Bottom name box
            ImVec2 nameMin = ImVec2(pMin.x, pMax.y - nameHeight);
            ImVec2 nameMax = pMax;
            dl->AddRectFilled(nameMin, nameMax, colNameBg, cornerR, ImDrawFlags_RoundCornersBottom);

            // Name text centered with adjustable size
            const std::string& spriteName = g_editor.sprites[i]->name;
            ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(spriteTextSize, 1000.0f, 0.0f, spriteName.c_str());
            ImVec2 textPos = ImVec2(
                nameMin.x + (spriteW * 0.5f) - textSize.x * 0.5f,
                nameMin.y + (nameHeight - textSize.y) * 0.5f
            );
            ImU32 textCol = IM_COL32(255, 255, 255, 255);
            dl->AddText(ImGui::GetFont(), spriteTextSize, textPos, textCol, spriteName.c_str());

            // --- Button Collision Box ---
            ImGui::SetCursorScreenPos(pMin);
            std::string spriteBtnId = std::string("sprite_btn_") + std::to_string(i);
            bool pressed = ImGui::InvisibleButton(spriteBtnId.c_str(), ImVec2(spriteW, spriteH));

            if (pressed)
                g_editor.selectedSprite = g_editor.sprites[i];

            // --- Hover Outline ---
            if (ImGui::IsItemHovered() || g_editor.selectedSprite == g_editor.sprites[i])
            {
                ImU32 hoverCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.984f, 0.376f, 0.384f, 1));

                dl->AddRect( ImVec2(pMin.x - 2, pMin.y - 2), ImVec2(pMax.x + 2, pMax.y + 2), hoverCol, cornerR + 2, 0, 2.0f);
            }


            
            float offX = ((i + 1) % 5) * (spriteW + 10.0);
            float offY = floor((i+1) / 5.0) * (spriteH + 10.0);
            // Move to next sprite
            ImGui::SetCursorPos(ImVec2(x + offX, y + offY));
        }

        // Add Sprite Button
        ImGui::SetCursorPos(ImVec2(panelSize.x - 120, panelSize.y - 40));
        if (ImGui::Button("Add Sprite", ImVec2(120, 30))) {
            g_editor.sprites.push_back(new Sprite("Sprite" + std::to_string(g_editor.sprites.size() + 1)));
        }
    }

    ImGui::EndChild();
}


// -----------------------------------------------------------
// Main editor layout
// -----------------------------------------------------------
void DrawEditorUI() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Scratch Editor", nullptr,
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove);

    // If we were dragging a new block from the palette and the mouse was released,
    // clear the draggingNewBlock pointer so the block stays in the workspace.
    if (g_editor.draggingNewBlock && !ImGui::GetIO().MouseDown[0]) {
        g_editor.draggingNewBlock->isDragging = false;
        g_editor.draggingNewBlock = nullptr;
    }

    // Three columns: Palette | Workspace | Stage+Sprites
    ImGui::Columns(3, nullptr, true);

    DrawBlockPalette();
    ImGui::NextColumn();

    DrawCodeWorkspace();
    ImGui::NextColumn();

    DrawStageWithSprites();

    ImGui::Columns(1);
    ImGui::End();
}