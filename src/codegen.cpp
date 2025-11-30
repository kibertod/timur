#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include <format>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <memory>
#include <set>
#include <variant>

#include "codegen.h"
#include "ast.h"
#include "print.hpp"

using namespace ast;

std::string Codegen::var_name() {
    std::string name;
    unsigned long long counter = m_var_count;
    while (counter > 0) {
        char c = 'a' + (counter % 26);
        name = c + name;
        counter /= 26;
    }
    m_var_count += 1;
    return name;
}

void Codegen::generate_classes() {
    std::set<std::string> exists = { "Program" };
    for (const Class& class_ : m_ast.classes) {
        if (m_structs.contains(stringify(class_.name))) {
            exists.insert(stringify(class_.name));
            continue;
        }
        if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty())
            generate_class(class_);
    }
    for (const Class& class_ : m_ast.classes)
        if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty() &&
            !exists.contains(stringify(class_.name)))
            generate_class_properties(class_);
    for (const Class& class_ : m_ast.classes)
        if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty() &&
            !exists.contains(stringify(class_.name)))
            generate_class_method_definitions(class_);
    for (const Class& class_ : m_ast.classes)
        if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty() &&
            !exists.contains(stringify(class_.name)))
            generate_class_method_implementations(class_);
}

void Codegen::generate_class(Class class_) {
    llvm::StructType* struct_ = llvm::StructType::create(m_context, stringify(class_.name));
    m_structs[stringify(class_.name)] = struct_;
}

void Codegen::generate_class_properties(Class class_) {
    std::vector<llvm::Type*> types {};
    for (const MemberDeclaration& member : class_.body)
        if (auto prop = std::get_if<Variable>(&member.value)) {
            TypeName type = prop->type_name;
            types.push_back(m_structs[stringify(type)]);
            m_props[stringify(class_.name)][prop->name.name] =
                m_props[stringify(class_.name)].size();
        }
    m_structs[stringify(class_.name)]->setBody(types);
}

void Codegen::generate_class_method_definitions(Class class_) {
    for (const MemberDeclaration& member : class_.body) {
        if (auto constructor_ = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
            MemberDeclaration::Constructor constructor = *constructor_;

            std::vector<llvm::Type*> args {};
            for (const auto& arg : constructor.arguments)
                args.push_back(m_structs[stringify(arg.first)]);

            generate_function_entry(m_structs[stringify(class_.name)], args,
                std::format("this", stringify(class_.name)), stringify(class_.name), false);
        }
        if (auto method_ = std::get_if<MemberDeclaration::Method>(&member.value)) {
            MemberDeclaration::Method method = *method_;

            for (size_t i = 0; i < method.arguments.size(); i++)
                method.arguments[i].first = method.arguments[i].first;
            method.return_type = method.return_type;

            std::vector<llvm::Type*> args { llvm::PointerType::get(
                m_structs[stringify(class_.name)], 0) };
            for (const auto& arg : method.arguments)
                args.push_back(m_structs[stringify(arg.first)]);
            llvm::Type* return_type;
            if (method.return_type)
                return_type = m_structs[stringify(*method.return_type)];
            else
                return_type = m_builder.getVoidTy();

            generate_function_entry(
                return_type, args, method.name.name, stringify(class_.name), false);
        }
    }
}

