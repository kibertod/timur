#include <print>
#include <set>
#include <string>
#include "seman.h"
#include "print.hpp"

using namespace ast;

TypeName Analyzer::substitute_generics(
    const TypeName& declaration, const TypeName& instance, const TypeName& type) {
    TypeName res = type;
    if (declaration.generic_arguments.size() != instance.generic_arguments.size())
        return res;

    std::map<std::string, TypeName> substitutions;
    for (size_t i = 0; i < declaration.generic_arguments.size(); i++)
        substitutions[declaration.generic_arguments[i].name.name] = instance.generic_arguments[i];

    if (substitutions.contains(res.name.name))
        res = substitutions[res.name.name];

    for (size_t i = 0; i < res.generic_arguments.size(); i++)
        res.generic_arguments[i] =
            substitute_generics(declaration, instance, res.generic_arguments[i]);

    return res;
}

std::vector<TypeName> Analyzer::substitute_generics(
    const TypeName& declaration, const TypeName& instance, const std::vector<TypeName>& types) {
    std::vector res = types;
    for (size_t i = 0; i < types.size(); i++) {
        res[i] = substitute_generics(declaration, instance, types[i]);
    }
    return res;
}

std::optional<Class> Analyzer::type_exists(const TypeName& name) {
    if (m_class.has_value())
        for (const TypeName& generic : m_class.value().name.generic_arguments)
            if (generic == name)
                return Class { generic, {}, {} };
    if (m_classes.contains(name.name.name)) {
        Class class_ = m_classes[name.name.name];
        if (class_.name.generic_arguments.size() == name.generic_arguments.size())
            return class_;
    }
    return {};
}

std::optional<TypeName> Analyzer::get_property(const TypeName& type, const Identifier& identifier) {
    if (!m_classes.contains(type.name.name))
        return {};

    Class class_ = m_classes[type.name.name];
    if (!m_properties[type.name.name].contains(identifier.name)) {
        if (m_classes[type.name.name].extends.has_value()) {
            TypeName parent = m_classes[type.name.name].extends.value();
            TypeName parent_decl = m_classes[parent.name.name].name;
            TypeName extent_parent = substitute_generics(class_.name, type, parent);
            if (m_properties[parent.name.name].contains(identifier.name)) {
                TypeName property_type = m_properties[parent.name.name][identifier.name];
                return substitute_generics(parent_decl, extent_parent, property_type);
            }
        }
        print_error(std::format(
            "ERROR class {} doesn't have property {}\n", type.name.name, identifier.name));
        return {};
    }

    TypeName property_type = m_properties[type.name.name][identifier.name];
    return substitute_generics(class_.name, type, property_type);
}

std::optional<TypeName> Analyzer::get_method(
    const TypeName& type, const Identifier& identifier, const std::vector<TypeName>& arguments) {
    if (!m_classes.contains(type.name.name))
        return {};
    if (!m_methods[type.name.name].contains(identifier.name)) {
        bool from_extended = false;
        if (m_classes[type.name.name].extends.has_value())
            from_extended = m_methods[m_classes[type.name.name].extends.value().name.name].contains(
                identifier.name);
        if (!from_extended) {
            print_error(std::format(
                "ERROR class {} doesn't have method {}\n", stringify(type), identifier.name));
            return {};
        }
    }

    Class class_ = m_classes[type.name.name];
    auto overloads = m_methods[type.name.name][identifier.name];
    for (auto overload : overloads)
        if (substitute_generics(class_.name, type, overload.first) == arguments)
            return substitute_generics(class_.name, type, overload.second);
    if (m_classes[type.name.name].extends.has_value()) {
        auto overloads =
            m_methods[m_classes[type.name.name].extends.value().name.name][identifier.name];
        TypeName parent = m_classes[type.name.name].extends.value();
        TypeName parent_decl = m_classes[parent.name.name].name;
        TypeName extent_parent = substitute_generics(class_.name, type, parent);
        for (auto overload : overloads)
            if (substitute_generics(parent_decl, extent_parent, overload.first) == arguments)
                return substitute_generics(parent_decl, extent_parent, overload.second);
    }

    print_error(std::format("ERROR method {}.{} doesn't have an overload with arguments ({})\n",
        stringify(type), identifier.name, stringify(arguments)));
    return {};
}

