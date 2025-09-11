#include "lexer.h"
#include <string>

Lexer::Lexer(std::string src) :
    src(src),
    pos(0),
    current() { };

Token Lexer::NextToken() { }

Token Lexer::Peek() { return current; }
