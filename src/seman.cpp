#pragma once

#include <vector>
#include <print>
#include "seman.h"
#include "print.hpp"

std::optional<ast::MemberDeclaration> Analyzer::get_memeber(
    const ast::TypeName& class_, const ast::Identifier& identifier) {
    for (size_t i = 0; i < ast.classes.size(); i++) {
        if (ast.classes[i].name.name.name == class_.name.name) {
            for (size_t j = 0; j < ast.classes[i].body.size(); j++) {
                ast::MemberDeclaration member = ast.classes[i].body[j];
                if (auto variable = std::get_if<ast::Variable>(&member.value)) {
                    if (variable->name.name == identifier.name)
                        return member;
                }
                if (auto method = std::get_if<ast::MemberDeclaration::Method>(
                        &member.value)) {
                    if (method->name.name == identifier.name)
                        return member;
                }
            }
            return {};
        }
    }
    return {};
}

std::optional<ast::MemberDeclaration::Constructor> Analyzer::get_constructor(
    const ast::TypeName& class_) {
    for (size_t i = 0; i < ast.classes.size(); i++) {
        if (ast.classes[i].name.name.name == class_.name.name) {
            for (size_t j = 0; j < ast.classes[i].body.size(); j++) {
                ast::MemberDeclaration member = ast.classes[i].body[j];
                if (auto constructor =
                        std::get_if<ast::MemberDeclaration::Constructor>(
                            &member.value)) {
                    return *constructor;
                }
            }
        }
    }
    return {};
}

bool Analyzer::check_arguments(ast::Arguments arguments,
    std::vector<ast::Expression> expressions, Variables& variables) {
    if (arguments.size() != expressions.size())
        return false;
    for (size_t i = 0; i < expressions.size(); i++) {
        ast::TypeName type = check_expression(expressions[i], variables);
        // TODO: generics
        if (arguments[i].first.name.name != type.name.name) {
            return false;
        }
    }
    return true;
}

ast::TypeName Analyzer::check_expression(
    const ast::Expression& expression, Variables& variables) {
    if (auto identifier = std::get_if<ast::Identifier>(&expression.value)) {
        if (variables.contains(identifier->name))
            return variables[identifier->name].type_name;
        std::print("ERROR use of undeclared identifier {}", identifier->name);
        return void_;
    }
    if (auto access =
            std::get_if<ast::Expression::MemberAccess>(&expression.value)) {
        ast::TypeName object_type =
            check_expression(*access->object, variables);
        std::optional<ast::MemberDeclaration> member =
            get_memeber(object_type, access->member);
        if (member.has_value()) {
            if (auto variable =
                    std::get_if<ast::Variable>(&member.value().value)) {
                return variable->type_name;
            } else {
                return void_;
            }
        } else
            return void_;
    }
    if (auto method_call =
            std::get_if<ast::Expression::MethodCall>(&expression.value)) {
        ast::TypeName object_type =
            check_expression(*method_call->object, variables);
        std::optional<ast::MemberDeclaration> member =
            get_memeber(object_type, method_call->method);
        if (member.has_value()) {
            if (auto method = std::get_if<ast::MemberDeclaration::Method>(
                    &member.value().value)) {
                if (check_arguments(
                        method->arguments, method_call->arguments, variables))
                    return method->return_type;
                else
                    return void_;
            } else {
                return void_;
            }
        } else
            return void_;
    }
    if (auto literal =
            std::get_if<ast::Expression::Literal>(&expression.value)) {
        switch (literal->type) {

        case ast::Expression::Literal::Int:
            return ast::TypeName { { "Integer" }, {} };
        case ast::Expression::Literal::Str:
            return ast::TypeName { { "String" }, {} };
        case ast::Expression::Literal::Bool:
            return ast::TypeName { { "Bool" }, {} };
        case ast::Expression::Literal::Real:
            return ast::TypeName { { "Real" }, {} };
        }
    }
    if (auto constructor_call =
            std::get_if<ast::Expression::ConstructorCall>(&expression.value)) {
        auto constructor = get_constructor(constructor_call->type_name);
        if (constructor.has_value()) {
            if (check_arguments(constructor->arguments,
                    constructor_call->arguments, variables))
                return constructor_call->type_name;
            else
                return void_;
        } else {
            if (constructor_call->arguments.empty())
                return constructor_call->type_name;
            else
                return void_;
        }
    }
    return void_;
}

