#include "parser.tab.hpp"
#include <sstream>
#include <fstream>
#include "seman.h"
#include "codegen.h"

// #include "print.hpp"

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

    Analyzer analyzer(ast::Root { ctx.root });
    analyzer.analyze();

    // for (size_t i = 0; i < analyzer.ast().classes.size(); i++) {
    //     auto node = analyzer.ast().classes[i];
    //     print(node, 0);
    // }

    if (!analyzer.error) {
        Codegen codegen { analyzer.ast() };
        codegen.generate();
    }
}