TypeName Analyzer::check_literal(const Expression::Literal& literal) {
    switch (literal.type) {
    case Expression::Literal::Int:
        return TypeName { { "Integer" }, {} };
    case Expression::Literal::Str:
        return TypeName { { "String" }, {} };
    case Expression::Literal::Bool:
        return TypeName { { "Bool" }, {} };
    case Expression::Literal::Real:
        return TypeName { { "Real" }, {} };
    }
    return {};
}

std::optional<TypeName> Analyzer::check_member_access(const Expression::MemberAccess& access) {
    auto object = check_expression(*access.object);
    if (!object.has_value())
        return {};
    return get_property(object.value(), access.member);
}

std::optional<TypeName> Analyzer::check_this_access(const Expression::ThisAccess& access) {
    if (!m_class.has_value()) {
        print_error(std::format("ERROR 'this' can be accessed only inside methods\n"));
        return {};
    }
    return get_property(m_class.value().name, access.member);
}

std::optional<TypeName> Analyzer::check_method_call(const Expression::MethodCall& call) {
    auto object = check_expression(*call.object);
    if (!object.has_value())
        return {};
    std::vector<TypeName> arguments {};
    for (Expression expr : call.arguments) {
        std::optional<TypeName> type = check_expression(expr);
        if (type.has_value())
            arguments.push_back(type.value());
        else
            return {};
    }
    return get_method(object.value(), call.method, arguments);
}

std::optional<TypeName> Analyzer::check_this_call(const Expression::ThisCall& call) {
    if (!m_class.has_value()) {
        print_error(std::format("ERROR 'this' can be accessed only inside methods\n"));
        return {};
    }
    std::vector<TypeName> arguments {};
    for (Expression expr : call.arguments) {
        std::optional<TypeName> type = check_expression(expr);
        if (type.has_value())
            arguments.push_back(type.value());
        else
            return {};
    }
    return get_method(m_class.value().name, call.method, arguments);
}

std::optional<TypeName> Analyzer::check_constructor_call(const Expression::ConstructorCall& call) {
    std::vector<TypeName> arguments {};
    for (Expression expr : call.arguments) {
        std::optional<TypeName> type = check_expression(expr);
        if (type.has_value())
            arguments.push_back(type.value());
        else
            return {};
    }
    if (!m_classes.contains(call.type_name.name.name)) {
        print_error(std::format("ERROR class {} doesn't exist\n", stringify(call.type_name)));
        print(Expression { call }, 0);
        return {};
    }
    for (std::vector<TypeName> overload : m_constructors[call.type_name.name.name])
        if (substitute_generics(
                m_classes[call.type_name.name.name].name, call.type_name, overload) == arguments)
            return call.type_name;
    print_error(std::format("ERROR class {} doesn't have a custructor with arguments ({}))\n",
        stringify(call.type_name), stringify(arguments)));
    return {};
}

std::optional<TypeName> Analyzer::check_expression(const Expression& expression) {
    if (auto literal = std::get_if<Expression::Literal>(&expression.value))
        return check_literal(*literal);
    if (auto member_access = std::get_if<Expression::MemberAccess>(&expression.value))
        return check_member_access(*member_access);
    if (auto this_access = std::get_if<Expression::ThisAccess>(&expression.value))
        return check_this_access(*this_access);
    if (auto method_call = std::get_if<Expression::MethodCall>(&expression.value))
        return check_method_call(*method_call);
    if (auto this_call = std::get_if<Expression::ThisCall>(&expression.value))
        return check_this_call(*this_call);
    if (auto constructor_call = std::get_if<Expression::ConstructorCall>(&expression.value))
        return check_constructor_call(*constructor_call);
    if (auto identifier = std::get_if<Identifier>(&expression.value)) {
        if (!m_variables.contains(identifier->name))
            print_error(
                std::format("ERROR variable {} doesn't exist in this scope\n", identifier->name));
        else if (!m_variables[identifier->name].set)
            print_error(
                std::format("ERROR variable {} doesn't have a value assigned\n", identifier->name));
        else
            return m_variables[identifier->name].type;
    }
    return {};
}

