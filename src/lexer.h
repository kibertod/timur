#include <fstream>
#include <string>
#include <vector>

enum TokenType { KEYWORD, IDENTIFIER, OPERATOR, DELIMITER, LITERAL, COMMENT };

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
private:
    std::fstream file;
    std::vector<Token> current_sequence;

public:
    Lexer(std::string target);
    ~Lexer();
    Token NextToken();
};
