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
    for (const Class& class_ : m_ast.classes)
        if (class_.name.name.name != "Program")
            generate_class(class_);
    for (const Class& class_ : m_ast.classes)
        if (class_.name.name.name != "Program")
            generate_class_properties(class_);
    for (const Class& class_ : m_ast.classes)
        if (class_.name.name.name != "Program")
            generate_class_methods(class_);
}

void Codegen::generate_class(const Class& class_) {
    llvm::StructType* struct_ = llvm::StructType::create(m_context, class_.name.name.name);
    m_structs[class_.name.name.name] = struct_;
    m_props[class_.name.name.name] = {};
}

void Codegen::generate_class_properties(const Class& class_) {
    std::vector<llvm::Type*> types {};
    for (const MemberDeclaration& member : class_.body)
        if (auto prop = std::get_if<Variable>(&member.value)) {
            types.push_back(m_structs[prop->type_name.name.name]);
            m_props[class_.name.name.name][prop->name.name] = m_props[class_.name.name.name].size();
        }
    m_structs[class_.name.name.name]->setBody(types);
}

void Codegen::generate_class_methods(const Class& class_) {
    for (const MemberDeclaration& member : class_.body) {
        if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
            auto variables_bak = m_variables;
            m_variables = {};

            std::vector<llvm::Type*> args {};
            for (const auto& arg : constructor->arguments)
                args.push_back(m_structs[arg.first.name.name]);
            llvm::Function* fn = generate_function_entry(m_structs[class_.name.name.name], args,
                std::format("{}_this", class_.name.name.name), class_.name.name.name);
            m_this = { m_builder.CreateAlloca(m_structs[class_.name.name.name]),
                m_structs[class_.name.name.name] };
            auto arg_iter = fn->arg_begin();
            for (const auto& arg : constructor->arguments) {
                m_variables[arg.second.name] = {
                    m_builder.CreateAlloca(m_structs[arg.first.name.name]), var_name()
                };
                m_builder.CreateStore(&*arg_iter++, m_variables[arg.second.name].first);
            }
            for (const Statement& stmt : constructor->body) {
                generate_statement(stmt);
            }
            m_builder.CreateRet(
                m_builder.CreateLoad(m_structs[class_.name.name.name], m_this.value().first));

            m_this = {};
            m_variables = variables_bak;
            m_functions[class_.name.name.name]["this"].push_back({ args, fn });
        }
        if (auto method = std::get_if<MemberDeclaration::Method>(&member.value)) {
            auto variables_bak = m_variables;
            m_variables = {};

            std::vector<llvm::Type*> args { m_structs[class_.name.name.name] };
            for (const auto& arg : method->arguments)
                args.push_back(m_structs[arg.first.name.name]);
            llvm::Type* return_type;
            if (method->return_type.name.name == "Void")
                return_type = llvm::Type::getVoidTy(m_context);
            else
                return_type = m_structs[method->return_type.name.name];
            llvm::Function* fn = generate_function_entry(
                return_type, args, method->name.name, class_.name.name.name);

            auto arg_iter = fn->arg_begin();
            m_this = { m_builder.CreateAlloca(m_structs[class_.name.name.name]),
                m_structs[class_.name.name.name] };
            m_builder.CreateStore(&*arg_iter++, m_this.value().first);
            for (const auto& arg : method->arguments) {
                m_variables[arg.second.name] = {
                    m_builder.CreateAlloca(m_structs[arg.first.name.name]), var_name()
                };
                m_builder.CreateStore(&*arg_iter++, m_variables[arg.second.name].first);
            }
            for (const Statement& stmt : method->body) {
                generate_statement(stmt);
            }

            if (method->return_type == TypeName { { "Void" }, {} })
                m_builder.CreateRetVoid();

            m_this = {};
            m_variables = variables_bak;
            m_functions[class_.name.name.name][method->name.name].push_back({ args, fn });
        }
    }
}

llvm::Function* Codegen::generate_function_entry(llvm::Type* return_type,
    std::vector<llvm::Type*> args, std::string method_name, std::string struct_name) {

    std::string signature {};
    for (llvm::Type* arg : args)
        signature += arg->getStructName().str();

    auto* fn_type = llvm::FunctionType::get(return_type, args, false);
    auto* fn =
        llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, method_name, *m_module);
    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(m_context, std::format("{}_{}_entry", method_name, signature), fn);
    m_builder.SetInsertPoint(entry);
    m_functions[struct_name][method_name].push_back({ args, fn });
    return fn;
}