std::optional<VariableState> Analyzer::check_variable(const Variable& variable) {
    if (!type_exists(variable.type_name)) {
        print_error(std::format("ERROR class {} doesn't exist\n", stringify(variable.type_name)));
        return {};
    }

    VariableState state { variable.name.name, variable.type_name, false };
    if (variable.value.has_value()) {
        auto value = check_expression(variable.value.value());
        if (!value.has_value()) {
            print(&variable, 0);
            return state;
        } else if (value.value() != variable.type_name) {
            print_error(std::format("ERROR expression doesn't match with variable type\n"));
            print(&variable, 0);
            return state;
        }
        state.set = true;
    }
    return state;
}

void Analyzer::check_if(const Statement::If& if_) {
    bool errors = false;
    if (check_expression(if_.condition) != TypeName { { "Bool" }, {} }) {
        print_error(std::format("ERROR if statement condition is not bool\n"));
        errors = true;
    }

    std::set<std::string> variables;
    for (auto variable : m_variables)
        variables.insert(variable.first);
    for (Statement statement : if_.body)
        check_statement(statement);
    for (auto variable : m_variables)
        if (!variables.contains(variable.first))
            m_variables.erase(variable.first);

    for (Statement::If::ElIf elif : if_.elifs) {
        if (check_expression(elif.condition) != TypeName { { "Bool" }, {} }) {
            print_error(std::format("ERROR elif statement condition is not bool\n"));
            errors = true;
        }
        for (Statement statement : elif.body)
            check_statement(statement);
        for (auto variable : m_variables)
            if (!variables.contains(variable.first))
                m_variables.erase(variable.first);
    }

    for (Statement statement : if_.else_body)
        check_statement(statement);
    for (auto variable : m_variables)
        if (!variables.contains(variable.first))
            m_variables.erase(variable.first);

    if (errors)
        print(Statement { if_ }, 0);
}

void Analyzer::check_while(const Statement::While& while_) {
    if (check_expression(while_.condition) != TypeName { { "Bool" }, {} }) {
        print_error(std::format("ERROR while statement condition is not bool\n"));
        print(Statement { { while_ } }, 0);
    }

    std::set<std::string> variables;
    for (auto variable : m_variables)
        variables.insert(variable.first);
    for (Statement statement : while_.body)
        check_statement(statement);
    for (auto variable : m_variables)
        if (!variables.contains(variable.first))
            m_variables.erase(variable.first);
}

void Analyzer::check_assignment(const Statement::Assignment& assignment) {
    std::optional<TypeName> type;
    if (auto _ = std::get_if<Expression::MemberAccess>(&assignment.left.value))
        type = check_expression(assignment.left);
    else if (auto _ = std::get_if<Expression::ThisAccess>(&assignment.left.value))
        type = check_expression(assignment.left);
    else if (auto identifier = std::get_if<Identifier>(&assignment.left.value)) {
        if (!m_variables.contains(identifier->name)) {
            print_error(
                std::format("ERROR variable {} doesn't exist in this scope\n", identifier->name));
            return;
        } else {
            type = m_variables[identifier->name].type;
        }
    } else {
        print_error(std::format(
            "ERROR assignment operation is only allowed for variables and properties\n"));
        print(Statement { assignment }, 0);
        return;
    }
    if (!type.has_value()) {
        return;
    }
    if (type != check_expression(assignment.right)) {
        print_error(std::format("ERROR lhs type doesn't match with rhs\n"));
        print(Statement { assignment }, 0);
        return;
    }
    if (auto variable = std::get_if<Identifier>(&assignment.left.value)) {
        m_variables[variable->name].set = true;
    }
}

