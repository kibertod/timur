#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include <memory>
#include <variant>

#include "codegen.h"
#include "ast.h"

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

void Codegen::generate_integer() {
    llvm::StructType* integer = llvm::StructType::create(m_context, "Integer");
    integer->setBody({ llvm::Type::getInt32Ty(m_context) });
    m_structs["Integer"] = integer;
    m_functions["Integer"] = {};

    llvm::Type* args[2] = { integer, integer };
    auto* fn_type = llvm::FunctionType::get(integer, args, false);
    auto* fn = llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, "Plus", *m_module);
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(m_context, "Plus_entry", fn);
    m_builder.SetInsertPoint(entry);
    m_functions["Integer"]["Plus"] = fn;

    auto arg_iter = fn->arg_begin();
    llvm::Value* a = &*arg_iter++;
    llvm::Value* b = &*arg_iter;
    a->setName(var_name());
    b->setName(var_name());
    llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 }, "a_val");
    llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 }, "b_val");
    llvm::Value* sum = m_builder.CreateAdd(a_val, b_val, "sum");
    llvm::Value* result = llvm::UndefValue::get(integer);

    result = m_builder.CreateInsertValue(result, sum, { 0 }, "result");

    m_builder.CreateRet(result);
}

llvm::Type* Codegen::get_type(const TypeName& type) {
    if (type.name.name == "Integer")
        return m_structs["Integer"];
}

llvm::Value* Codegen::generate_literal(const Expression::Literal& literal) {
    if (literal.type == Expression::Literal::Type::Int) {
        llvm::Value* var = m_builder.CreateAlloca(m_structs["Integer"], nullptr, var_name());
        llvm::Value* val = m_builder.CreateStructGEP(m_structs["Integer"], var, 0, var_name());
        m_builder.CreateStore(m_builder.getInt32(std::stoi(literal.value)), val);
        return var;
    }
};

llvm::Value* Codegen::generate_method_call(const Expression::MethodCall& call) {
    llvm::Value* object = generate_expression(*call.object);
    std::string type = object->getType()->getStructName().str();
    llvm::Function* fn = m_functions[type][call.method.name];

    std::vector<llvm::Value*> args = { object };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg));
    llvm::Value* fn_call = m_builder.CreateCall(fn, args, var_name());
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
}

void Codegen::generate_variable(const Variable& variable) {
    std::string name = var_name();
    llvm::Type* type = get_type(variable.type_name);
    llvm::AllocaInst* alloc = m_builder.CreateAlloca(type, nullptr, name);

    if (variable.value.has_value()) {
        m_builder.CreateStore(generate_expression(variable.value.value()), alloc);
    }
    m_variables[variable.name.name] = { alloc, name };
}

void Codegen::generate_statement(const Statement& stmt) {
    if (auto var = std::get_if<Variable>(&stmt.value))
        generate_variable(*var);
}

void Codegen::generate() {
    generate_integer();
    for (const Class& class_ : m_ast.classes) {
        if (class_.name.name.name == "Program") {
            for (const MemberDeclaration& member : class_.body) {
                if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
                    llvm::FunctionType* func_type =
                        llvm::FunctionType::get(llvm::Type::getVoidTy(m_context), false);

                    llvm::Function* program_fn = llvm::Function::Create(
                        func_type, llvm::Function::ExternalLinkage, "program_main", m_module.get());

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
    if (llvm::verifyModule(*m_module, &llvm::errs())) {
        llvm::errs() << "Error constructing LLVM module!\n";
        return;
    }

    m_module->print(llvm::outs(), nullptr);
}

Codegen::Codegen(Root root) :
    m_builder(m_context) {
    m_ast = root;
    m_var_count = 1;
    m_module = std::make_unique<llvm::Module>("main_module", m_context);
};
