#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include "Block.hpp"
#include <vector>

struct EditorState {
    std::vector<Block*> blocks;
    Block* selected = nullptr;

    Block* hoveredSocket = nullptr;
    Block* hoveredBoolean = nullptr;

    // Running state
    bool running = false;

    // Script execution
    size_t currentBlockIndex = 0; // index of the currently executing block
    std::vector<Block*> executionOrder; // flattened stack of blocks to execute

};

// Global editor state
inline EditorState g_editor;

// Utility function to flatten blocks into execution order (simple linear execution)
inline void BuildExecutionOrder()
{
    g_editor.executionOrder.clear();
    for (Block* b : g_editor.blocks)
    {
        if (b->shape == BlockShape::Stack)
            g_editor.executionOrder.push_back(b);
        else if (b->shape == BlockShape::CBlock)
        {
            g_editor.executionOrder.push_back(b);
            for (Block* child : b->children) {
                g_editor.executionOrder.push_back(child);
            }
        }
        g_editor.currentBlockIndex = 0;
    }
}

// Step the execution by one block
inline void StepExecution() {
    if (!g_editor.running) return;
    if (g_editor.currentBlockIndex >= g_editor.executionOrder.size()) {
        g_editor.running = false; // stop when finished
        g_editor.currentBlockIndex = 0;
        return;
    }

    Block* b = g_editor.executionOrder[g_editor.currentBlockIndex];

    // Execute the block logic (placeholder)
    if (b->text == "Move Steps")
    {
        // Implement movement logic here
    }
    else if (b->text == "Turn")
    {
        // Implement turn logic here
    }

    g_editor.currentBlockIndex++;

}

#endif // EDITORSTATE_HPP
