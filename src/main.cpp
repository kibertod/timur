#include <iostream>
#include <sstream>
#include "lexer.h"

int main() {
    std::ifstream file("test.tmr");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string src = buffer.str();
    Lexer lexer = Lexer(src);
    while (true) {
        Token current = lexer.NextToken();
        switch (current.type) {
        case TokenType::KVar:
            std::cout << "var ";
            break;
        case TokenType::KWhile:
            std::cout << "while ";
            break;
        case TokenType::KLoop:
            std::cout << "loop ";
            break;
        case TokenType::KIf:
            std::cout << "if ";
            break;
        case TokenType::KThen:
            std::cout << "then ";
            break;
        case TokenType::KElse:
            std::cout << "else ";
            break;
        case TokenType::KIs:
            std::cout << "is ";
            break;
        case TokenType::KEnd:
            std::cout << "end ";
            break;
        case TokenType::KClass:
            std::cout << "class ";
            break;
        case TokenType::KMethod:
            std::cout << "method ";
            break;
        case TokenType::KExtends:
            std::cout << "extends ";
            break;
        case TokenType::KThis:
            std::cout << "this ";
            break;
        case TokenType::KSuper:
            std::cout << "super ";
            break;
        case TokenType::KTrue:
            std::cout << "true ";
            break;
        case TokenType::KFalse:
            std::cout << "false ";
            break;
        case TokenType::Identifier:
            std::cout << "identifier ";
            break;
        case TokenType::Access:
            std::cout << "access ";
            break;
        case TokenType::Assign:
            std::cout << "assign ";
            break;
        case TokenType::LParen:
            std::cout << "lParen ";
            break;
        case TokenType::RParen:
            std::cout << "rParen ";
            break;
        case TokenType::Colon:
            std::cout << "colon ";
            break;
        case TokenType::Comma:
            std::cout << "comma ";
            break;
        case TokenType::LBracket:
            std::cout << "lBracket ";
            break;
        case TokenType::RBracket:
            std::cout << "rBracket ";
            break;
        case TokenType::LitStr:
            std::cout << "litStr ";
            break;
        case TokenType::LitInt:
            std::cout << "litInt ";
            break;
        case TokenType::LitReal:
            std::cout << "litReal ";
            break;
        case TokenType::Eof:
            std::cout << "eof ";
            break;
        case TokenType::Err:
            std::cout << "err ";
            break;
        }
        std::cout << current.value << std::endl;
        if (current.type == TokenType::Eof || current.type == TokenType::Err)
            break;
    }
}
