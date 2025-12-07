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

enum class ArgType {
    None,
    Number,
    Text,
    Boolean,
    Color
};

struct BlockArgument {
    ArgType type;
    std::string value;
};

#endif // BLOCKTYPES_HPP