void Codegen::generate_class_method_implementations(Class class_) {
    for (const MemberDeclaration& member : class_.body) {
        if (auto constructor_ = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
            MemberDeclaration::Constructor constructor = *constructor_;

            auto variables_bak = m_variables;
            m_variables = {};

            std::vector<llvm::Type*> args {};
            for (const auto& arg : constructor.arguments)
                args.push_back(m_structs[stringify(arg.first)]);

            llvm::Function* fn;
            for (auto overload : m_functions[stringify(class_.name)]["this"])
                if (overload.first == args) {
                    fn = overload.second;
                    break;
                }

            std::string signature {};
            for (llvm::Type* arg : args)
                if (arg->isStructTy())
                    signature += arg->getStructName().str();
                else
                    signature += "Ptr";

            llvm::BasicBlock* entry =
                llvm::BasicBlock::Create(m_context, std::format("this_{}_entry", signature), fn);
            m_builder.SetInsertPoint(entry);
            m_this = { m_builder.CreateAlloca(m_structs[stringify(class_.name)]),
                m_structs[stringify(class_.name)] };
            auto arg_iter = fn->arg_begin();
            for (const auto& arg : constructor.arguments) {
                m_variables[arg.second.name] = { m_builder.CreateAlloca(
                                                     m_structs[stringify(arg.first)]),
                    var_name(), m_structs[stringify(arg.first)] };
                m_builder.CreateStore(&*arg_iter++, m_variables[arg.second.name].ptr);
            }
            for (const Statement& stmt : constructor.body) {
                generate_statement(stmt);
            }
            m_builder.CreateRet(
                m_builder.CreateLoad(m_structs[stringify(class_.name)], m_this->first));

            m_this = {};
            m_variables = variables_bak;
        }
        if (auto method_ = std::get_if<MemberDeclaration::Method>(&member.value)) {
            MemberDeclaration::Method method = *method_;

            auto variables_bak = m_variables;
            m_variables = {};

            std::vector<llvm::Type*> args { llvm::PointerType::get(
                m_structs[stringify(class_.name)], 0) };
            for (const auto& arg : method.arguments)
                args.push_back(m_structs[stringify(arg.first)]);
            llvm::Function* fn;
            for (auto overload : m_functions[stringify(class_.name)][method.name.name])
                if (overload.first == args) {
                    fn = overload.second;
                    break;
                }

            std::string signature {};
            for (llvm::Type* arg : args)
                if (arg->isStructTy())
                    signature += arg->getStructName().str();
                else
                    signature += "Ptr";

            llvm::BasicBlock* entry = llvm::BasicBlock::Create(
                m_context, std::format("{}_{}_entry", method.name.name, signature), fn);
            m_builder.SetInsertPoint(entry);

            auto arg_iter = fn->arg_begin();
            m_this = { arg_iter++, m_structs[stringify(class_.name)] };
            for (const auto& arg : method.arguments) {
                m_variables[arg.second.name] = { m_builder.CreateAlloca(
                                                     m_structs[stringify(arg.first)]),
                    var_name(), m_structs[stringify(arg.first)] };
                m_builder.CreateStore(&*arg_iter++, m_variables[arg.second.name].ptr);
            }
            for (const Statement& stmt : method.body) {
                generate_statement(stmt);
            }

            m_this = {};
            m_variables = variables_bak;
        }
    }
}

llvm::Function* Codegen::generate_function_entry(llvm::Type* return_type,
    std::vector<llvm::Type*> args, std::string method_name, std::string struct_name, bool insert) {

    std::string signature {};
    for (llvm::Type* arg : args)
        if (arg->isStructTy())
            signature += arg->getStructName().str();
        else
            signature += "Ptr";

    auto* fn_type = llvm::FunctionType::get(return_type, args, false);
    auto* fn =
        llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, method_name, *m_module);
    if (insert) {
        llvm::BasicBlock* entry = llvm::BasicBlock::Create(
            m_context, std::format("{}_{}_entry", method_name, signature), fn);
        m_builder.SetInsertPoint(entry);
    }
    m_functions[struct_name][method_name].push_back({ args, fn });
    return fn;
}

llvm::Value* Codegen::generate_literal(const Expression::Literal& literal) {
    if (literal.type == Expression::Literal::Type::Int) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["Integer"]);
        return m_builder.CreateInsertValue(
            val, m_builder.getInt64(std::stoi(literal.value)), { 0 });
    }
    if (literal.type == Expression::Literal::Type::Bool) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["Bool"]);
        if (literal.value == "true")
            return m_builder.CreateInsertValue(val, m_builder.getInt1(true), { 0 });
        else
            return m_builder.CreateInsertValue(val, m_builder.getInt1(false), { 0 });
    }
    if (literal.type == Expression::Literal::Type::Str) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["String"]);
        val = m_builder.CreateInsertValue(val, m_builder.CreateGlobalString(literal.value), 0);
        val = m_builder.CreateInsertValue(val, m_builder.getInt64(literal.value.size()), 1);
        return val;
    }
    if (literal.type == Expression::Literal::Type::Real) {
        return m_builder.CreateInsertValue(llvm::UndefValue::get(m_structs["Real"]),
            llvm::ConstantFP::get(m_builder.getDoubleTy(), std::stof(literal.value)), 0);
    }
    llvm_unreachable("generate_literal called on non-literal");
};

