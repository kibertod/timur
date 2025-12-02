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
        bool operator<(const Identifier&) const;
    };

    struct TypeName {
        Identifier name;
        std::vector<TypeName> generic_arguments;
        bool operator==(const TypeName&) const = default;
        bool operator<(const TypeName&) const;
    };

    struct Expression {
        struct MemberAccess {
            Ptr<Expression> object;
            Identifier member;
            bool operator==(const MemberAccess&) const = default;
            bool operator<(const MemberAccess&) const;
        };

        struct MethodCall {
            Ptr<Expression> object;
            Identifier method;
            std::vector<Expression> arguments;
            bool operator==(const MethodCall&) const = default;
            bool operator<(const MethodCall&) const;
        };

        struct ThisCall {
            Identifier method;
            std::vector<Expression> arguments;
            bool operator==(const ThisCall&) const = default;
            bool operator<(const ThisCall&) const;
        };

        struct ThisAccess {
            Identifier member;
            bool operator==(const ThisAccess&) const = default;
            bool operator<(const ThisAccess&) const;
        };

        struct ConstructorCall {
            TypeName type_name;
            std::vector<Expression> arguments;
            bool operator==(const ConstructorCall&) const = default;
            bool operator<(const ConstructorCall&) const;
        };

        struct Literal {
            enum Type { Int, Str, Bool, Real };

            Type type;
            std::string value;
            bool operator==(const Literal&) const = default;
            bool operator<(const Literal&) const;
        };

        std::variant<Identifier, MemberAccess, MethodCall, ConstructorCall, Literal, ThisAccess,
            ThisCall>
            value;
        bool operator==(const Expression&) const = default;
        bool operator<(const Expression&) const;
    };

    struct Variable {
        TypeName type_name;
        Identifier name;
        std::optional<Expression> value;
        bool operator==(const Variable&) const = default;
        bool operator<(const Variable&) const;
    };

    struct Statement {
        struct If {
            struct ElIf {
                Expression condition;
                std::vector<Statement> body;
                bool operator==(const ElIf&) const = default;
                bool operator<(const ElIf&) const;
            };

            Expression condition;
            std::vector<Statement> body;
            std::vector<ElIf> elifs;
            std::vector<Statement> else_body;
            bool operator==(const If&) const = default;
            bool operator<(const If&) const;
        };

        struct While {
            Expression condition;
            std::vector<Statement> body;
            bool operator==(const While&) const = default;
            bool operator<(const While&) const;
        };

        struct Assignment {
            Expression left;
            Expression right;
            bool operator==(const Assignment&) const = default;
            bool operator<(const Assignment&) const;
        };

        struct SuperCall {
            std::optional<TypeName> parent;
            std::vector<Expression> arguments;
            bool operator==(const SuperCall&) const = default;
            bool operator<(const SuperCall&) const;
        };

        struct Return {
            std::optional<Expression> value;
            bool operator==(const Return&) const = default;
            bool operator<(const Return&) const;
        };

        std::variant<If, While, Assignment, Return, Variable, Expression, SuperCall> value;
        bool operator==(const Statement&) const = default;
        bool operator<(const Statement&) const;
    };

    typedef std::vector<std::pair<TypeName, Identifier>> Arguments;

    struct MemberDeclaration {
        struct Method {
            Identifier name;
            std::optional<TypeName> return_type;
            std::vector<std::pair<TypeName, Identifier>> arguments;
            std::vector<Statement> body;
            bool operator==(const Method&) const = default;
            bool operator<(const Method&) const;
        };

        struct Constructor {
            std::vector<std::pair<TypeName, Identifier>> arguments;
            std::vector<Statement> body;
            bool operator==(const Constructor&) const = default;
            bool operator<(const Constructor&) const;
        };

        std::variant<Variable, Method, Constructor> value;
        bool operator==(const MemberDeclaration&) const = default;
        bool operator<(const MemberDeclaration&) const;
    };

    struct Class {
        TypeName name;
        std::vector<TypeName> extends;
        std::vector<MemberDeclaration> body;
        bool operator==(const Class&) const = default;
        bool operator<(const Class&) const;
    };

    struct Root {
        std::vector<Class> classes;
        bool operator==(const Root&) const = default;
        bool operator<(const Root&) const;
    };

}
