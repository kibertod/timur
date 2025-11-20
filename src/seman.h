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

    TypeName substitute_generics(
        const TypeName& declaration, const TypeName& instance, const TypeName& type);
    std::vector<TypeName> substitute_generics(
        const TypeName& declaration, const TypeName& instance, const std::vector<TypeName>& types);
    std::optional<Class> type_exists(const TypeName& name);
    std::optional<TypeName> get_property(const TypeName& type, const Identifier& property);
    std::optional<TypeName> get_method(
        const TypeName& type, const Identifier& method, const std::vector<TypeName>& arguments);

    std::optional<TypeName> check_expression(const Expression& expression);
    TypeName check_literal(const Expression::Literal& literal);
    std::optional<TypeName> check_member_access(const Expression::MemberAccess& access);
    std::optional<TypeName> check_this_access(const Expression::ThisAccess& access);
    std::optional<TypeName> check_method_call(const Expression::MethodCall& call);
    std::optional<TypeName> check_this_call(const Expression::ThisCall& call);
    std::optional<TypeName> check_constructor_call(const Expression::ConstructorCall& call);

    void check_statement(const Statement& statement);
    void check_if(const Statement::If& if_);
    void check_while(const Statement::While& while_);
    void check_assignment(const Statement::Assignment& assignment);
    void check_super_call(const Statement::SuperCall& call);
    void check_return(const Statement::Return& return_);

    std::optional<VariableState> check_variable(const Variable& variable);

    void check_class(const Class& class_);
    void check_method(const MemberDeclaration::Method& method);
    void check_constructor(const MemberDeclaration::Constructor& constructor);

public:
    void analyze();
    Analyzer(Root ast);
};
