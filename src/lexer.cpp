#include "lexer.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

Lexer::Lexer() :
    pos(0),
    current() {

    std::ifstream file("test.tmr");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    src = buffer.str();
};

Token Lexer::nextTokenUnsaved() {
    std::string buffer;

    while (true) {
        if (pos >= src.length()) {
            return Token(TokenType::Eof);
        }
        if (std::isspace(src[pos])) {
            pos++;
            continue;
        }
        if (std::isdigit(src[pos])) {
            bool real = false;
            buffer += src[pos];
            pos++;
            while (std::isdigit(src[pos]) || src[pos] == '.') {
                if (src[pos] == '.') {
                    if (real) {
                        return Token(TokenType::LitInt, buffer);
                    }
                    real = true;
                }
                buffer += src[pos];
                pos++;
            }
            if (buffer[buffer.length() - 1] == '.')
                return Token(TokenType::Err,
                    std::string("\'.\' at the end of a number"));
            if (real) {
                return Token(TokenType::LitReal, buffer);
            } else {
                return Token(TokenType::LitInt, buffer);
            }
        }
        if (src[pos] == '"') {
            pos++;
            while (src[pos] != '"') {
                buffer += src[pos];
                pos++;
            }
            pos++;
            return Token(TokenType::LitStr, buffer);
        }
        switch (src[pos]) {
        case '(':
            pos++;
            return Token(TokenType::LParen);
        case ')':
            pos++;
            return Token(TokenType::RParen);
        case '.':
            pos++;
            return Token(TokenType::Access);
        case ',':
            pos++;
            return Token(TokenType::Comma);
        case ':':
            pos++;
            if (src[pos] == '=') {
                pos++;
                return Token(TokenType::Assign);
            }
            return Token(TokenType::Colon);
        case '[':
            pos++;
            return Token(TokenType::LBracket);
        case ']':
            pos++;
            return Token(TokenType::RBracket);
        case '/':
            pos++;
            if (src[pos] == '/') {
                while (true) {
                    pos++;
                    if (std::isspace(src[pos]) && !std::isblank(src[pos]))
                        break;
                }
            }
            continue;
        }

        if (std::isalpha(src[pos]) || src[pos] == '_') {
            buffer += src[pos];
            pos++;
            while (std::isalpha(src[pos]) || src[pos] == '_' ||
                   std::isdigit(src[pos])) {
                buffer += src[pos];
                pos++;
            }
            if (buffer == "var")
                return Token(TokenType::KVar);
            if (buffer == "while")
                return Token(TokenType::KWhile);
            if (buffer == "loop")
                return Token(TokenType::KLoop);
            if (buffer == "if")
                return Token(TokenType::KIf);
            if (buffer == "then")
                return Token(TokenType::KThen);
            if (buffer == "else")
                return Token(TokenType::KElse);
            if (buffer == "is")
                return Token(TokenType::KIs);
            if (buffer == "end")
                return Token(TokenType::KEnd);
            if (buffer == "class")
                return Token(TokenType::KClass);
            if (buffer == "method")
                return Token(TokenType::KMethod);
            if (buffer == "extends")
                return Token(TokenType::KExtends);
            if (buffer == "this")
                return Token(TokenType::KThis);
            if (buffer == "super")
                return Token(TokenType::KSuper);
            if (buffer == "true")
                return Token(TokenType::KTrue);
            if (buffer == "false")
                return Token(TokenType::KFalse);
            return Token(TokenType::Identifier, buffer);
        }
        return Token(TokenType::Err, "Unexpected character");
    }
}

Token Lexer::NextToken() {
    current = nextTokenUnsaved();
    return current;
}

Token Lexer::Peek() { return current; }

Lexer lexer = Lexer();

namespace yy {
    parser::symbol_type yylex() {
        Token token = lexer.NextToken();
        switch (token.type) {

        case TokenType::KVar:
            return yy::parser::make_KVar();
        case TokenType::KWhile:
            return yy::parser::make_KWhile();
        case TokenType::KLoop:
            return yy::parser::make_KLoop();
        case TokenType::KIf:
            return yy::parser::make_KIf();
        case TokenType::KThen:
            return yy::parser::make_KThen();
        case TokenType::KElse:
            return yy::parser::make_KElse();
        case TokenType::KIs:
            return yy::parser::make_KIs();
        case TokenType::KEnd:
            return yy::parser::make_KEnd();
        case TokenType::KClass:
            return yy::parser::make_KClass();
        case TokenType::KMethod:
            return yy::parser::make_KMethod();
        case TokenType::KExtends:
            return yy::parser::make_KExtends();
        case TokenType::KThis:
            return yy::parser::make_KThis();
        case TokenType::KSuper:
            return yy::parser::make_KSuper();
        case TokenType::KTrue:
            return yy::parser::make_KTrue();
        case TokenType::KFalse:
            return yy::parser::make_KFalse();
        case TokenType::Identifier:
            return yy::parser::make_Identifier(token.value);
        case TokenType::Access:
            return yy::parser::make_Access();
        case TokenType::Assign:
            return yy::parser::make_Assign();
        case TokenType::LParen:
            return yy::parser::make_LParen();
        case TokenType::RParen:
            return yy::parser::make_RParen();
        case TokenType::Colon:
            return yy::parser::make_Colon();
        case TokenType::Comma:
            return yy::parser::make_Comma();
        case TokenType::LBracket:
            return yy::parser::make_LBracket();
        case TokenType::RBracket:
            return yy::parser::make_RBracket();
        case TokenType::LitStr:
            return yy::parser::make_LitStr(token.value);
        case TokenType::LitInt:
            return yy::parser::make_LitInt(token.value);
        case TokenType::LitReal:
            return yy::parser::make_LitReal(token.value);
        case TokenType::Eof:
            return yy::parser::make_YYEOF();
        case TokenType::Err:
            return yy::parser::make_Err(token.value);
        }
    }
}
