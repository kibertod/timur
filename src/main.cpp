#include "parser.tab.hpp"
#include "lexer.h"
#include <sstream>

int main() {
    ParserContext ctx;

    std::ifstream file("test.tmr");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    ctx.src = buffer.str();
    ctx.pos = 0;

    yy::parser p(&ctx);
    p.parse();
}
