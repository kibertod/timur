#pragma once

#include "ast.h"
#include <map>
#include <optional>
#include <unordered_map>

using namespace ast;

static TypeName void_ { { "Void" }, {} };

struct VariableState {
    std::string name;
    TypeName type;
    bool set;
};

class Analyzer {
private:
    std::unordered_map<std::string, VariableState> m_variables;
    std::unordered_map<std::string, VariableState> m_members;
    Root m_ast;
    std::unordered_map<std::string, Class> m_classes;
    std::unordered_map<std::string, std::map<std::string, TypeName>> m_properties;
    std::unordered_map<std::string,
        std::map<std::string, std::vector<std::pair<std::vector<TypeName>, TypeName>>>>
        m_methods;
    std::unordered_map<std::string, std::vector<std::vector<TypeName>>> m_constructors;
    std::optional<Class> m_class;
    std::optional<MemberDeclaration::Method> m_method;
    std::unordered_map<std::string, TypeName> m_generics;

    bool lvalue_accessible(const Expression&);
    std::optional<Class> type_exists(const TypeName&);
    std::optional<TypeName> get_property(const TypeName&, const Identifier&);
    std::optional<TypeName> get_method(
        const TypeName&, const Identifier&, const std::vector<TypeName>&);

    TypeName check_literal(const Expression::Literal&);
    std::optional<TypeName> check_member_access(const Expression::MemberAccess&);
    std::optional<TypeName> check_this_access(const Expression::ThisAccess&);
    std::optional<TypeName> check_method_call(const Expression::MethodCall&);
    std::optional<TypeName> check_this_call(const Expression::ThisCall&);
    std::optional<TypeName> check_constructor_call(const Expression::ConstructorCall&);

    MemberDeclaration::Method substitute_generics(MemberDeclaration::Method);
    MemberDeclaration::Constructor substitute_generics(MemberDeclaration::Constructor);
    MemberDeclaration substitute_generics(MemberDeclaration);
    Variable substitute_generics(Variable);
    Class substitute_generics(Class);
    TypeName substitute_generics(TypeName);
    Expression substitute_generics(Expression);

    Statement substitute_generics(Statement);
    Statement::If substitute_generics(Statement::If);
    Statement::While substitute_generics(Statement::While);

    void check_statement(const Statement&);
    void check_if(const Statement::If&);
    void check_while(const Statement::While&);
    void check_assignment(const Statement::Assignment&);
    void check_super_call(const Statement::SuperCall&);
    void check_return(const Statement::Return&);

    std::optional<VariableState> check_variable(const Variable&);

    void check_class_declaration(const Class&);
    void check_class(const Class&);
    void check_method(const MemberDeclaration::Method&);
    void check_constructor(const MemberDeclaration::Constructor&);

    void print_error(std::string error);

public:
    bool error = false;

    void analyze();
    std::optional<TypeName> check_expression(const Expression&);
    Analyzer(Root ast);
};