void Analyzer::check_super_call(const Statement::SuperCall& super_call) {
    if (!m_class.has_value()) {
        print_error(std::format("ERROR super can be called only inside methods\n"));
        return;
    }
    if (!m_class.value().extends.has_value()) {
        print_error(
            std::format("ERROR super can be called only for classes that extends smth.\nbruh..\n"));
        return;
    }

    std::vector<TypeName> arguments {};
    for (auto argument : super_call.arguments) {
        std::optional<TypeName> type = check_expression(argument);
        if (!type.has_value()) {
            return;
        }
        arguments.push_back(type.value());
    }

    bool exists = false;
    for (std::vector<TypeName> constructor :
        m_constructors[m_class.value().extends.value().name.name])
        if (constructor == arguments) {
            exists = true;
            break;
        }
    if (!exists) {
        print_error(std::format("ERROR class {} doesn't have matching constructor\n",
            stringify(m_class.value().extends.value())));
        print(Statement { super_call }, 0);
    }
}

void Analyzer::check_return(const Statement::Return& return_) {
    if (!m_method.has_value()) {
        print_error(std::format("ERROR return statement outside of method\n"));
        print(Statement { return_ }, 0);
        return;
    }

    std::optional<TypeName> type = check_expression(return_.value);
    if (!type.has_value())
        return;

    if (type.value() != m_method.value().return_type) {
        print_error(std::format("ERROR return value doesn't match return type\n"));
        print(Statement { return_ }, 0);
    }
}

void Analyzer::check_statement(const Statement& statement) {
    if (auto if_ = std::get_if<Statement::If>(&statement.value))
        check_if(*if_);
    if (auto while_ = std::get_if<Statement::While>(&statement.value))
        check_while(*while_);
    if (auto assignment = std::get_if<Statement::Assignment>(&statement.value))
        check_assignment(*assignment);
    if (auto super_call = std::get_if<Statement::SuperCall>(&statement.value))
        check_super_call(*super_call);
    if (auto return_ = std::get_if<Statement::Return>(&statement.value))
        check_return(*return_);
    if (auto expression = std::get_if<Expression>(&statement.value))
        check_expression(*expression);
    if (auto variable = std::get_if<Variable>(&statement.value)) {
        auto state = check_variable(*variable);
        if (!state.has_value()) {
            print(statement, 0);
            return;
        }
        if (m_variables.contains(state.value().name)) {
            print_error(std::format(
                "ERROR variable {} already exists in this scope\n", state.value().name));
            print(statement, 0);
            return;
        }
        m_variables[state.value().name] = state.value();
    }
}

void Analyzer::check_method(const MemberDeclaration::Method& method) {
    m_variables = {};
    m_method = method;
    for (std::pair<TypeName, Identifier> argument : method.arguments) {
        if (m_variables.contains(argument.second.name)) {
            print_error(std::format("ERROR overlaping identifiers in method arguments\n"));
            print(MemberDeclaration { method }, 0);
        } else
            m_variables[argument.second.name] = { argument.second.name, argument.first, true };
    }
    for (Statement statement : method.body)
        check_statement(statement);
    m_method = {};
    m_variables = {};
}

void Analyzer::check_constructor(const MemberDeclaration::Constructor& constructor) {
    m_variables = {};
    for (std::pair<TypeName, Identifier> argument : constructor.arguments) {
        if (m_variables.contains(argument.second.name)) {
            print_error(std::format("ERROR overlaping identifiers in constructor arguments\n"));
            print(MemberDeclaration { constructor }, 0);
        } else
            m_variables[argument.second.name] = { argument.second.name, argument.first, true };
    }
    for (Statement statement : constructor.body)
        check_statement(statement);
    m_variables = {};
}

