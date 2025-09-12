#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include "parser.tab.hpp"

enum class TokenType {
    KVar,
    KWhile,
    KLoop,
    KIf,
    KThen,
    KElse,
    KIs,
    KEnd,
    KClass,
    KMethod,
    KExtends,
    KThis,
    KSuper,
    KTrue,
    KFalse,
    Identifier,
    Access,
    Assign,
    LParen,
    RParen,
    Colon,
    Comma,
    LBracket,
    RBracket,
    LitStr,
    LitInt,
    LitReal,
    Eof,
    Err
};

struct Token {
    TokenType type;
    std::string value;
    Token() :
        type(TokenType::Eof),
        value() { };
    Token(TokenType type) :
        type(type),
        value() { };
    Token(TokenType type, const std::string& value) :
        type(type),
        value(value) { };
};

class Lexer {
private:
    std::string src;
    size_t pos;
    Token current;
    Token nextTokenUnsaved();

public:
    Lexer();
    Token NextToken();
    Token Peek();
};

yy::parser::symbol_kind::symbol_kind_type yylex();

struct ParserContext {
    Lexer lexer;
};
