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
        res += "  ";
    }
    return res;
}

std::string expression_heaading(std::string text) {
    return std::string("\033[1m\033[31m") + text + "\033[0m";
}

std::string misc_heaading(std::string text) {
    return std::string("\033[1m\033[32m") + text + "\033[0m";
}

std::string statement_heading(std::string text) {
    return std::string("\033[1m\033[35m") + text + "\033[0m";
}

std::string member_heading(std::string text) {
    return std::string("\033[1m\033[36m") + text + "\033[0m";
}

std::string class_heading(std::string text) {
    return std::string("\033[1m\033[34m") + text + "\033[0m";
}

std::string ident(std::string text) {
    return std::string("\033[1m\033[33m") + text + "\033[0m";
}

void print(ast::TypeName type_name, size_t depth) {
    std::print("{0}{1}\n{0}{2} {3}\n{0}{4} \n", tabs(depth),
        misc_heaading("TYPENAME"), ident("name:"), type_name.name.name,
        ident("generic arguments:"));
    for (size_t i = 0; i < type_name.generic_arguments.size(); i++) {
        print(type_name.generic_arguments[i], depth + 1);
    }
}

void print(ast::Expression expression, size_t depth) {
    if (auto access =
            std::get_if<ast::Expression::MemberAccess>(&expression.value)) {
        std::print("{0}{1}\n{0}{2} \n", tabs(depth),
            expression_heaading("MEMBER ACCESS"),
            ident("preceeding expression:"));
        print(*access->object, depth + 1);
        std::print(
            "{0}{1} {2}\n", tabs(depth), ident("member:"), access->member.name);
    }
    if (auto access =
            std::get_if<ast::Expression::ThisAccess>(&expression.value)) {
        std::print("{0}{1}\n", tabs(depth), expression_heaading("THIS ACCESS"));
        std::print(
            "{0}{1} {2}\n", tabs(depth), ident("member:"), access->member.name);
    }
    if (auto call =
            std::get_if<ast::Expression::MethodCall>(&expression.value)) {
        std::print("{0}{1}\n{0}{2} \n", tabs(depth),
            expression_heaading("METHOD CALL"),
            ident("preceeding expression:"));
        print(*call->object, depth + 1);
        std::print("{0}{1} {2}\n{0}{3}\n", tabs(depth), ident("method:"),
            call->method.name, ident("arguments:"));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto call =
            std::get_if<ast::Expression::ConstructorCall>(&expression.value)) {
        std::print("{0}{1}\n{0}{2} \n", tabs(depth),
            expression_heaading("CONSTRUCTOR CALL"), ident("type:"));
        print(call->type_name, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("arguments:"));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto call =
            std::get_if<ast::Expression::SuperCall>(&expression.value)) {
        std::print("{0}{1}\n", tabs(depth), expression_heaading("SUPER CALL"));
        std::print("{0}{1}\n", tabs(depth), ident("arguments:"));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto call = std::get_if<ast::Expression::ThisCall>(&expression.value)) {
        std::print("{0}{1}\n", tabs(depth), expression_heaading("THIS CALL"));
        std::print("{0}{1}\n", tabs(depth), ident("arguments:"));
        for (size_t i = 0; i < call->arguments.size(); i++) {
            auto argument = call->arguments[i];
            print(argument, depth + 1);
        }
    }
    if (auto identifier = std::get_if<ast::Identifier>(&expression.value)) {
        std::print("{0}{1} {2}\n", tabs(depth), misc_heaading("IDENTIFIER"),
            identifier->name);
    }
    if (auto literal =
            std::get_if<ast::Expression::Literal>(&expression.value)) {
        std::cout << tabs(depth) << expression_heaading("LITERAL ");
        switch (literal->type) {
        case ast::Expression::Literal::Int:
            std::cout << ident("Integer ") << literal->value << std::endl;
            break;
        case ast::Expression::Literal::Str:
            std::cout << ident("String ") << literal->value << std::endl;
            break;
        case ast::Expression::Literal::Bool:
            std::cout << ident("Boolean ") << literal->value << std::endl;
            break;
        case ast::Expression::Literal::Real:
            std::cout << ident("Real ") << literal->value << std::endl;
            break;
        }
    }
}

