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

typedef std::unordered_map<std::string, TypeName> Generics;

class Codegen {
private:
    Root m_ast;

    struct VarInfo {
        llvm::Value* ptr;
        std::string gen_name;
        llvm::Type* type;
    };

    struct FuncInfo {
        std::vector<llvm::Type*> args;
        llvm::Function* func;
        std::vector<int> owner;
    };

    struct MethodSignature {
        std::vector<llvm::Type*> args;
        llvm::Type* return_type;
    };

    std::optional<std::pair<llvm::Value*, llvm::Type*>> m_this;

    std::unordered_map<std::string, VarInfo> m_variables;
    unsigned long long m_var_count;

    std::set<TypeName> m_postponed;
    std::unordered_map<std::string, Class> m_definitions;
    std::unordered_map<std::string, llvm::StructType*> m_structs;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<FuncInfo>>>
        m_functions;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> m_props;

    llvm::LLVMContext m_context;
    llvm::IRBuilder<> m_builder;
    std::unique_ptr<llvm::Module> m_module;
    Analyzer m_analyzer;

    std::string var_name();
    bool parents_generated(const Class&);
    MethodSignature get_signature(const MemberDeclaration::Method&);

    void generate_stdio();
    void generate_string();
    void generate_integer();
    void generate_real();
    void generate_bool();

    void generate_stdio_methods();
    void generate_integer_methods();
    void generate_real_methods();
    void generate_string_methods();
    void generate_bool_methods();

    void generate_classes();
    void generate_class(Class class_);
    void generate_class_properties(Class class_);
    void generate_class_method_declarations(Class class_);
    void generate_class_method_implementations(Class class_);

    llvm::Function* generate_function_entry(llvm::Type* return_type, std::vector<llvm::Type*> args,
        std::string method_name, std::string struct_name, bool insert = true);

    llvm::Value* generate_literal(const Expression::Literal&);
    llvm::Value* generate_method_call(const Expression::MethodCall&);
    llvm::Value* generate_this_call(const Expression::ThisCall&);
    llvm::Value* generate_constructor_call(const Expression::ConstructorCall&);
    llvm::Value* generate_this_access(const Expression::ThisAccess&);
    llvm::Value* generate_member_access(const Expression::MemberAccess&);
    llvm::Value* generate_expression(const Expression&);
    std::pair<llvm::Value*, llvm::Type*> generate_lvalue(const Expression&);

    void generate_variable(const Variable&);
    void generate_assignment(const Statement::Assignment&);
    void generate_if(const Statement::If&);
    void generate_while(const Statement::While&);
    void generate_return(const Statement::Return&);
    void generate_super_call(const Statement::SuperCall&);
    void generate_statement(const Statement&);

public:
    Codegen(Root root);
    void generate();
};