llvm::Type* Codegen::get_type(const TypeName& type) {
    if (type.generic_arguments.size() == 0)
        return m_structs[type.name.name];
}

llvm::Value* Codegen::generate_literal(const Expression::Literal& literal) {
    if (literal.type == Expression::Literal::Type::Int) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["Integer"]);
        return m_builder.CreateInsertValue(
            val, m_builder.getInt32(std::stoi(literal.value)), { 0 });
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
        val = m_builder.CreateInsertValue(val, m_builder.getInt32(literal.value.size()), 1);
        return val;
    }
};

llvm::Value* Codegen::generate_method_call(const Expression::MethodCall& call) {
    llvm::Value* object = generate_expression(*call.object);
    std::string type = object->getType()->getStructName().str();

    std::vector<llvm::Value*> args = { object };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg));

    std::vector<llvm::Type*> arg_types = {};
    for (llvm::Value* arg : args)
        arg_types.push_back(arg->getType());

    llvm::Function* fn;
    for (auto overload : m_functions[type][call.method.name])
        if (overload.first == arg_types)
            fn = overload.second;

    llvm::Value* fn_call = m_builder.CreateCall(fn, args);
    return fn_call;
}

llvm::Value* Codegen::generate_constructor_call(const Expression::ConstructorCall& call) {
    std::string type = stringify(call.type_name);

    std::vector<llvm::Value*> args = {};
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg));

    std::vector<llvm::Type*> arg_types = {};
    for (llvm::Value* arg : args)
        arg_types.push_back(arg->getType());

    llvm::Function* fn;
    for (auto overload : m_functions[type]["this"])
        if (overload.first == arg_types) {
            fn = overload.second;
        }

    llvm::Value* fn_call = m_builder.CreateCall(fn, args);
    return fn_call;
}

llvm::Value* Codegen::generate_this_access(const Expression::ThisAccess& access) {
    int index = m_props[m_this.value().second->getStructName().str()][access.member.name];
    llvm::Value* val = m_builder.CreateLoad(m_this.value().second, m_this.value().first);
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
    if (auto ident = std::get_if<Identifier>(&expr.value))
        return m_builder.CreateLoad(m_variables[ident->name].first->getAllocatedType(),
            m_variables[ident->name].first, (m_variables[ident->name].second + "_val"));
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
}

void Codegen::generate_variable(const Variable& variable) {
    std::string name = var_name();
    llvm::Type* type = get_type(variable.type_name);
    llvm::AllocaInst* alloc = m_builder.CreateAlloca(type, nullptr, name);

    if (variable.value.has_value()) {
        llvm::Value* val = generate_expression(variable.value.value());
        m_builder.CreateStore(val, alloc);
    }
    m_variables[variable.name.name] = { alloc, name };
}

void Codegen::generate_assignment(const Statement::Assignment& assign) {
    if (auto ident = std::get_if<Identifier>(&assign.left.value)) {
        m_builder.CreateStore(generate_expression(assign.right), m_variables[ident->name].first);
    }
    if (auto access = std::get_if<Expression::ThisAccess>(&assign.left.value)) {
        int index = m_props[m_this.value().second->getStructName().str()][access->member.name];
        llvm::Value* val =
            m_builder.CreateStructGEP(m_this.value().second, m_this.value().first, index);
        m_builder.CreateStore(generate_expression(assign.right), val);
    }
}

void Codegen::generate_return(const Statement::Return& ret) {
    m_builder.CreateRet(generate_expression(ret.value));
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
}

void Codegen::generate() {
    generate_integer();
    generate_bool();
    generate_stdio();
    generate_string();
    generate_void();

    generate_stdio_methods();
    generate_integer_methods();
    generate_string_methods();

    generate_classes();

    m_module->print(llvm::outs(), nullptr);
    for (const Class& class_ : m_ast.classes) {
        if (class_.name.name.name == "Program") {
            for (const MemberDeclaration& member : class_.body) {
                if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
                    llvm::FunctionType* func_type =
                        llvm::FunctionType::get(llvm::Type::getVoidTy(m_context), false);

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
