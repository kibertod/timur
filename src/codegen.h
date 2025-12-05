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
static std::optional<TypeName> integer_tn = TypeName { { "Integer" }, {}, false };
static std::optional<TypeName> bool_tn = TypeName { { "Bool" }, {}, false };
static std::optional<TypeName> real_tn = TypeName { { "Real" }, {}, false };
static std::optional<TypeName> stdio_tn = TypeName { { "StdIO" }, {}, false };
static std::optional<TypeName> string_tn = TypeName { { "String" }, {}, false };

class Codegen {
private:
    Root m_ast;

    struct VarInfo {
        llvm::Value* ptr;
        std::string gen_name;
        TypeName type;
        llvm::Type* llvm_type;
    };

    struct FuncInfo {
        std::vector<llvm::Type*> args;
        llvm::Function* func;
        std::vector<int> owner;
        std::string name;
        std::optional<TypeName> return_type;
    };

    struct MethodSignature {
        std::vector<llvm::Type*> args;
        llvm::Type* llvm_return;
        std::optional<TypeName> return_type;
    };

    struct GlobalFnKey {
        std::string parent;
        std::string method;
        std::vector<llvm::Type*> args;

        bool operator<(const GlobalFnKey& other) const {
            return std::tie(parent, method, args) <
                   std::tie(other.parent, other.method, other.args);
        };
    };

    struct This {
        llvm::Value* ptr;
        llvm::Type* llvm_ty;
        TypeName tn;
    };

    struct ClassGenStatus {
        bool struct_generated;
        bool props_generated;
        bool fn_def_generated;
        bool fn_impl_generated;
        bool operator==(const ClassGenStatus&) const = default;
    };

    std::optional<This> m_this;

    std::unordered_map<std::string, VarInfo> m_variables;
    unsigned long long m_var_count;

    std::set<TypeName> m_postponed;
    std::unordered_map<std::string, Class> m_definitions;
    std::unordered_map<std::string, ClassGenStatus> m_class_gen_statuses;
    std::unordered_map<std::string, llvm::StructType*> m_structs;
    std::unordered_map<std::string, llvm::StructType*> m_pointers;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<FuncInfo>>>
        m_functions;
    std::unordered_map<std::string, int> m_functions_count;
    std::unordered_map<std::string,
        std::unordered_map<std::string, std::pair<std::vector<int>, TypeName>>>
        m_props;
    std::map<std::pair<std::string, std::string>, std::vector<int>> m_parents;
    llvm::StructType* m_ptr;

    std::map<std::pair<std::string, std::string>, llvm::GlobalVariable*> m_fn_arrays;
    std::map<std::pair<std::string, std::string>, llvm::GlobalVariable*> m_offset_arrays;
    std::map<GlobalFnKey, uint64_t> m_vtable_fn_ids;

    llvm::LLVMContext m_context;
    llvm::IRBuilder<> m_builder;
    llvm::DataLayout m_dl;
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
    void generate_ptr();

    void generate_stdio_methods();
    void generate_integer_methods();
    void generate_real_methods();
    void generate_string_methods();
    void generate_bool_methods();

    void map_child_fn_ptrs(const Class&, const Class&);

    void generate_classes();
    void generate_class(Class);
    void generate_class_properties(Class);
    void generate_class_method_declarations(Class);
    void generate_class_method_implementations(Class);

    llvm::Function* generate_function_entry(llvm::Type*, std::optional<TypeName>,
        std::vector<llvm::Type*>, std::string, std::string, bool = true);

    std::pair<llvm::Value*, std::optional<TypeName>> generate_literal(const Expression::Literal&);
    std::pair<llvm::Value*, std::optional<TypeName>> generate_method_call(
        const Expression::MethodCall&);
    std::pair<llvm::Value*, std::optional<TypeName>> generate_this_call(
        const Expression::ThisCall&);
    std::pair<llvm::Value*, std::optional<TypeName>> generate_constructor_call(
        const Expression::ConstructorCall&);
    std::pair<llvm::Value*, std::optional<TypeName>> generate_this_access(
        const Expression::ThisAccess&);
    std::pair<llvm::Value*, std::optional<TypeName>> generate_member_access(
        const Expression::MemberAccess&);
    std::pair<llvm::Value*, std::optional<TypeName>> generate_expression(const Expression&);
    std::pair<llvm::Value*, TypeName> generate_lvalue(const Expression&);

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
