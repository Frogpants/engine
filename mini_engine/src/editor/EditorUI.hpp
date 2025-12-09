#pragma once
#include "BlockTypes.hpp"
#include "Block.hpp"
#include "EditorState.hpp"
#include "../../imgui/imgui.h"

// -----------------------------------------------------------
// Draw blocks recursively in workspace
// -----------------------------------------------------------
void DrawBlockRecursive(Block* b) {
    if (!b) return;

    ImGui::PushID(b);

    bool isRunning =
        g_editor.running &&
        g_editor.currentBlockIndex < g_editor.executionOrder.size() &&
        g_editor.executionOrder[g_editor.currentBlockIndex] == b;

    if (isRunning)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 1.0f, 0.2f, 1.0f));

    if (ImGui::Button(b->text.c_str(), ImVec2(200, 30)))
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
                ImGui::InputFloat(("##num" + std::to_string(i)).c_str(), &arg.numberValue);
                break;
            case ArgType::Boolean:
                ImGui::Checkbox(("##bool" + std::to_string(i)).c_str(), &arg.boolValue);
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

    // !!! ADD DIFFERENT BLOCKS FOR EACH TAB !!! (BlockManager.hpp)

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
    // Stage panel
    ImGui::BeginChild("StagePanel", ImVec2(0, -150), true);
    ImVec2 stageSize = ImGui::GetContentRegionAvail();

    ImGui::Text("Stage / Sprite Area");
    ImGui::Dummy(stageSize);
    ImGui::EndChild();

    ImGui::BeginChild("SpritePanel", ImVec2(0, 150), true);
    ImGui::Text("Sprites:");

    // Display existing sprites
    for (size_t i = 0; i < g_editor.sprites.size(); i++) {
        ImGui::Text("- %s", g_editor.sprites[i]->name.c_str());
    }

    // Add new sprite button in bottom right
    ImVec2 panelSize = ImGui::GetContentRegionAvail();
    ImGui::SetCursorPosX(panelSize.x - 120);
    ImGui::SetCursorPosY(panelSize.y - 30);
    if (ImGui::Button("Add Sprite", ImVec2(120, 30)))
    {
        g_editor.sprites.push_back(new Sprite("Sprite1"));
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