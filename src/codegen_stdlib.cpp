#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>
#include <memory>

#include "codegen.h"

void Codegen::generate_void() {
    generate_function_entry(m_builder.getVoidTy(), {}, "this", "Void");
    m_builder.CreateRetVoid();
}

void Codegen::generate_string() {
    llvm::StructType* string = llvm::StructType::create(m_context, "String");
    string->setBody({ llvm::PointerType::get(llvm::PointerType::getInt8Ty(m_context), 0),
        m_builder.getInt64Ty() });
    m_structs["String"] = string;
}

void Codegen::generate_stdio() {
    llvm::StructType* stdio = llvm::StructType::create(m_context, "StdIO");
    stdio->setBody({ m_builder.getInt1Ty() });
    m_structs["StdIO"] = stdio;
}

void Codegen::generate_bool() {
    llvm::StructType* bool_ = llvm::StructType::create(m_context, "Bool");
    bool_->setBody({ m_builder.getInt1Ty() });
    m_structs["Bool"] = bool_;
}

void Codegen::generate_integer() {
    llvm::StructType* integer = llvm::StructType::create(m_context, "Integer");
    integer->setBody({ m_builder.getInt64Ty() });
    m_structs["Integer"] = integer;
}

void Codegen::generate_real() {
    llvm::StructType* real = llvm::StructType::create(m_context, "Real");
    real->setBody({ m_builder.getDoubleTy() });
    m_structs["Real"] = real;
}

