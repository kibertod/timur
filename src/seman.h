#pragma once

#include "ast.h"
#include <optional>
#include <unordered_map>

static ast::TypeName void_ { { "Void" }, {} };

typedef std::unordered_map<std::string, ast::Variable> Variables;

class Analyzer {
private:
    ast::TypeName check_expression(
        const ast::Expression& expression, Variables& variables);

    bool check_arguments(ast::Arguments arguments,
        std::vector<ast::Expression> expressions, Variables& variables);

    std::optional<ast::MemberDeclaration> get_memeber(
        const ast::TypeName& class_, const ast::Identifier& identifier);

    std::optional<ast::MemberDeclaration::Constructor> get_constructor(
        const ast::TypeName& class_);

    void check_statements(
        const std::vector<ast::Statement>& statements, Variables& variables);

    ast::Root ast;

public:
    void analyze();
    Analyzer(ast::Root ast) :
        ast(ast) { };
};
