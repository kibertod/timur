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
        std::vector<Identifier> generic_arguments;
    };

    struct Expression {
        struct MemberAccess {
            Ptr<Expression> object;
            Identifier member;
        };

        struct MethodCall {
            Ptr<Expression> object;
            Identifier method;
            std::vector<Expression> argumens;
        };

        struct Literal {
            enum Type { Int, Str, Bool, Real };

            std::string value;
        };

        std::variant<Identifier, MemberAccess, MethodCall, Literal> value;
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

        struct Assignment {
            Expression left;
            Expression right;
        };

        struct While {
            Expression condition;
            std::vector<Statement> body;
        };

        struct Return {
            Expression value;
        };

        std::variant<If, While, Return, Variable, Expression> value;
    };

    struct Method {
        Identifier name;
        std::vector<std::pair<TypeName, Identifier>> arguments;
        std::vector<Statement> body;
    };

    struct Class {
        Identifier name;
        std::vector<Identifier> generic_arguments;
        std::optional<Identifier> extends;
        std::vector<std::variant<Variable, Method>> body;
    };

    struct Root {
        std::vector<Class> classes;
    };
};
