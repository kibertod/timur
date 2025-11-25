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
        return m_builder.CreateInsertValue(val, m_builder.CreateGlobalString(literal.value), { 0 });
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
}

void Codegen::generate_statement(const Statement& stmt) {
    if (auto var = std::get_if<Variable>(&stmt.value))
        generate_variable(*var);
    if (auto expr = std::get_if<Expression>(&stmt.value))
        generate_expression(*expr);
    if (auto assign = std::get_if<Statement::Assignment>(&stmt.value))
        generate_assignment(*assign);
}

void Codegen::generate() {
    generate_integer();
    generate_bool();
    generate_stdio();
    generate_string();

    generate_stdio_methods();
    generate_integer_methods();

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
