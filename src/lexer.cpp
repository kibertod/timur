#include "lexer.h"
#include <string>
#include <cctype>

Lexer::Lexer(std::string src) :
    src(src),
    pos(0),
    current() { };

Token Lexer::NextToken() {
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

Token Lexer::Peek() { return current; }
