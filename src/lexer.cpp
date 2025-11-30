#include <string>
#include <cctype>
#include "parser.tab.hpp"

namespace yy {
    std::string point_err(ParserContext& ctx, size_t row, size_t col) {
        size_t pos = 0;
        size_t row_i = 1;
        while (row_i != row) {
            if (ctx.src[pos] == '\n')
                row_i++;
            pos++;
        }
        std::string res;
        while (ctx.src[pos] != '\n') {
            res += ctx.src[pos++];
        }
        res += "\n";
        for (size_t i = 0; i < col - 1; i++) {
            res += " ";
        }
        res += "^\n";
        return res;
    }

    parser::symbol_type yylex(ParserContext& ctx) {
        std::string buffer;

        while (true) {
            if (ctx.pos >= ctx.src.length()) {
                return parser::make_YYEOF(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            }
            if (std::isspace(ctx.src[ctx.pos])) {
                if (ctx.src[ctx.pos] == '\n') {
                    ctx.row++;
                    ctx.row_pos = ctx.pos;
                }
                ctx.pos++;
                continue;
            }
            if (std::isdigit(ctx.src[ctx.pos])) {
                bool real = false;
                buffer += ctx.src[ctx.pos];
                ctx.pos++;
                while (std::isdigit(ctx.src[ctx.pos]) || ctx.src[ctx.pos] == '.') {
                    if (ctx.src[ctx.pos] == '.') {
                        if (real) {
                            return parser::make_LitReal(
                                buffer, location(position(YY_NULLPTR, ctx.row,
                                            ctx.pos - ctx.row_pos - buffer.size())));
                        }
                        real = true;
                    }
                    buffer += ctx.src[ctx.pos];
                    ctx.pos++;
                }
                if (buffer[buffer.length() - 1] == '.')
                    return parser::make_Err(std::string("\'.\' at the end of a number"),
                        location(
                            position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (real) {
                    return parser::make_LitReal(buffer,
                        location(
                            position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                } else {
                    return parser::make_LitInt(buffer, location(position(YY_NULLPTR, ctx.row,
                                                           ctx.pos - ctx.row_pos - buffer.size())));
                }
            }
            if (ctx.src[ctx.pos] == '"') {
                ctx.pos++;
                while (ctx.src[ctx.pos] != '"') {
                    buffer += ctx.src[ctx.pos];
                    ctx.pos++;
                }
                ctx.pos++;
                return parser::make_LitStr(buffer, location(position(YY_NULLPTR, ctx.row,
                                                       ctx.pos - ctx.row_pos - buffer.size() - 2)));
            }
            switch (ctx.src[ctx.pos]) {
            case '(':
                ctx.pos++;
                return parser::make_LParen(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case ')':
                ctx.pos++;
                return parser::make_RParen(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case '.':
                ctx.pos++;
                return parser::make_Access(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case ',':
                ctx.pos++;
                return parser::make_Comma(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case ':':
                ctx.pos++;
                if (ctx.src[ctx.pos] == '=') {
                    ctx.pos++;
                    return parser::make_Assign(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                }
                return parser::make_Colon(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case '[':
                ctx.pos++;
                return parser::make_LBracket(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case ']':
                ctx.pos++;
                return parser::make_RBracket(
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            case '/':
                ctx.pos++;
                if (ctx.src[ctx.pos] == '/') {
                    while (true) {
                        ctx.pos++;
                        if (std::isspace(ctx.src[ctx.pos]) && !std::isblank(ctx.src[ctx.pos]))
                            break;
                    }
                }
                continue;
            }

            if (std::isalpha(ctx.src[ctx.pos]) || ctx.src[ctx.pos] == '_') {
                buffer += ctx.src[ctx.pos];
                ctx.pos++;
                while (std::isalpha(ctx.src[ctx.pos]) || ctx.src[ctx.pos] == '_' ||
                       std::isdigit(ctx.src[ctx.pos])) {
                    buffer += ctx.src[ctx.pos];
                    ctx.pos++;
                }
                if (buffer == "var")
                    return parser::make_KVar(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "void")
                    return parser::make_KVoid(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "while")
                    return parser::make_KWhile(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "loop")
                    return parser::make_KLoop(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "if")
                    return parser::make_KIf(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "then")
                    return parser::make_KThen(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "else")
                    return parser::make_KElse(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "is")
                    return parser::make_KIs(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "end")
                    return parser::make_KEnd(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "class")
                    return parser::make_KClass(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "elif")
                    return parser::make_KElif(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "extends")
                    return parser::make_KExtends(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "this")
                    return parser::make_KThis(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "super")
                    return parser::make_KSuper(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "true")
                    return parser::make_KTrue(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "false")
                    return parser::make_KFalse(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                if (buffer == "return")
                    return parser::make_KReturn(location(
                        position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
                return parser::make_Identifier(buffer,
                    location(position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size())));
            }
            return parser::make_Err("Unexpected character",
                location(location(
                    position(YY_NULLPTR, ctx.row, ctx.pos - ctx.row_pos - buffer.size()))));
        }
    }
}