llvm::Value* Codegen::generate_method_call(const Expression::MethodCall& call) {
    auto [object, type] = generate_lvalue(*call.object);

    std::vector<llvm::Value*> args = { object };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg));

    std::vector<llvm::Type*> arg_types = { llvm::PointerType::get(type, 0) };
    for (size_t i = 1; i < args.size(); i++)
        arg_types.push_back(args[i]->getType());

    llvm::Function* fn;
    for (auto overload : m_functions[type->getStructName().str()][call.method.name])
        if (overload.first == arg_types) {
            fn = overload.second;
            break;
        }

    llvm::Value* fn_call = m_builder.CreateCall(fn, args);
    return fn_call;
}

llvm::Value* Codegen::generate_constructor_call(const Expression::ConstructorCall& call) {
    TypeName type = call.type_name;

    std::vector<llvm::Value*> args = {};
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg));

    std::vector<llvm::Type*> arg_types = {};
    for (llvm::Value* arg : args)
        arg_types.push_back(arg->getType());

    llvm::Function* fn;
    for (auto overload : m_functions[stringify(type)]["this"])
        if (overload.first == arg_types) {
            fn = overload.second;
        }

    llvm::Value* fn_call = m_builder.CreateCall(fn, args);
    return fn_call;
}

llvm::Value* Codegen::generate_this_access(const Expression::ThisAccess& access) {
    int index = m_props[m_this->second->getStructName().str()][access.member.name];
    llvm::Value* val = m_builder.CreateLoad(m_this->second, m_this->first);
    return m_builder.CreateExtractValue(val, index);
}

llvm::Value* Codegen::generate_member_access(const Expression::MemberAccess& access) {
    llvm::Value* object = generate_expression(*access.object);
    int index = m_props[object->getType()->getStructName().str()][access.member.name];
    return m_builder.CreateExtractValue(object, index);
}

llvm::Value* Codegen::generate_expression(const Expression& expr) {
    if (auto literal = std::get_if<Expression::Literal>(&expr.value))
        return generate_literal(*literal);
    if (auto ident = std::get_if<Identifier>(&expr.value)) {
        return m_builder.CreateLoad(m_variables[ident->name].type, m_variables[ident->name].ptr,
            (m_variables[ident->name].gen_name + "_val"));
    }
    if (auto call = std::get_if<Expression::MethodCall>(&expr.value))
        return generate_method_call(*call);
    if (auto call = std::get_if<Expression::ConstructorCall>(&expr.value)) {
        return generate_constructor_call(*call);
    }
    if (auto access = std::get_if<Expression::ThisAccess>(&expr.value)) {
        return generate_this_access(*access);
    }
    if (auto access = std::get_if<Expression::MemberAccess>(&expr.value)) {
        return generate_member_access(*access);
    }
    llvm_unreachable("generate_expression called on non-expression");
}

std::pair<llvm::Value*, llvm::Type*> Codegen::generate_lvalue(const Expression& expr) {
    if (auto ident = std::get_if<Identifier>(&expr.value)) {
        return { m_variables[ident->name].ptr, m_variables[ident->name].type };
    } else if (auto access = std::get_if<Expression::ThisAccess>(&expr.value)) {
        int index = m_props[m_this->second->getStructName().str()][access->member.name];
        return { m_builder.CreateStructGEP(m_this->second, m_this->first, index),
            m_this->second->getStructElementType(index) };
    } else if (auto access = std::get_if<Expression::MemberAccess>(&expr.value)) {
        auto [ptr, type] = generate_lvalue(*access->object);
        int index = m_props[type->getStructName().str()][access->member.name];
        return { m_builder.CreateStructGEP(type, ptr, index), type->getStructElementType(index) };
    } else {
        llvm::Value* val = generate_expression(expr);
        llvm::Type* type = val->getType();
        llvm::AllocaInst* ptr = m_builder.CreateAlloca(type);
        m_builder.CreateStore(val, ptr);
        return { ptr, type };
    }

    llvm_unreachable("generate_lvalue called on non-lvalue expression");
}

void Codegen::generate_variable(const Variable& variable) {
    std::string name = var_name();
    TypeName type_name = variable.type_name;
    llvm::Type* type = m_structs[stringify(type_name)];
    llvm::AllocaInst* alloc = m_builder.CreateAlloca(type, nullptr, name);

    if (variable.value) {
        llvm::Value* val = generate_expression(*variable.value);
        m_builder.CreateStore(val, alloc);
    }
    m_variables[variable.name.name] = { alloc, name, type };
}

void Codegen::generate_assignment(const Statement::Assignment& assign) {
    m_builder.CreateStore(generate_expression(assign.right), generate_lvalue(assign.left).first);
}

void Codegen::generate_return(const Statement::Return& ret) {
    if (ret.value)
        m_builder.CreateRet(generate_expression(*ret.value));
    else
        m_builder.CreateRetVoid();
}

