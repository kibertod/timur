#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

    template<typename T>
    using Ptr = std::shared_ptr<T>;

    struct Identifier {
        std::string name;
        bool operator==(const Identifier&) const = default;
    };

    struct TypeName {
        Identifier name;
        std::vector<TypeName> generic_arguments;
        bool operator==(const TypeName&) const = default;
    };

    struct Expression {
        struct MemberAccess {
            Ptr<Expression> object;
            Identifier member;
            bool operator==(const MemberAccess&) const = default;
        };

        struct MethodCall {
            Ptr<Expression> object;
            Identifier method;
            std::vector<Expression> arguments;
            bool operator==(const MethodCall&) const = default;
        };

        struct ThisCall {
            Identifier method;
            std::vector<Expression> arguments;
            bool operator==(const ThisCall&) const = default;
        };

        struct ThisAccess {
            Identifier member;
            bool operator==(const ThisAccess&) const = default;
        };

        struct ConstructorCall {
            TypeName type_name;
            std::vector<Expression> arguments;
            bool operator==(const ConstructorCall&) const = default;
        };

        struct Literal {
            enum Type { Int, Str, Bool, Real };

            Type type;
            std::string value;
            bool operator==(const Literal&) const = default;
        };

        std::variant<Identifier, MemberAccess, MethodCall, ConstructorCall, Literal, ThisAccess,
            ThisCall>
            value;
        bool operator==(const Expression&) const = default;
    };

    struct Variable {
        TypeName type_name;
        Identifier name;
        std::optional<Expression> value;
        bool operator==(const Variable&) const = default;
    };

    struct Statement {
        struct If {
            struct ElIf {
                Expression condition;
                std::vector<Statement> body;
                bool operator==(const ElIf&) const = default;
            };

            Expression condition;
            std::vector<Statement> body;
            std::vector<ElIf> elifs;
            std::vector<Statement> else_body;
            bool operator==(const If&) const = default;
        };

        struct While {
            Expression condition;
            std::vector<Statement> body;
            bool operator==(const While&) const = default;
        };

        struct Assignment {
            Expression left;
            Expression right;
            bool operator==(const Assignment&) const = default;
        };

        struct SuperCall {
            std::vector<Expression> arguments;
            bool operator==(const SuperCall&) const = default;
        };

        struct Return {
            std::optional<Expression> value;
            bool operator==(const Return&) const = default;
        };

        std::variant<If, While, Assignment, Return, Variable, Expression, SuperCall> value;
        bool operator==(const Statement&) const = default;
    };

    typedef std::vector<std::pair<TypeName, Identifier>> Arguments;

    struct MemberDeclaration {
        struct Method {
            Identifier name;
            std::optional<TypeName> return_type;
            std::vector<std::pair<TypeName, Identifier>> arguments;
            std::vector<Statement> body;
            bool operator==(const Method&) const = default;
        };

        struct Constructor {
            std::vector<std::pair<TypeName, Identifier>> arguments;
            std::vector<Statement> body;
            bool operator==(const Constructor&) const = default;
        };

        std::variant<Variable, Method, Constructor> value;
        bool operator==(const MemberDeclaration&) const = default;
    };

    struct Class {
        TypeName name;
        std::optional<TypeName> extends;
        std::vector<MemberDeclaration> body;
        bool operator==(const Class&) const = default;
    };

    struct Root {
        std::vector<Class> classes;
        bool operator==(const Root&) const = default;
    };

}
