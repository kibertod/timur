#include "parser.tab.hpp"
#include "lexer.h"

int main() {
    ParserContext ctx;
    yy::parser p(&ctx);
    p.parse();
}
