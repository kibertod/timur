#include "parser.tab.hpp"
#include <sstream>
#include <fstream>
#include <variant>

std::string tabs(size_t depth) {
    std::string res = "";
    for (size_t i = 0; i < depth; i++) {
        res += "\t";
    }
    return res;
}

void print(ast::Expression expression, size_t depth) {
    if (auto access =
            std::get_if<ast::Expression::MemberAccess>(&expression.value)) {
        std::cout << tabs(depth) << "MEMBER ACCESS" << std::endl
                  << tabs(depth) << "preceeding expression: " << std::endl;
        print(*access->object, depth + 1);
        std::cout << tabs(depth) << "member: " << access->member.name
                  << std::endl;
    }
    if (auto call =
            std::get_if<ast::Expression::MethodCall>(&expression.value)) {
        std::cout << tabs(depth) << "METHOD CALL" << std::endl
                  << tabs(depth) << "preceeding expression: " << std::endl;
        print(*call->object, depth + 1);
        std::cout << tabs(depth) << "member: " << call->method.name
                  << std::endl;
        std::cout << tabs(depth) << "arguments: " << std::endl;
        for (size_t i = 0; i < call->argumens.size(); i++) {
            auto argument = call->argumens[i];
            print(argument, depth + 1);
        }
    }
    if (auto identifier = std::get_if<ast::Identifier>(&expression.value)) {
        std::cout << tabs(depth) << "IDENTIFIER" << std::endl
                  << tabs(depth) << identifier->name << std::endl;
    }
    if (auto literal =
            std::get_if<ast::Expression::Literal>(&expression.value)) {
        std::cout << tabs(depth) << "LITERAL ";
        switch (literal->type) {
        case ast::Expression::Literal::Int:
            std::cout << "Integer " << literal->value << std::endl;
            break;
        case ast::Expression::Literal::Str:
            std::cout << "String " << literal->value << std::endl;
            break;
        case ast::Expression::Literal::Bool:
            std::cout << "Boolean " << literal->value << std::endl;
            break;
        case ast::Expression::Literal::Real:
            std::cout << "Real " << literal->value << std::endl;
            break;
        }
    }
}

int main() {
    ParserContext ctx;

    std::ifstream file("test.tmr");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    ctx.src = buffer.str();
    ctx.pos = 0;

    yy::parser p(ctx);
    p.parse();

    for (size_t i = 0; i < ctx.root.size(); i++) {
        auto expression = ctx.root[i];
        print(expression, 0);
    }
}
