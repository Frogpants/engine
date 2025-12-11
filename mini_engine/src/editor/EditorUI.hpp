#pragma once
#include "BlockTypes.hpp"
#include "Block.hpp"
#include "EditorState.hpp"
#include "../../imgui/imgui.h"
// #include "BlockManager.hpp"

// -----------------------------------------------------------
// Scratch-like block renderer
// -----------------------------------------------------------
ImVec4 GetBlockColor(BlockShape shape) {
    switch (shape) {
        case BlockShape::Stack:  return ImVec4(0.30f, 0.55f, 1.0f, 1.0f); // Motion blue
        case BlockShape::CBlock: return ImVec4(1.00f, 0.70f, 0.25f, 1.0f); // Control orange
        default: return ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
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


void DrawScratchBlockBackground(const ImVec2& pos, const ImVec2& size, ImVec4 color, float outlineThickness)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImU32 col = ImGui::ColorConvertFloat4ToU32(color);

    float x = pos.x;
    float y = pos.y;
    float w = size.x;
    float h = size.y;

    // Connector sizes (unchanged)
    const float notchX = 15.0f;

    const float notchW = 40.0f;
    const float notchH = 4.5f;
    const float i = 4.5f;
    const float r = 4.0f;

    const float br = 2.0f;

    // ---------------------------
    // 1) DRAW ORIGINAL BLOCK SHAPE (your code untouched)
    // ---------------------------
    dl->PathClear();

    // TOP LEFT
    dl->PathLineTo(ImVec2(x + r, y));

    // TOP INDENT (unchanged geometry)
    dl->PathLineTo(ImVec2(x + notchX, y));
    dl->PathLineTo(ImVec2(x + notchX, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW - i, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW, y));

    // TOP RIGHT
    dl->PathLineTo(ImVec2(x + w - r, y));
    dl->PathArcTo(ImVec2(x + w - r, y + r), r, -1.5708f, 0.0f, 10);

    // RIGHT
    dl->PathLineTo(ImVec2(x + w, y + h - r));

    // BOTTOM RIGHT
    dl->PathArcTo(ImVec2(x + w - r, y + h - r), r, 0.0f, 1.5708f, 10);

    float notchX0 = x + notchX;        // left indent start
    float notchX1 = x + notchW;        // right indent end

    float y0 = y + h;                  // top of notch cut
    float y1 = y + h + notchH;         // bottom of notch

    // Move to right top of notch
    dl->PathLineTo(ImVec2(notchX1, y0));

    //
    // RIGHT CURVE
    // arc center should be BELOW the block, outside the shape
    //
    ImVec2 rightArcCenter(notchX1 - br - i, y1);

    //  PI to PI*1.5  (curves upward & inward)
    dl->PathArcTo(rightArcCenter, br, IM_PI, IM_PI * 1.5f, 6);

    //
    // BOTTOM EDGE OF NOTCH
    //
    dl->PathLineTo(ImVec2(notchX0 + br + i, y1));

    //
    // LEFT CURVE
    //
    ImVec2 leftArcCenter(notchX0 + br + i, y1);

    //  PI*1.5 to PI*2  (curves upward & inward)
    dl->PathArcTo(leftArcCenter, br, IM_PI * 1.5f, IM_PI * 2.0f, 6);

    //
    // Back to top of notch
    //
    dl->PathLineTo(ImVec2(notchX0, y0));

    // BOTTOM LEFT
    dl->PathLineTo(ImVec2(x + r, y + h));
    dl->PathArcTo(ImVec2(x + r, y + h - r), r, 1.5708f, 3.14159f, 10);

    // LEFT
    dl->PathLineTo(ImVec2(x, y + r));
    dl->PathArcTo(ImVec2(x + r, y + r), r, 3.14159f, 4.71239f, 10);

    dl->PathFillConvex(col);

    // ---------------------------
    // 2) REDRAW THE TOP NOTCH AS A CUT-OUT
    // ---------------------------
    ImU32 bg = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

    dl->PathClear();
    dl->PathLineTo(ImVec2(x + notchX, y));
    dl->PathLineTo(ImVec2(x + notchX + i, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW - i, y + notchH));
    dl->PathLineTo(ImVec2(x + notchW, y));
    dl->PathFillConvex(bg);

    // Start a new path
    dl->PathClear();

    ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(Brightness(color, 0.5));
    ImU32 outlineColor = fillColor;    

    // ──────────────────────────────
    // Fill the block
    // ──────────────────────────────
    dl->PathFillConvex(fillColor);

    // ──────────────────────────────
    // Stroke the same path to draw the outline
    // ──────────────────────────────
    dl->PathStroke(outlineColor, false, outlineThickness);
}

bool DrawScratchBlock(Block* b, float width = 220.0f, float height = 36.0f)
{
    ImGui::InvisibleButton(b->text.c_str(), ImVec2(width, height));
    bool pressed = ImGui::IsItemClicked();

    ImVec2 pos = ImGui::GetItemRectMin();
    ImVec2 size = ImGui::GetItemRectSize();

    ImVec4 color = GetBlockColor(b->shape);

    // Highlight running block
    bool isRunning =
        g_editor.running &&
        g_editor.currentBlockIndex < g_editor.executionOrder.size() &&
        g_editor.executionOrder[g_editor.currentBlockIndex] == b;

    if (isRunning) {
        color = ImVec4(0.1f, 1.0f, 0.1f, 1.0f);
    }

    DrawScratchBlockBackground(pos, size, color, 5.0);

    // Draw text on top
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddText(
        ImVec2(pos.x + 10, pos.y + 10),
        IM_COL32(255, 255, 255, 255),
        b->text.c_str()
    );

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
// Left panel: Block palette
// -----------------------------------------------------------
void DrawBlockPalette() {
    ImGui::BeginChild("BlockPalette", ImVec2(300, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    static int selectedTab = 0;
    char* tabs[] = { "Motion", "Looks", "Sound", "Events", "Control", "Sensing", "Operators", "Variables", "Lists", "My Blocks" };

    for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
        if (ImGui::Selectable(tabs[i], selectedTab == i)) {
            selectedTab = i;
        }
    }
    ImGui::Separator();
    ImGui::Text("%s Blocks:", tabs[selectedTab]);

    for (int j = 0; j < 5; j++) {
        std::string blockName = std::string(tabs[selectedTab]) + " Block " + std::to_string(j+1);
        if (ImGui::Button(blockName.c_str())) {
            Block* newBlock = new Block();
            newBlock->id = rand();
            newBlock->text = blockName;
            newBlock->shape = BlockShape::Stack;
            newBlock->position = ImVec2(50, 50);
            g_editor.blocks.push_back(newBlock);
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

    for (Block* b : g_editor.blocks)
    {
        if (b->shape != BlockShape::Stack && b->shape != BlockShape::CBlock) {
            continue;
        }

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

        ImVec2 panelSize = ImGui::GetContentRegionAvail();

        const float spriteW = 50.0f;
        const float spriteH = 50.0f;
        const float cornerR = 6.0f;
        const float nameHeight = 14.0f;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        float x = ImGui::GetCursorPosX();
        float y = ImGui::GetCursorPosY();

        for (std::size_t i = 0; i < g_editor.sprites.size(); i++) {

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

            // Name text centered
            ImGui::SetCursorScreenPos(ImVec2(
                nameMin.x + (spriteW * 0.5f) - ImGui::CalcTextSize(g_editor.sprites[i]->name.c_str()).x * 0.5f,
                nameMin.y
            ));
            ImGui::Text("%s", g_editor.sprites[i]->name.c_str());

            // --- Button Collision Box ---
            ImGui::SetCursorScreenPos(pMin);
            std::string spriteBtnId = std::string("sprite_btn_") + std::to_string(i);
            bool pressed = ImGui::InvisibleButton(
                spriteBtnId.c_str(),
                ImVec2(spriteW, spriteH)
            );

            if (pressed)
                g_editor.selectedSprite = g_editor.sprites[i];

            // --- Hover Outline ---
            if (ImGui::IsItemHovered())
            {
                ImU32 hoverCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.984f, 0.376f, 0.384f, 1));

                dl->AddRect( ImVec2(pMin.x - 2, pMin.y - 2), ImVec2(pMax.x + 2, pMax.y + 2), hoverCol, cornerR + 2, 0, 2.0f);
            }
            

            float offX = 0.0f;
            float offY = 0.0f;
            // Move to next sprite
            ImGui::SetCursorPos(ImVec2(x + offX, y + offY));
        }

        // Add Sprite Button
        ImGui::SetCursorPos(ImVec2(panelSize.x - 120, panelSize.y - 40));
        if (ImGui::Button("Add Sprite", ImVec2(120, 30))) {
            g_editor.sprites.push_back(new Sprite("Sprite"));
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