void Codegen::generate_real_methods() {
    llvm::StructType* real = m_structs["Real"];
    llvm::PointerType* real_ptr = llvm::PointerType::get(real, 0);
    llvm::StructType* integer = m_structs["Integer"];
    TypeName real_tn = { { "Real" }, {} };

    // this(int)
    {
        llvm::Function* fn = generate_function_entry(real, { integer }, "this", "Real");
        llvm::Value* int_val = m_builder.CreateExtractValue(fn->arg_begin(), 0);
        llvm::Value* real_val = m_builder.CreateSIToFP(int_val, m_builder.getDoubleTy());
        m_builder.CreateRet(m_builder.CreateInsertValue(llvm::UndefValue::get(real), real_val, 0));
    }

    // plus
    {
        llvm::Function* fn = generate_function_entry(real, { real_ptr, real }, "Plus", "Real");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(real, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateFAdd(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(real);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // minus
    {
        llvm::Function* fn = generate_function_entry(real, { real_ptr, real }, "Minus", "Real");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(real, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateFSub(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(real);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // div
    {
        llvm::Function* fn = generate_function_entry(real, { real_ptr, real }, "Div", "Real");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(real, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateFDiv(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(real);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // mult
    {
        llvm::Function* fn = generate_function_entry(real, { real_ptr, real }, "Mult", "Real");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(real, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateFMul(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(real);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // eq
    {
        llvm::Function* fn =
            generate_function_entry(m_structs["Bool"], { real_ptr, real }, "Eq", "Real");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(real, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateFCmpOEQ(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(m_structs["Bool"]);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // great
    {
        llvm::Function* fn =
            generate_function_entry(m_structs["Bool"], { real_ptr, real }, "Great", "Real");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(real, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateFCmpOGT(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(m_structs["Bool"]);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // ceil
    {
        llvm::Function* fn = generate_function_entry(real, { real_ptr }, "Ceil", "Real");
        llvm::Value* val = m_builder.CreateLoad(real, fn->arg_begin());
        val = m_builder.CreateExtractValue(val, 0);
        llvm::Function* ceil = llvm::Intrinsic::getOrInsertDeclaration(
            m_module.get(), llvm::Intrinsic::ceil, { m_builder.getDoubleTy() });
        llvm::Value* res = m_builder.CreateCall(ceil, { val });
        m_builder.CreateRet(m_builder.CreateInsertValue(llvm::UndefValue::get(real), res, 0));
    }

    // floor
    {
        llvm::Function* fn = generate_function_entry(real, { real_ptr }, "Floor", "Real");
        llvm::Value* val = m_builder.CreateLoad(real, fn->arg_begin());
        val = m_builder.CreateExtractValue(val, 0);
        llvm::Function* floor = llvm::Intrinsic::getOrInsertDeclaration(
            m_module.get(), llvm::Intrinsic::floor, { m_builder.getDoubleTy() });
        llvm::Value* res = m_builder.CreateCall(floor, { val });
        m_builder.CreateRet(m_builder.CreateInsertValue(llvm::UndefValue::get(real), res, 0));
    }

    // int
    {
        llvm::Function* fn =
            generate_function_entry(m_structs["Integer"], { real_ptr }, "Int", "Real");
        llvm::Value* val = m_builder.CreateLoad(real, fn->arg_begin());
        val = m_builder.CreateExtractValue(val, 0);
        llvm::Value* res = m_builder.CreateFPToSI(val, m_builder.getInt64Ty());
        m_builder.CreateRet(
            m_builder.CreateInsertValue(llvm::UndefValue::get(m_structs["Integer"]), res, 0));
    }
}

void Codegen::generate_stdio_methods() {
    llvm::Type* stdio = m_structs["StdIO"];
    llvm::PointerType* stdio_ptr = llvm::PointerType::get(stdio, 0);
    auto* i8ptr = llvm::PointerType::get(llvm::PointerType::getInt8Ty(m_context), 0);
    auto* printf_type = llvm::FunctionType::get(m_builder.getInt32Ty(), { i8ptr }, true);

    llvm::Function* fn_printf =
        llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, "printf", *m_module);
    // this
    {
        generate_function_entry(stdio, {}, "this", "StdIO");
        llvm::Value* val = m_builder.CreateAlloca(stdio);
        llvm::Value* field = m_builder.CreateStructGEP(stdio, val, 0);
        m_builder.CreateStore(m_builder.getInt1(true), field);
        m_builder.CreateRet(m_builder.CreateLoad(stdio, val));
    }

    // print int
    {
        llvm::Function* print = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["Integer"] }, "Print", "StdIO");
        {
            auto arg_iter = print->arg_begin();
            llvm::Value* a = ++arg_iter;
            llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
            llvm::Value* fmt = m_builder.CreateGlobalString("%d");
            m_builder.CreateCall(fn_printf, { fmt, a_val });
            m_builder.CreateRetVoid();
        }

        // ln
        llvm::Function* println = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["Integer"] }, "PrintLn", "StdIO");
        auto arg_iter = println->arg_begin();
        llvm::Value* self = arg_iter++;
        llvm::Value* a = arg_iter;
        m_builder.CreateCall(print, { self, a });
        llvm::Value* fmt = m_builder.CreateGlobalString("\n");
        m_builder.CreateCall(fn_printf, { fmt });
        m_builder.CreateRetVoid();
    }

    // print real
    {
        llvm::Function* print = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["Real"] }, "Print", "StdIO");
        {
            auto arg_iter = print->arg_begin();
            llvm::Value* a = ++arg_iter;
            llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
            llvm::Value* fmt = m_builder.CreateGlobalString("%f");
            m_builder.CreateCall(fn_printf, { fmt, a_val });
            m_builder.CreateRetVoid();
        }

        // ln
        llvm::Function* println = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["Real"] }, "PrintLn", "StdIO");
        auto arg_iter = println->arg_begin();
        llvm::Value* self = arg_iter++;
        llvm::Value* a = arg_iter;
        m_builder.CreateCall(print, { self, a });
        llvm::Value* fmt = m_builder.CreateGlobalString("\n");
        m_builder.CreateCall(fn_printf, { fmt });
        m_builder.CreateRetVoid();
    }

    // print str
    {
        llvm::Function* print = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["String"] }, "Print", "StdIO");
        {
            auto arg_iter = print->arg_begin();
            llvm::Value* a = ++arg_iter;
            llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
            llvm::Value* fmt = m_builder.CreateGlobalString("%s");
            m_builder.CreateCall(fn_printf, { fmt, a_val });
            m_builder.CreateRetVoid();
        }

        // ln
        llvm::Function* println = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["String"] }, "PrintLn", "StdIO");
        auto arg_iter = println->arg_begin();
        llvm::Value* self = arg_iter++;
        llvm::Value* a = arg_iter;
        m_builder.CreateCall(print, { self, a });
        llvm::Value* fmt = m_builder.CreateGlobalString("\n");
        m_builder.CreateCall(fn_printf, { fmt });
        m_builder.CreateRetVoid();
    }

    // print bool
    {
        llvm::Function* print = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["Bool"] }, "Print", "StdIO");
        {
            auto arg_iter = print->arg_begin();
            llvm::Value* a = ++arg_iter;
            llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
            llvm::Value* true_ = m_builder.CreateGlobalString("true");
            llvm::Value* false_ = m_builder.CreateGlobalString("false");
            llvm::Value* val = m_builder.CreateSelect(a_val, true_, false_);
            llvm::Value* fmt = m_builder.CreateGlobalString("%s");
            m_builder.CreateCall(fn_printf, { fmt, val });
            m_builder.CreateRetVoid();
        }
        // ln
        llvm::Function* println = generate_function_entry(
            m_builder.getVoidTy(), { stdio_ptr, m_structs["Bool"] }, "PrintLn", "StdIO");
        auto arg_iter = println->arg_begin();
        llvm::Value* self = arg_iter++;
        llvm::Value* a = arg_iter;
        m_builder.CreateCall(print, { self, a });
        llvm::Value* fmt = m_builder.CreateGlobalString("\n");
        m_builder.CreateCall(fn_printf, { fmt });
        m_builder.CreateRetVoid();
    }
}

void Codegen::generate_integer_methods() {
    llvm::StructType* integer = m_structs["Integer"];
    llvm::PointerType* integer_ptr = llvm::PointerType::get(integer, 0);
    TypeName integer_tn = { { "Integer" }, {} };
    // plus
    {
        llvm::Function* fn =
            generate_function_entry(integer, { integer_ptr, integer }, "Plus", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(integer, arg_iter++);
        llvm::Value* b = arg_iter;
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
            generate_function_entry(integer, { integer_ptr, integer }, "Minus", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(integer, arg_iter++);
        llvm::Value* b = arg_iter;
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
            generate_function_entry(integer, { integer_ptr, integer }, "Div", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(integer, arg_iter++);
        llvm::Value* b = arg_iter;
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
            generate_function_entry(integer, { integer_ptr, integer }, "Mult", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(integer, arg_iter++);
        llvm::Value* b = arg_iter;
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
            generate_function_entry(m_structs["Bool"], { integer_ptr, integer }, "Eq", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(integer, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateICmpEQ(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(m_structs["Bool"]);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }

    // great
    {
        llvm::Function* fn = generate_function_entry(
            m_structs["Bool"], { integer_ptr, integer }, "Great", "Integer");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(integer, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_val = m_builder.CreateExtractValue(a, { 0 });
        llvm::Value* b_val = m_builder.CreateExtractValue(b, { 0 });
        llvm::Value* op = m_builder.CreateICmpSGT(a_val, b_val);
        llvm::Value* result = llvm::UndefValue::get(m_structs["Bool"]);
        result = m_builder.CreateInsertValue(result, op, { 0 });
        m_builder.CreateRet(result);
    }
}

void Codegen::generate_string_methods() {
    llvm::Type* string = m_structs["String"];
    llvm::Type* string_ptr = llvm::PointerType::get(string, 0);
    auto* i8ptr = llvm::PointerType::get(llvm::PointerType::getInt8Ty(m_context), 0);
    auto* strcat_type = llvm::FunctionType::get(i8ptr, { i8ptr, i8ptr }, true);
    llvm::Function* strcat =
        llvm::Function::Create(strcat_type, llvm::Function::ExternalLinkage, "strcat", *m_module);

    // concat
    {
        llvm::Function* fn =
            generate_function_entry(string, { string_ptr, string }, "Concat", "String");
        auto arg_iter = fn->arg_begin();
        llvm::Value* a = m_builder.CreateLoad(string, arg_iter++);
        llvm::Value* b = arg_iter;
        llvm::Value* a_ptr = m_builder.CreateExtractValue(a, 0);
        llvm::Value* b_ptr = m_builder.CreateExtractValue(b, 0);
        llvm::Value* a_size = m_builder.CreateExtractValue(a, 1);
        llvm::Value* b_size = m_builder.CreateExtractValue(b, 1);
        llvm::Value* new_size = m_builder.CreateAdd(m_builder.getInt64(1), a_size);
        new_size = m_builder.CreateAdd(new_size, b_size);
        llvm::Value* new_ptr = m_builder.CreateMalloc(m_builder.getInt8Ty(), m_builder.getInt8Ty(),
            m_builder.getInt64(1), m_builder.CreateZExt(new_size, m_builder.getInt64Ty()));
        m_builder.CreateStore(m_builder.getInt8(0), new_ptr);
        m_builder.CreateCall(strcat, { new_ptr, a_ptr });
        m_builder.CreateCall(strcat, { new_ptr, b_ptr });
        llvm::Value* res = llvm::UndefValue::get(string);
        res = m_builder.CreateInsertValue(res, new_ptr, 0);
        res = m_builder.CreateInsertValue(res, new_size, 1);
        m_builder.CreateRet(res);
    }
}

void Codegen::generate_bool_methods() {
    llvm::StructType* bool_ = m_structs["Bool"];
    llvm::PointerType* bool_ptr = llvm::PointerType::get(bool_, 0);

    // not
    {
        llvm::Function* fn = generate_function_entry(bool_, { bool_ptr }, "Not", "Bool");
        auto arg_iter = fn->arg_begin();
        llvm::Value* val = m_builder.CreateLoad(bool_, arg_iter++);
        val = m_builder.CreateNot(m_builder.CreateExtractValue(val, 0));
        llvm::Value* res = llvm::UndefValue::get(bool_);
        res = m_builder.CreateInsertValue(res, val, 0);
        m_builder.CreateRet(res);
    }

    // and
    {
        llvm::Function* fn = generate_function_entry(bool_, { bool_ptr, bool_ }, "And", "Bool");
        auto arg_iter = fn->arg_begin();
        llvm::Value* val = m_builder.CreateLoad(bool_, arg_iter++);
        llvm::Value* other = arg_iter++;
        val = m_builder.CreateAnd(
            m_builder.CreateExtractValue(val, 0), m_builder.CreateExtractValue(other, 0));
        llvm::Value* res = llvm::UndefValue::get(bool_);
        res = m_builder.CreateInsertValue(res, val, 0);
        m_builder.CreateRet(res);
    }

    // or
    {
        llvm::Function* fn = generate_function_entry(bool_, { bool_ptr, bool_ }, "Or", "Bool");
        auto arg_iter = fn->arg_begin();
        llvm::Value* val = m_builder.CreateLoad(bool_, arg_iter++);
        llvm::Value* other = arg_iter++;
        val = m_builder.CreateOr(
            m_builder.CreateExtractValue(val, 0), m_builder.CreateExtractValue(other, 0));
        llvm::Value* res = llvm::UndefValue::get(bool_);
        res = m_builder.CreateInsertValue(res, val, 0);
        m_builder.CreateRet(res);
    }

    // xor
    {
        llvm::Function* fn = generate_function_entry(bool_, { bool_ptr, bool_ }, "Xor", "Bool");
        auto arg_iter = fn->arg_begin();
        llvm::Value* val = m_builder.CreateLoad(bool_, arg_iter++);
        llvm::Value* other = arg_iter++;
        val = m_builder.CreateXor(
            m_builder.CreateExtractValue(val, 0), m_builder.CreateExtractValue(other, 0));
        llvm::Value* res = llvm::UndefValue::get(bool_);
        res = m_builder.CreateInsertValue(res, val, 0);
        m_builder.CreateRet(res);
    }
}
