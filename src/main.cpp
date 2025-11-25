#include "parser.tab.hpp"
#include <sstream>
#include <fstream>
#include "seman.h"
#include "codegen.h"

int main() {
    std::ifstream file("test.tmr");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }

    std::ifstream core("src/core.tmr");
    if (!core) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }

    std::stringstream core_src;
    std::stringstream test_src;
    core_src << core.rdbuf();
    test_src << file.rdbuf();

    ParserContext ctx;
    ctx.row = 1;
    ctx.src = core_src.str();
    ctx.src += test_src.str();
    ctx.pos = 0;
    yy::parser parser(ctx);
    parser.parse();

    // for (size_t i = 0; i < ctx.root.size(); i++) {
    //     auto node = ctx.root[i];
    //     print(node, 0);
    // }
    Analyzer analyzer(ast::Root { ctx.root });
    analyzer.analyze();
    if (!analyzer.error) {
        ctx = {};
        ctx.row = 1;
        ctx.src += test_src.str();
        ctx.pos = 0;
        yy::parser parser(ctx);
        parser.parse();

        Codegen codegen { ast::Root { ctx.root } };
        codegen.generate();
    }
}
