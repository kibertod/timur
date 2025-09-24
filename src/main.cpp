#include "parser.tab.hpp"
#include <format>
#include <sstream>
#include <fstream>
#include <utility>
#include <variant>
#include <print>
#include <vector>

std::string tabs(size_t depth) {
    std::string res = "";
    for (size_t i = 0; i < depth; i++) {
        res += "\t";
    }
    return res;
}

void print(ast::TypeName type_name, size_t depth) {
    std::print("{0}TYPENAME\n{0}name: {1}\n{0}generic arguments: \n",
        tabs(depth), type_name.name.name);
    for (size_t i = 0; i < type_name.generic_arguments.size(); i++) {
        print(type_name.generic_arguments[i], depth + 1);
    }
}

void print(ast::Expression expression, size_t depth) {
    if (auto access =
            std::get_if<ast::Expression::MemberAccess>(&expression.value)) {
        std::print(
            "{0}MEMBER ACCESS\n{0}preceeding expression: \n", tabs(depth));
        print(*access->object, depth + 1);
        std::print("{}member: {}\n", tabs(depth), access->member.name);
    }
    if (auto access =
            std::get_if<ast::Expression::ThisAccess>(&expression.value)) {
        std::print("{0}THIS ACCESS\n", tabs(depth));
        std::print("{0}member: {1}\n", tabs(depth), access->member.name);
    }
    if (auto call =
            std::get_if<ast::Expression::MethodCall>(&expression.value)) {
        std::print("{0}METHOD CALL\n{0}preceeding expression: \n", tabs(depth));
        print(*call->object, depth + 1);
        std::print(
            "{0}method: {1}\n{0}arguments:\n", tabs(depth), call->method.name);
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto call =
            std::get_if<ast::Expression::ConstructorCall>(&expression.value)) {
        std::print("{0}CONSTRUCTOR CALL\n{0}type: \n", tabs(depth));
        print(call->type_name, depth + 1);
        std::print("{0}arguments:\n", tabs(depth));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto call =
            std::get_if<ast::Expression::SuperCall>(&expression.value)) {
        std::print("{0}SUPER CALL\n", tabs(depth));
        std::print("{0}arguments:\n", tabs(depth));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto call = std::get_if<ast::Expression::ThisCall>(&expression.value)) {
        std::print("{0}THIS CALL\n", tabs(depth));
        std::print("{0}arguments:\n", tabs(depth));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto identifier = std::get_if<ast::Identifier>(&expression.value)) {
        std::print("{0}IDENTIFIER\n{0}{1}\n", tabs(depth), identifier->name);
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

void print(ast::Variable* variable, size_t depth) {
    std::print("{0}VARIALBE DECLARATION\n{0}type: \n", tabs(depth));
    print(variable->type_name, depth + 1);
    std::print("{0}name: {1}\n", tabs(depth), variable->name.name);
    std::print("{0}assigned value: ", tabs(depth));
    if (variable->value.has_value()) {
        std::cout << std::endl;
        print(*variable->value, depth + 1);
    } else {
        std::cout << "undefined";
    }
}

void print(ast::Statement statement, size_t depth) {
    if (auto expression = std::get_if<ast::Expression>(&statement.value)) {
        print(*expression, depth);
    }
    if (auto assignment =
            std::get_if<ast::Statement::Assignment>(&statement.value)) {
        std::print("{0}ASSIGNMENT\n{0}lsh:\n", tabs(depth));
        print(assignment->left, depth + 1);
        std::print("{0}rhs:\n", tabs(depth));
        print(assignment->right, depth + 1);
    }
    if (auto variable = std::get_if<ast::Variable>(&statement.value)) {
        print(variable, depth);
    }
    if (auto while_ = std::get_if<ast::Statement::While>(&statement.value)) {
        std::print("{0}WHILE LOOP\n{0}condition:\n", tabs(depth));
        print(while_->condition, depth + 1);
        std::print("{0}body:\n", tabs(depth));
        for (size_t i = 0; i < while_->body.size(); i++) {
            print(while_->body[i], depth + 1);
        }
    }
    if (auto if_ = std::get_if<ast::Statement::If>(&statement.value)) {
        std::print("{0}IF STATEMENT\n{0}condition:\n", tabs(depth));
        print(if_->condition, depth + 1);
        std::print("{0}body:\n", tabs(depth));
        for (size_t i = 0; i < if_->body.size(); i++) {
            print(if_->body[i], depth + 1);
        }
        for (size_t i = 0; i < if_->elifs.size(); i++) {
            std::print("{0}ELSE IF\n{0}condition:\n", tabs(depth + 1));
            print(if_->elifs[i].condition, depth + 2);
            std::print("{0}body:\n", tabs(depth + 1));
            for (size_t j = 0; j < if_->elifs[i].body.size(); j++) {
                print(if_->elifs[i].body[j], depth + 2);
            }
        }
        std::print("{0}else body:\n", tabs(depth));
        for (size_t i = 0; i < if_->else_body.size(); i++) {
            print(if_->else_body[i], depth + 1);
        }
    }
    if (auto return_ = std::get_if<ast::Statement::Return>(&statement.value)) {
        std::print("{0}RETURN\n{0}expression:\n", tabs(depth));
        print(return_->value, depth + 1);
    }
}

void print(
    const std::vector<std::pair<ast::TypeName, ast::Identifier>>& arguments,
    size_t depth) {
    for (size_t i = 0; i < arguments.size(); i++) {
        std::print("{0}ARGUMENT\n{0}name: {1}\n{0}type: \n", tabs(depth),
            arguments[i].second.name);
        print(arguments[i].first, depth + 1);
    }
}

void print(ast::MemberDeclaration declaration, size_t depth) {
    if (auto method =
            std::get_if<ast::MemberDeclaration::Method>(&declaration.value)) {
        std::print("{0}METHOD\n{0}name: {1}\n{0}return type:\n", tabs(depth),
            method->name.name);
        print(method->return_type, depth + 1);
        std::print("{0}arguments:\n", tabs(depth));
        print(method->arguments, depth + 1);
        std::print("{0}body:\n", tabs(depth));
        for (size_t i = 0; i < method->body.size(); i++) {
            print(method->body[i], depth + 1);
        }
    }
    if (auto constructor = std::get_if<ast::MemberDeclaration::Constructor>(
            &declaration.value)) {
        std::print("{0}CONSTRUCTOR\n", tabs(depth));
        std::print("{0}arguments:\n", tabs(depth));
        print(constructor->arguments, depth + 1);
        std::print("{0}body:\n", tabs(depth));
        for (size_t i = 0; i < constructor->body.size(); i++) {
            print(constructor->body[i], depth + 1);
        }
    }
    if (auto variable = std::get_if<ast::Variable>(&declaration.value)) {
        print(variable, depth);
    }
}

void print(ast::Class class_, size_t depth) {
    std::print("{}CLASS\ntype_name: \n", tabs(depth));
    print(class_.name, depth + 1);
    std::print("{}extends:\n", tabs(depth));
    for (size_t i = 0; i < class_.extends.size(); i++) {
        std::print("{0}{1}", tabs(depth + 1), class_.extends[i].name);
    }
    std::print("{}members:\n", tabs(depth));
    for (size_t i = 0; i < class_.body.size(); i++) {
        print(class_.body[i], depth + 1);
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
        auto node = ctx.root[i];
        print(node, 0);
    }
}
