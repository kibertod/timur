#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Bitcode/BitcodeWriter.h"

#include "ast.h"
#include "seman.h"
#include <memory>
#include <unordered_map>

using namespace ast;

class Codegen {
private:
    Root m_ast;

    struct VarInfo {
        llvm::AllocaInst* ptr;
        std::string gen_name;
        llvm::Type* type;
    };

    std::optional<std::pair<llvm::AllocaInst*, llvm::Type*>> m_this;
    std::unordered_map<std::string, VarInfo> m_variables;
    unsigned long long m_var_count;

    std::unordered_map<std::string, llvm::StructType*> m_structs;
    std::unordered_map<std::string,
        std::unordered_map<std::string,
            std::vector<std::pair<std::vector<llvm::Type*>, llvm::Function*>>>>
        m_functions;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> m_props;

    llvm::LLVMContext m_context;
    llvm::IRBuilder<> m_builder;
    std::unique_ptr<llvm::Module> m_module;
    Analyzer m_analyzer;

    std::string var_name();

    void generate_stdio();
    void generate_string();
    void generate_integer();
    void generate_real();
    void generate_bool();
    void generate_void();

    void generate_stdio_methods();
    void generate_integer_methods();
    void generate_real_methods();
    void generate_string_methods();
    void generate_bool_methods();

    void generate_classes();
    void generate_class(const Class& class_);
    void generate_class_properties(const Class& class_);
    void generate_class_methods(const Class& class_);

    llvm::Function* generate_function_entry(llvm::Type* return_type, std::vector<llvm::Type*> args,
        std::string method_name, std::string struct_name);

    llvm::Value* generate_literal(const Expression::Literal& literal);
    llvm::Value* generate_method_call(const Expression::MethodCall& call);
    llvm::Value* generate_constructor_call(const Expression::ConstructorCall& call);
    llvm::Value* generate_this_access(const Expression::ThisAccess& access);
    llvm::Value* generate_member_access(const Expression::MemberAccess& access);
    llvm::Value* generate_expression(const Expression& expr);
    std::pair<llvm::Value*, llvm::Type*> generate_lvalue(const Expression& expr);

    void generate_variable(const Variable& var);
    void generate_assignment(const Statement::Assignment& assign);
    void generate_if(const Statement::If& if_);
    void generate_while(const Statement::While& while_);
    void generate_return(const Statement::Return& ret);
    void generate_statement(const Statement& stmt);

    llvm::Type* get_type(const TypeName& type);

public:
    Codegen(Root root);
    void generate();
};
