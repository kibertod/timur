#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <memory>

#include "codegen.h"

void Codegen::generate_string() {
    llvm::StructType* string = llvm::StructType::create(m_context, "String");
    m_structs["String"] = string;
    string->setBody({ llvm::PointerType::get(llvm::PointerType::getInt8Ty(m_context), 0) });
}

void Codegen::generate_stdio() {
    llvm::StructType* stdio = llvm::StructType::create(m_context, "StdIO");
    m_structs["StdIO"] = stdio;
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

    // print bool
    {
        llvm::Function* fn = generate_function_entry(llvm::Type::getVoidTy(m_context),
            { m_structs["StdIO"], m_structs["Bool"] }, "PrintLn", "StdIO");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = &*(++arg_iter);
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });

        llvm::Value* true_ = m_builder.CreateGlobalString("true");
        llvm::Value* false_ = m_builder.CreateGlobalString("false");
        llvm::Value* val = m_builder.CreateSelect(a_val, true_, false_);

        llvm::Value* fmt = m_builder.CreateGlobalString("%s\n");
        m_builder.CreateCall(fn_printf, { fmt, val });

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
