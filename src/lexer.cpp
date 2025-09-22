#include <string>
#include <cctype>
#include "parser.tab.hpp"

namespace yy {
    parser::symbol_type yylex(ParserContext& ctx) {
        std::string buffer;

        while (true) {
            if (ctx.pos >= ctx.src.length()) {
                return parser::make_YYEOF();
            }
            if (std::isspace(ctx.src[ctx.pos])) {
                ctx.pos++;
                continue;
            }
            if (std::isdigit(ctx.src[ctx.pos])) {
                bool real = false;
                buffer += ctx.src[ctx.pos];
                ctx.pos++;
                while (
                    std::isdigit(ctx.src[ctx.pos]) || ctx.src[ctx.pos] == '.') {
                    if (ctx.src[ctx.pos] == '.') {
                        if (real) {
                            return parser::make_LitReal(buffer);
                        }
                        real = true;
                    }
                    buffer += ctx.src[ctx.pos];
                    ctx.pos++;
                }
                if (buffer[buffer.length() - 1] == '.')
                    return parser::make_Err(
                        std::string("\'.\' at the end of a number"));
                if (real) {
                    return parser::make_LitReal(buffer);
                } else {
                    return parser::make_LitInt(buffer);
                }
            }
            if (ctx.src[ctx.pos] == '"') {
                ctx.pos++;
                while (ctx.src[ctx.pos] != '"') {
                    buffer += ctx.src[ctx.pos];
                    ctx.pos++;
                }
                ctx.pos++;
                return parser::make_LitStr(buffer);
            }
            switch (ctx.src[ctx.pos]) {
            case '(':
                ctx.pos++;
                return parser::make_LParen();
            case ')':
                ctx.pos++;
                return parser::make_RParen();
            case '.':
                ctx.pos++;
                return parser::make_Access();
            case ',':
                ctx.pos++;
                return parser::make_Comma();
            case ':':
                ctx.pos++;
                if (ctx.src[ctx.pos] == '=') {
                    ctx.pos++;
                    return parser::make_Assign();
                }
                return parser::make_Colon();
            case '[':
                ctx.pos++;
                return parser::make_LBracket();
            case ']':
                ctx.pos++;
                return parser::make_RBracket();
            case '/':
                ctx.pos++;
                if (ctx.src[ctx.pos] == '/') {
                    while (true) {
                        ctx.pos++;
                        if (std::isspace(ctx.src[ctx.pos]) &&
                            !std::isblank(ctx.src[ctx.pos]))
                            break;
                    }
                }
                continue;
            }

            if (std::isalpha(ctx.src[ctx.pos]) || ctx.src[ctx.pos] == '_') {
                buffer += ctx.src[ctx.pos];
                ctx.pos++;
                while (std::isalpha(ctx.src[ctx.pos]) ||
                       ctx.src[ctx.pos] == '_' ||
                       std::isdigit(ctx.src[ctx.pos])) {
                    buffer += ctx.src[ctx.pos];
                    ctx.pos++;
                }
                if (buffer == "var")
                    return parser::make_KVar();
                if (buffer == "while")
                    return parser::make_KWhile();
                if (buffer == "loop")
                    return parser::make_KLoop();
                if (buffer == "if")
                    return parser::make_KIf();
                if (buffer == "then")
                    return parser::make_KThen();
                if (buffer == "else")
                    return parser::make_KElse();
                if (buffer == "is")
                    return parser::make_KIs();
                if (buffer == "end")
                    return parser::make_KEnd();
                if (buffer == "class")
                    return parser::make_KClass();
                if (buffer == "method")
                    return parser::make_KMethod();
                if (buffer == "extends")
                    return parser::make_KExtends();
                if (buffer == "this")
                    return parser::make_KThis();
                if (buffer == "super")
                    return parser::make_KSuper();
                if (buffer == "true")
                    return parser::make_KTrue();
                if (buffer == "false")
                    return parser::make_KFalse();
                if (buffer == "return")
                    return parser::make_KReturn();
                return parser::make_Identifier(buffer);
            }
            return parser::make_Err("Unexpected character");
        }
    }
}
