#include "lexer.h"
#include <string>
#include <vector>

Lexer::Lexer(std::string filename) {
    file = std::fstream(filename);
    current_sequence = std::vector<Token>();
};

Lexer::~Lexer() { file.close(); }

Token Lexer::NextToken() {
    return Token { TokenType::COMMENT, std::string("// this is comment") };
}
