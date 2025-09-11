#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

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
    LBracket,
    RBracket,
    LitStr,
    LitInt,
    Eof
};

struct Token {
    TokenType type;
    std::string value;
    Token() :
        type(TokenType::Eof),
        value() { };
};

class Lexer {
private:
    std::string src;
    Token current;
    size_t pos;

public:
    Lexer(std::string src);
    Token NextToken();
    Token Peek();
};
