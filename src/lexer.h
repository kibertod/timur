#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include "parser.tab.hpp"

struct ParserContext {
    size_t pos;
    std::string src;
};
