#include "parser.tab.hpp"
#include <sstream>
#include <fstream>
#include "seman.h"
#include "print.hpp"

int main() {
    ParserContext ctx;
    ctx.row = 1;

    std::ifstream file("test.tmr");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    ctx.src = buffer.str();
    ctx.pos = 0;

    yy::parser p(ctx);
    p.parse();

    // for (size_t i = 0; i < ctx.root.size(); i++) {
    //     auto node = ctx.root[i];
    //     print(node, 0);
    // }
    Analyzer analyzer(ast::Root { ctx.root });
    analyzer.analyze();
}
