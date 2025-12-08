#ifndef BLOCKTYPES_HPP
#define BLOCKTYPES_HPP

#include <string>
#include <vector>

enum class BlockShape {
    Hat,
    Stack,
    CBlock,
    Boolean,
    Reporter,
    Cap
};

// Argument types
enum class ArgType {
    None,
    Number,
    Text,
    Boolean,
    Color,
    Socket    // <-- added to fix EditorUI using sockets
};

// Defines one argument field on a block
struct BlockArgument {
    std::string name;       // name of the argument ("Steps", "Degrees", etc.)
    ArgType type = ArgType::None;
    std::string value;
    std::string selectedBackdrop;

    // Different types of values:
    float numberValue = 0.0f;
    bool boolValue = false;
    std::string textValue = "";
    std::string colorValue = "#ffffff";

    // For reporter/socket arguments:
    void* socketValue = nullptr; // can be changed to Block* later if desired
};

#endif // BLOCKTYPES_HPP