void Codegen::generate_if(const Statement::If& if_) {
    llvm::Function* fn = m_builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* then = llvm::BasicBlock::Create(m_context, var_name(), fn);
    llvm::BasicBlock* else_ = llvm::BasicBlock::Create(m_context, var_name(), fn);
    llvm::BasicBlock* merge = llvm::BasicBlock::Create(m_context, var_name(), fn);

    llvm::Value* cond = generate_expression(if_.condition);
    m_builder.CreateCondBr(m_builder.CreateExtractValue(cond, 0), then, else_);

    m_builder.SetInsertPoint(then);
    for (const Statement& stmt : if_.body)
        generate_statement(stmt);
    m_builder.CreateBr(merge);

    m_builder.SetInsertPoint(else_);
    if (if_.elifs.empty())
        for (const Statement& stmt : if_.else_body)
            generate_statement(stmt);
    else {
        Expression condition = if_.elifs[0].condition;
        std::vector<Statement> body = if_.elifs[0].body;
        std::vector<Statement::If::ElIf> elifs = if_.elifs;
        elifs.erase(elifs.begin());
        Statement::If reduced = { condition, body, elifs, if_.else_body };
        generate_if(reduced);
    };
    m_builder.CreateBr(merge);

    m_builder.SetInsertPoint(merge);
}

void Codegen::generate_while(const Statement::While& while_) {
    llvm::Function* fn = m_builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* do_ = llvm::BasicBlock::Create(m_context, var_name(), fn);
    llvm::BasicBlock* merge = llvm::BasicBlock::Create(m_context, var_name(), fn);

    llvm::Value* cond = generate_expression(while_.condition);
    m_builder.CreateCondBr(m_builder.CreateExtractValue(cond, 0), do_, merge);

    m_builder.SetInsertPoint(do_);
    for (const Statement& stmt : while_.body)
        generate_statement(stmt);
    cond = generate_expression(while_.condition);
    m_builder.CreateCondBr(m_builder.CreateExtractValue(cond, 0), do_, merge);

    m_builder.SetInsertPoint(merge);
}

void Codegen::generate_statement(const Statement& stmt) {
    if (auto var = std::get_if<Variable>(&stmt.value))
        generate_variable(*var);
    if (auto expr = std::get_if<Expression>(&stmt.value))
        generate_expression(*expr);
    if (auto assign = std::get_if<Statement::Assignment>(&stmt.value))
        generate_assignment(*assign);
    if (auto ret = std::get_if<Statement::Return>(&stmt.value))
        generate_return(*ret);
    if (auto if_ = std::get_if<Statement::If>(&stmt.value))
        generate_if(*if_);
    if (auto while_ = std::get_if<Statement::While>(&stmt.value))
        generate_while(*while_);
}

void Codegen::generate() {
    generate_integer();
    generate_real();
    generate_bool();
    generate_stdio();
    generate_string();

    generate_stdio_methods();
    generate_integer_methods();
    generate_real_methods();
    generate_string_methods();
    generate_bool_methods();

    generate_classes();

    for (const Class& class_ : m_ast.classes) {
        if (stringify(class_.name) == "Program") {
            for (const MemberDeclaration& member : class_.body) {
                if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
                    llvm::FunctionType* func_type =
                        llvm::FunctionType::get(m_builder.getVoidTy(), false);

                    llvm::Function* program_fn = llvm::Function::Create(
                        func_type, llvm::Function::ExternalLinkage, "main", m_module.get());

                    llvm::BasicBlock* entry =
                        llvm::BasicBlock::Create(m_context, "entry", program_fn);
                    m_builder.SetInsertPoint(entry);
                    for (const Statement& stmt : constructor->body) {
                        generate_statement(stmt);
                    }
                    m_builder.CreateRetVoid();
                }
            }
        }
    }

    m_module->print(llvm::outs(), nullptr);
    if (llvm::verifyModule(*m_module, &llvm::errs())) {
        llvm::errs() << "Error constructing LLVM module!\n";
        return;
    }

    std::error_code EC;
    llvm::raw_fd_ostream dest("program.bc", EC);
    llvm::WriteBitcodeToFile(*m_module, dest);
    dest.flush();
}

Codegen::Codegen(Root root) :
    m_builder(m_context),
    m_analyzer(root) {
    m_ast = root;
    m_var_count = 1;
    m_module = std::make_unique<llvm::Module>("main_module", m_context);
};