void Analyzer::check_class(const Class& class_) {
    if (class_.extends.has_value()) {
        if (!m_classes.contains(class_.extends.value().name.name))
            print_error(std::format("ERROR 'class {0} extends {1}' class {1} doesn't exist\n",
                class_.name.name.name, class_.extends.value().name.name));
        if (class_.extends.value().name == class_.name.name)
            print_error(
                std::format("ERROR class {0} can't extend itself\n", class_.name.name.name));
    }

    for (TypeName generic : class_.name.generic_arguments) {
        if (m_classes.contains(generic.name.name))
            print_error(std::format("ERROR generic identifier {} overlaps with class {}\n",
                generic.name.name, m_classes[generic.name.name].name.name.name));
    }

    m_class = class_;
    for (MemberDeclaration member : class_.body)
        if (auto variable = std::get_if<Variable>(&member.value))
            check_variable(*variable);

    for (MemberDeclaration member : class_.body) {
        if (auto method = std::get_if<MemberDeclaration::Method>(&member.value))
            check_method(*method);
        if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value))
            check_constructor(*constructor);
    }
    m_class = {};

    if (m_constructors[class_.name.name.name].size() == 0)
        print_error(std::format(
            "ERROR class {} doesn't define a single constructor", stringify(class_.name)));
}

void Analyzer::analyze() {
    for (Class class_ : m_ast.classes) {
        std::string classname = class_.name.name.name;
        if (m_classes.contains(classname)) {
            print_error(std::format("ERROR redeclaration of class {}\n", classname));
        } else {
            m_classes[classname] = class_;
            m_properties[classname] = {};
            m_methods[classname] = {};
            m_constructors[classname] = {};

            for (MemberDeclaration member : class_.body) {
                if (auto property = std::get_if<Variable>(&member.value)) {
                    if (m_properties[classname].contains(property->name.name)) {
                        print_error(std::format("ERROR duplicate properties {} in class {}\n",
                            property->name.name, classname));
                    } else {
                        m_properties[classname][property->name.name] = property->type_name;
                    }
                }
                if (auto method = std::get_if<MemberDeclaration::Method>(&member.value)) {
                    std::vector<TypeName> arguments {};
                    for (auto pair : method->arguments)
                        arguments.push_back(pair.first);
                    if (m_methods[classname].contains(method->name.name)) {
                        bool overload_exists = false;
                        for (auto overload : m_methods[classname][method->name.name]) {
                            if (overload.first == arguments) {
                                overload_exists = true;
                                print_error(
                                    std::format("ERROR duplicate methods {}({})) in class {}\n",
                                        method->name.name, stringify(arguments), classname));
                                break;
                            }
                        }
                        if (!overload_exists) {
                            m_methods[classname][method->name.name].push_back(
                                { arguments, method->return_type });
                        }
                    } else {
                        m_methods[classname][method->name.name] = { { arguments,
                            method->return_type } };
                    }
                }
                if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
                    std::vector<TypeName> arguments {};
                    for (auto pair : constructor->arguments)
                        arguments.push_back(pair.first);
                    if (m_constructors.contains(classname)) {
                        bool overload_exists = false;
                        for (std::vector<TypeName> overload : m_constructors[classname]) {
                            if (overload == arguments) {
                                overload_exists = true;
                                print_error(std::format(
                                    "ERROR duplicate constructor overload ({}) in class {}\n",
                                    stringify(arguments), classname));
                                break;
                            }
                        }
                        if (!overload_exists)
                            m_constructors[classname].push_back(arguments);
                    } else {
                        m_constructors[classname] = { arguments };
                    }
                }
            }
        }
    }

    for (Class class_ : m_ast.classes) {
        check_class(class_);
    }

    if (!m_classes.contains("Program")) {
        print_error("ERROR class Program is not defined\n");
    }
}

void Analyzer::print_error(std::string error) {
    std::print("{}", error);
    this->error = true;
}

Analyzer::Analyzer(Root ast) { m_ast = ast; }