void print(ast::Variable* variable, size_t depth) {
    std::print("{0}{1}\n{0}{2} \n", tabs(depth),
        statement_heading("VARIABLE DECLARATION"), ident("type:"));
    print(variable->type_name, depth + 1);
    std::print(
        "{0}{1} {2}\n", tabs(depth), ident("name:"), variable->name.name);
    std::print("{0}{1} ", tabs(depth), ident("assigned value:"));
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
        std::print("{0}{1}\n{0}{2}\n", tabs(depth),
            statement_heading("ASSIGNMENT"), ident("lsh:"));
        print(assignment->left, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("rhs:"));
        print(assignment->right, depth + 1);
    }
    if (auto variable = std::get_if<ast::Variable>(&statement.value)) {
        print(variable, depth);
    }
    if (auto while_ = std::get_if<ast::Statement::While>(&statement.value)) {
        std::print("{0}{1}\n{0}{2}\n", tabs(depth),
            statement_heading("WHILE LOOP"), ident("condition:"));
        print(while_->condition, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("body:"));
        for (size_t i = 0; i < while_->body.size(); i++) {
            print(while_->body[i], depth + 1);
        }
    }
    if (auto if_ = std::get_if<ast::Statement::If>(&statement.value)) {
        std::print("{0}{1}\n{0}{2}\n", tabs(depth),
            statement_heading("IF STATEMENT"), ident("condition:"));
        print(if_->condition, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("body:"));
        for (size_t i = 0; i < if_->body.size(); i++) {
            print(if_->body[i], depth + 1);
        }
        for (size_t i = 0; i < if_->elifs.size(); i++) {
            std::print("{0}{1}\n{0}{2}\n", tabs(depth + 1),
                statement_heading("ELSE IF"), ident("condition:"));
            print(if_->elifs[i].condition, depth + 2);
            std::print("{0}{1}\n", tabs(depth + 1), ident("body:"));
            for (size_t j = 0; j < if_->elifs[i].body.size(); j++) {
                print(if_->elifs[i].body[j], depth + 2);
            }
        }
        std::print("{0}{1}\n", tabs(depth), ident("else body:"));
        for (size_t i = 0; i < if_->else_body.size(); i++) {
            print(if_->else_body[i], depth + 1);
        }
    }
    if (auto return_ = std::get_if<ast::Statement::Return>(&statement.value)) {
        std::print("{0}{1}\n{0}{2}\n", tabs(depth), statement_heading("RETURN"),
            ident("expression:"));
        print(return_->value, depth + 1);
    }
}

void print(
    const std::vector<std::pair<ast::TypeName, ast::Identifier>>& arguments,
    size_t depth) {
    for (size_t i = 0; i < arguments.size(); i++) {
        std::print("{0}{1}\n{0}{2} {3}\n{0}{4} \n", tabs(depth),
            expression_heaading("ARGUMENT"), ident("name:"),
            arguments[i].second.name, ident("type:"));
        print(arguments[i].first, depth + 1);
    }
}

void print(ast::MemberDeclaration declaration, size_t depth) {
    if (auto method =
            std::get_if<ast::MemberDeclaration::Method>(&declaration.value)) {
        std::print("{0}{1}\n{0}{2} {3}\n{0}{4}\n", tabs(depth),
            member_heading("METHOD"), ident("name:"), method->name.name,
            ident("return type:"));
        print(method->return_type, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("arguments:"));
        print(method->arguments, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("body:"));
        for (size_t i = 0; i < method->body.size(); i++) {
            print(method->body[i], depth + 1);
        }
    }
    if (auto constructor = std::get_if<ast::MemberDeclaration::Constructor>(
            &declaration.value)) {
        std::print("{0}{1}\n", tabs(depth), member_heading("CONSTRUCTOR"));
        std::print("{0}{1}\n", tabs(depth), ident("arguments:"));
        print(constructor->arguments, depth + 1);
        std::print("{0}{1}\n", tabs(depth), ident("body:"));
        for (size_t i = 0; i < constructor->body.size(); i++) {
            print(constructor->body[i], depth + 1);
        }
    }
    if (auto variable = std::get_if<ast::Variable>(&declaration.value)) {
        print(variable, depth);
    }
}

void print(ast::Class class_, size_t depth) {
    std::print("{0}{1}\n{2} \n", tabs(depth), class_heading("CLASS"),
        ident("type_name:"));
    print(class_.name, depth + 1);
    std::print("{0}{1}\n", tabs(depth), ident("extends:"));
    for (size_t i = 0; i < class_.extends.size(); i++) {
        std::print("{0}{1}\n", tabs(depth + 1), class_.extends[i].name);
    }
    std::print("{0}{1}\n", tabs(depth), ident("members:"));
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
