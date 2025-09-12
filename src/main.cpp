#include "parser.tab.hpp"

int main() {
    yy::parser p;
    p.parse();
}
