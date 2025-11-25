#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include <format>
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

void Codegen::generate_string() {
    llvm::StructType* string = llvm::StructType::create(m_context, "String");
    m_structs["String"] = string;
    string->setBody({ llvm::PointerType::get(llvm::PointerType::getInt8Ty(m_context), 0) });
}

void Codegen::generate_stdio() {
    llvm::StructType* stdio = llvm::StructType::create(m_context, "StdIO");
    m_structs["StdIO"] = stdio;
    stdio->setBody({ llvm::Type::getInt1Ty(m_context) });
}

void Codegen::generate_stdio_methods() {
    auto* i8ptr = llvm::PointerType::get(llvm::PointerType::getInt8Ty(m_context), 0);
    auto* printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(m_context), { i8ptr }, true);

    llvm::Function* fn_printf =
        llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, "printf", *m_module);
    // this
    {
        llvm::Function* fn = generate_function_entry(m_structs["StdIO"], {}, "this", "StdIO");
        llvm::Value* val = llvm::UndefValue::get(m_structs["StdIO"]);
        val = m_builder.CreateInsertValue(val, m_builder.getInt1(true), { 0 });
        m_builder.CreateRet(val);
    }

    // print int
    {
        llvm::Function* fn = generate_function_entry(llvm::Type::getVoidTy(m_context),
            { m_structs["StdIO"], m_structs["Integer"] }, "PrintLn", "StdIO");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*(++arg_iter);
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });

        llvm::Value* fmt = m_builder.CreateGlobalString("%d\n");
        m_builder.CreateCall(fn_printf, { fmt, a_val });

        m_builder.CreateRetVoid();
    }

    // print str
    {
        llvm::Function* fn = generate_function_entry(llvm::Type::getVoidTy(m_context),
            { m_structs["StdIO"], m_structs["String"] }, "PrintLn", "StdIO");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*(++arg_iter);
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });

        llvm::Value* fmt = m_builder.CreateGlobalString("%s\n");
        m_builder.CreateCall(fn_printf, { fmt, a_val });

        m_builder.CreateRetVoid();
    }
}

void Codegen::generate_bool() {
    llvm::StructType* bool_ = llvm::StructType::create(m_context, "Bool");
    bool_->setBody({ llvm::Type::getInt1Ty(m_context) });
    m_structs["Bool"] = bool_;
    m_functions["Bool"] = {};
}

void Codegen::generate_integer() {
    llvm::StructType* integer = llvm::StructType::create(m_context, "Integer");
    integer->setBody({ llvm::Type::getInt32Ty(m_context) });
    m_structs["Integer"] = integer;
    m_functions["Integer"] = {};
}

void Codegen::generate_integer_methods() {
    llvm::StructType* integer = m_structs["Integer"];
    TypeName integer_tn = { { "Integer" }, {} };
    // plus
    {
        llvm::Function* fn =
            generate_function_entry(integer, { integer, integer }, "Plus", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*arg_iter++;
        llvm::Value* b = &*arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateAdd(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(integer);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // minus
    {
        llvm::Function* fn =
            generate_function_entry(integer, { integer, integer }, "Minus", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*arg_iter++;
        llvm::Value* b = &*arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateSub(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(integer);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // div
    {
        llvm::Function* fn =
            generate_function_entry(integer, { integer, integer }, "Div", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*arg_iter++;
        llvm::Value* b = &*arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateSDiv(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(integer);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // mult
    {
        llvm::Function* fn =
            generate_function_entry(integer, { integer, integer }, "Mult", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*arg_iter++;
        llvm::Value* b = &*arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateMul(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(integer);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // eq
    {
        llvm::Function* fn =
            generate_function_entry(m_structs["Bool"], { integer, integer }, "Eq", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*arg_iter++;
        llvm::Value* b = &*arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateICmpEQ(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(m_structs["Bool"]);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // great
    {
        llvm::Function* fn =
            generate_function_entry(m_structs["Bool"], { integer, integer }, "Great", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*arg_iter++;
        llvm::Value* b = &*arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateICmpSGT(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(m_structs["Bool"]);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
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
        llvm::Value* var = m_builder.CreateAlloca(m_structs["Integer"], nullptr);
        llvm::Value* val = m_builder.CreateStructGEP(m_structs["Integer"], var, 0);
        m_builder.CreateStore(m_builder.getInt32(std::stoi(literal.value)), val);
        return m_builder.CreateLoad(m_structs["Integer"], var);
    }
    if (literal.type == Expression::Literal::Type::Bool) {
        llvm::Value* var = m_builder.CreateAlloca(m_structs["Bool"], nullptr);
        llvm::Value* val = m_builder.CreateStructGEP(m_structs["Bool"], var, 0);
        if (literal.value == "true")
            m_builder.CreateStore(m_builder.getInt1(true), val);
        else
            m_builder.CreateStore(m_builder.getInt1(false), val);
        return m_builder.CreateLoad(m_structs["Bool"], var);
    }
    if (literal.type == Expression::Literal::Type::Str) {
        llvm::Value* var = m_builder.CreateAlloca(m_structs["String"], nullptr);
        llvm::Value* val = m_builder.CreateStructGEP(m_structs["String"], var, 0);
        m_builder.CreateStore(m_builder.CreateGlobalString(literal.value), val);
        return m_builder.CreateLoad(m_structs["String"], var);
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
        llvm::Value* val = generate_expression(variable.value.value()); // i32
        llvm::Value* field_ptr = m_builder.CreateStructGEP(type, alloc, 0);
        m_builder.CreateStore(val, field_ptr);
    }
    m_variables[variable.name.name] = { alloc, name };
}

void Codegen::generate_statement(const Statement& stmt) {
    if (auto var = std::get_if<Variable>(&stmt.value))
        generate_variable(*var);
    if (auto expr = std::get_if<Expression>(&stmt.value))
        generate_expression(*expr);
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
    if (llvm::verifyModule(*m_module, &llvm::errs())) {
        llvm::errs() << "Error constructing LLVM module!\n";
        return;
    }

    m_module->print(llvm::outs(), nullptr);
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
