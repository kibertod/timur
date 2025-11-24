#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/BitcodeWriter.h"

#include "ast.h"
#include <memory>
#include <unordered_map>

using namespace ast;

class Codegen {
private:
    Root m_ast;

    std::unordered_map<std::string, std::pair<llvm::AllocaInst*, std::string>> m_variables;
    unsigned long long m_var_count;

    std::unordered_map<std::string, llvm::StructType*> m_structs;
    std::unordered_map<std::string, std::unordered_map<std::string, llvm::Function*>> m_functions;

    llvm::LLVMContext m_context;
    llvm::IRBuilder<> m_builder;
    std::unique_ptr<llvm::Module> m_module;

    std::string var_name();

    void generate_integer();

    llvm::Value* generate_literal(const Expression::Literal& literal);
    llvm::Value* generate_method_call(const Expression::MethodCall& call);
    llvm::Value* generate_expression(const Expression& expr);

    void generate_variable(const Variable& var);
    void generate_statement(const Statement& stmt);

    llvm::Type* get_type(const TypeName& type);

public:
    Codegen(Root root);
    void generate();
};
