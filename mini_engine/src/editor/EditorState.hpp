#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include "Block.hpp"

// ------------------------------------------------------------
// Global Editor State
// ------------------------------------------------------------
struct Sprite {
    std::string name;
    Sprite(const std::string& n) : name(n) {}
};

struct EditorState {
    std::vector<Block*> blocks;
    std::vector<Sprite*> sprites; // Added for sprite panel

    Block* selected = nullptr;
    Block* hoveredSocket = nullptr;
    Block* hoveredBoolean = nullptr;

    // Running state
    bool running = false;
    Sprite* selectedSprite = nullptr;

    // Execution
    size_t currentBlockIndex = 0;
    std::vector<Block*> executionOrder;

    EditorState() {
        AddSprite("Sprite1");
        selectedSprite = sprites[0];
    }

    // --------------------------------------------------------
    // Add a new sprite
    // --------------------------------------------------------
    void AddSprite(const std::string& name) {
        sprites.push_back(new Sprite(name));
    }

    // --------------------------------------------------------
    // Remove a sprite by index
    // --------------------------------------------------------
    void RemoveSprite(size_t index) {
        if (index >= sprites.size()) return;
        delete sprites[index];
        sprites.erase(sprites.begin() + index);
    }

    // --------------------------------------------------------
    // Clear all sprites
    // --------------------------------------------------------
    void ClearSprites() {
        for (Sprite* s : sprites) delete s;
        sprites.clear();
    }

};

inline EditorState g_editor;

// ------------------------------------------------------------
// RECURSIVE execution builder
// ------------------------------------------------------------
inline void FlattenBlock(Block* b, std::vector<Block*>& out)
{
    if (!b) return;

    out.push_back(b);

    if (b->inside)
        FlattenBlock(b->inside, out);

    if (b->next)
        FlattenBlock(b->next, out);

}

// ------------------------------------------------------------
// Build execution order from all top-level stack/C-blocks
// ------------------------------------------------------------
inline void BuildExecutionOrder()
{
    g_editor.executionOrder.clear();

    for (Block* b : g_editor.blocks)
    {
        if (b->shape == BlockShape::Stack || b->shape == BlockShape::CBlock)
        {
            FlattenBlock(b, g_editor.executionOrder);
        }
    }

    g_editor.currentBlockIndex = 0;

}

// ------------------------------------------------------------
// Step execution — executes ONE block per call
// ------------------------------------------------------------
inline void StepExecution()
{
    if (!g_editor.running) return;

    if (g_editor.currentBlockIndex >= g_editor.executionOrder.size())
    {
        g_editor.running = false;
        g_editor.currentBlockIndex = 0;
        return;
    }

    Block* b = g_editor.executionOrder[g_editor.currentBlockIndex];

    // ---------------------------
    // Placeholder behavior
    // ---------------------------
    if (b->text == "Move Steps")
    {
        // TODO: use arguments to move sprite
    }
    else if (b->text == "Turn")
    {
        // TODO: use args to rotate sprite
    }
    else if (b->text == "Repeat")
    {
        // TODO: Scratch-style loop
    }

    g_editor.currentBlockIndex++;

}


#endif // EDITORSTATE_HPP