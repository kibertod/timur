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
    };

    struct TypeName {
        Identifier name;
        std::vector<TypeName> generic_arguments;
    };

    struct Expression {
        struct MemberAccess {
            Ptr<Expression> object;
            Identifier member;
        };

        struct MethodCall {
            Ptr<Expression> object;
            Identifier method;
            std::vector<Expression> arguments;
        };

        struct ThisAccess {
            Identifier member;
        };

        struct ThisCall {
            std::vector<Expression> arguments;
        };

        struct ConstructorCall {
            TypeName type_name;
            std::vector<Expression> arguments;
        };

        struct SuperCall {
            std::vector<Expression> arguments;
        };

        struct Literal {
            enum Type { Int, Str, Bool, Real };

            Type type;
            std::string value;
        };

        std::variant<Identifier, MemberAccess, MethodCall, ConstructorCall,
            SuperCall, Literal, ThisAccess, ThisCall>
            value;
    };

    struct Variable {
        TypeName type_name;
        Identifier name;
        std::optional<Expression> value;
    };

    struct Statement {
        struct If {
            struct ElIf {
                Expression condition;
                std::vector<Statement> body;
            };

            Expression condition;
            std::vector<Statement> body;
            std::vector<ElIf> elifs;
            std::vector<Statement> else_body;
        };

        struct While {
            Expression condition;
            std::vector<Statement> body;
        };

        struct Assignment {
            Expression left;
            Expression right;
        };

        struct Return {
            Expression value;
        };

        std::variant<If, While, Assignment, Return, Variable, Expression> value;
    };

    struct MemberDeclaration {
        struct Method {
            Identifier name;
            TypeName return_type;
            std::vector<std::pair<TypeName, Identifier>> arguments;
            std::vector<Statement> body;
        };

        struct Constructor {
            std::vector<std::pair<TypeName, Identifier>> arguments;
            std::vector<Statement> body;
        };

        std::variant<Variable, Method, Constructor> value;
    };

    struct Class {
        TypeName name;
        std::vector<Identifier> extends;
        std::vector<MemberDeclaration> body;
    };

    struct Root {
        std::vector<Class> classes;
    };
};
