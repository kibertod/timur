#include <iostream>
#include "lexer.h"

int main() {
    Lexer lexer = Lexer(std::string("test.tmr"));
    Token token = lexer.NextToken();
    std::cout << token.value << std::endl;
}
