#include "ast.h"
#include <tuple>

using namespace ast;

bool Identifier::operator<(const Identifier& other) const { return name < other.name; }

bool TypeName::operator<(const TypeName& other) const {
    return std::tie(name, generic_arguments) < std::tie(other.name, other.generic_arguments);
}

bool TypeName::eq_no_ptr(const TypeName& other) const {
    return name == other.name && generic_arguments == other.generic_arguments;
}

bool Variable::operator<(const Variable& other) const {
    return std::tie(name, type_name, value) < std::tie(other.name, other.type_name, other.value);
}

bool Expression::MemberAccess::operator<(const MemberAccess& other) const {

    return std::tie(*object, member) < std::tie(*other.object, other.member);
}

bool Expression::MethodCall::operator<(const MethodCall& other) const {
    return std::tie(*object, method, arguments) <
           std::tie(*other.object, other.method, other.arguments);
}

bool Expression::ThisCall::operator<(const ThisCall& other) const {
    return std::tie(method, arguments) < std::tie(other.method, other.arguments);
}

bool Expression::ThisAccess::operator<(const ThisAccess& other) const {
    return member < other.member;
}

bool Expression::ConstructorCall::operator<(const ConstructorCall& other) const {
    return std::tie(type_name, arguments) < std::tie(other.type_name, other.arguments);
}

bool Expression::Literal::operator<(const Literal& other) const {
    if (bool cmp = type < other.type; cmp != 0) return cmp;
    return value < other.value;
}

bool Expression::Deref::operator<(const Deref& other) const { return *object < *other.object; }

bool Expression::operator<(const Expression& other) const { return value < other.value; }

bool Statement::If::ElIf::operator<(const ElIf& other) const {
    return std::tie(condition, body) < std::tie(other.condition, other.body);
}

bool Statement::If::operator<(const If& other) const {
    return std::tie(condition, body, elifs, else_body) <
           std::tie(other.condition, other.body, other.elifs, other.else_body);
}

bool Statement::While::operator<(const While& other) const {
    return std::tie(condition, body) < std::tie(other.condition, other.body);
}

bool Statement::Assignment::operator<(const Assignment& other) const {
    return std::tie(left, right) < std::tie(other.left, other.right);
}

bool Statement::SuperCall::operator<(const SuperCall& other) const {
    return std::tie(parent, arguments) < std::tie(other.parent, other.arguments);
}

bool Statement::Return::operator<(const Return& other) const { return value < other.value; }

bool Statement::operator<(const Statement& other) const { return value < other.value; }

bool MemberDeclaration::Method::operator<(const Method& other) const {
    return std::tie(name, return_type, arguments, body) <
           std::tie(other.name, other.return_type, other.arguments, other.body);
}

bool MemberDeclaration::Constructor::operator<(const Constructor& other) const {
    return arguments < other.arguments;
}

bool MemberDeclaration::operator<(const MemberDeclaration& other) const {
    return value < other.value;
}

bool Class::operator<(const Class& other) const {
    return std::tie(name, extends, body) < std::tie(other.name, other.extends, other.body);
}

bool Root::operator<(const Root& other) const { return classes < other.classes; }