void Analyzer::check_statements(
    const std::vector<ast::Statement>& statements, Variables& variables) {
    std::vector<std::string> scope_variables {};
    for (size_t k = 0; k < statements.size(); k++) {
        ast::Statement statement = statements[k];
        if (auto if_statement =
                std::get_if<ast::Statement::If>(&statement.value)) {
            if (check_expression(if_statement->condition, variables)
                    .name.name != "Bool") {
                print(statement, 0);
                std::print("ERROR condition expected to be of type Bool\n");
            }
            check_statements(if_statement->body, variables);
            for (size_t h = 0; h < if_statement->elifs.size(); h++) {
                if (check_expression(
                        if_statement->elifs[h].condition, variables)
                        .name.name != "Bool") {
                    print(statement, 0);
                    std::print("ERROR condition expected to be of type Bool\n");
                }
                check_statements(if_statement->elifs[h].body, variables);
            }
            check_statements(if_statement->else_body, variables);
        }
        if (auto while_statement =
                std::get_if<ast::Statement::While>(&statement.value)) {
            if (check_expression(while_statement->condition, variables)
                    .name.name == "Bool") {
                print(statement, 0);
                std::print("ERROR conditoin expected to be of type Bool\n");
            }
            check_statements(while_statement->body, variables);
        }
        if (auto variable = std::get_if<ast::Variable>(&statement.value)) {
            if (variable->value.has_value()) {
                Variables variables {};
                std::string expression_type =
                    check_expression(variable->value.value(), variables)
                        .name.name;
                if (expression_type != variable->type_name.name.name) {
                    print(&(*variable), 0);
                    std::print("ERROR expected type {}, got {}\n",
                        variable->type_name.name.name, expression_type);
                }
            }
            variables[variable->name.name] = *variable;
            scope_variables.push_back(variable->name.name);
        }
    }
}

void Analyzer::analyze() {
    for (size_t i = 0; i < ast.classes.size(); i++) {
        const ast::Class& class_ = ast.classes[i];
        // check redefinitions
        for (size_t j = 0; j < i; j++) {
            if (class_.name.name.name == ast.classes[j].name.name.name &&
                i != j) {

                std::print("ERROR multiple definitions of class {}\n",
                    class_.name.name.name);
                break;
            }
        }
        // check inheritance
        for (size_t j = 0; j < class_.extends.size(); j++) {
            const std::string& extends = class_.extends[j].name;
            bool exists = false;
            for (size_t k = 0; k < ast.classes.size(); k++) {
                if (ast.classes[k].name.name.name == extends) {
                    exists = true;
                    break;
                }
            }
            if (!exists)
                std::print("ERROR '{0} extends {1}' class {1} doesn't exist\n",
                    class_.name.name.name, extends);
        }
        // check declarations
        for (size_t j = 0; j < class_.body.size(); j++) {
            const ast::MemberDeclaration& member = class_.body[j];
            if (auto variable = std::get_if<ast::Variable>(&member.value)) {
                if (variable->value.has_value()) {
                    Variables variables {};
                    std::string expression_type =
                        check_expression(variable->value.value(), variables)
                            .name.name;
                    if (expression_type != variable->type_name.name.name) {
                        print(&(*variable), 0);
                        std::print("ERROR expected type {}, got {}\n",
                            variable->type_name.name.name, expression_type);
                    }
                }
            }
            if (auto method = std::get_if<ast::MemberDeclaration::Method>(
                    &member.value)) {
                Variables variables {};
                for (size_t k = 0; k < method->arguments.size(); k++) {
                    variables[method->arguments[i].second.name] =
                        ast::Variable { method->arguments[i].first,
                            method->arguments[i].second, {} };
                }
            }
        }
    }
}
