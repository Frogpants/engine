#pragma once
#include "EditorState.hpp"
#include "BlockRenderer.hpp"
#include "BlockConnections.hpp"
#include "BlockArgumentSlots.hpp"
#include <imgui.h>

// Layout constants
constexpr float PALETTE_WIDTH = 250.0f;
constexpr float STAGE_WIDTH = 400.0f;
constexpr float STAGE_HEIGHT = 300.0f;

inline void DrawBlockPalette()
{
ImGui::SetNextWindowPos(ImVec2(0,0));
ImGui::SetNextWindowSize(ImVec2(PALETTE_WIDTH, ImGui::GetIO().DisplaySize.y));
ImGui::Begin("Block Menu", nullptr,
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoMove |
ImGuiWindowFlags_NoCollapse);

// Run/Stop Buttons
if (ImGui::Button("Run"))
    g_editor.running = true;
ImGui::SameLine();
if (ImGui::Button("Stop"))
    g_editor.running = false;

ImGui::Separator();

struct BlockSection {
    std::string name;
    std::vector<std::pair<std::string, BlockShape>> blocks;
};

static int idCounter = 1000;

std::vector<BlockSection> sections = {
    {"Motion", { {"Move Steps", BlockShape::Stack}, {"Turn", BlockShape::Stack} }},
    {"Looks", { {"Say", BlockShape::Stack}, {"Show", BlockShape::Stack}, {"Hide", BlockShape::Stack} }},
    {"Control", { {"Repeat", BlockShape::CBlock}, {"If", BlockShape::CBlock}, {"Wait", BlockShape::Stack} }},
    {"Operators", { {"Add", BlockShape::Reporter}, {"Subtract", BlockShape::Reporter} }},
    {"Sensing", { {"Touching?", BlockShape::Boolean}, {"Key Pressed?", BlockShape::Boolean} }}
};

for (auto& section : sections)
{
    if (ImGui::CollapsingHeader(section.name.c_str()))
    {
        for (auto& pair : section.blocks)
        {
            if (ImGui::Button(pair.first.c_str()))
            {
                Block* b = new Block();
                b->id = idCounter++;
                b->shape = pair.second;
                b->position = ImVec2(PALETTE_WIDTH + 50, 50);
                b->text = pair.first;
                if (b->shape == BlockShape::Stack)
                    b->args.push_back({ArgType::Number, "10"});
                g_editor.blocks.push_back(b);
            }
        }
    }
}

ImGui::End();

}

inline void DrawStage()
{
ImGui::SetNextWindowPos(ImVec2(PALETTE_WIDTH, 0));
ImGui::SetNextWindowSize(ImVec2(STAGE_WIDTH, STAGE_HEIGHT));
ImGui::Begin("Stage", nullptr,
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoMove |
ImGuiWindowFlags_NoCollapse);

ImDrawList* dl = ImGui::GetWindowDrawList();

// Draw blocks
for (Block* b : g_editor.blocks)
{
    UpdateBlockDragging(b);
    DrawBlock(dl, b);
    DrawArgumentSlots(dl, b);
}

ImGui::End();

}

inline void DrawCodeEditor()
{
ImGui::SetNextWindowPos(ImVec2(PALETTE_WIDTH + STAGE_WIDTH, 0));
ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - (PALETTE_WIDTH + STAGE_WIDTH),
ImGui::GetIO().DisplaySize.y));
ImGui::Begin("Code Editor", nullptr,
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoMove |
ImGuiWindowFlags_NoCollapse);

ImGui::Text("Selected Block Info:");
if (g_editor.selected)
{
    ImGui::Text("ID: %d", g_editor.selected->id);
    ImGui::Text("Text: %s", g_editor.selected->text.c_str());
    ImGui::Text("Position: %.1f, %.1f", g_editor.selected->position.x, g_editor.selected->position.y);
}
else
{
    ImGui::Text("No block selected");
}

ImGui::End();

}

inline void DrawEditorUI()
{
    DrawBlockPalette();
    DrawStage();
    DrawCodeEditor();
}