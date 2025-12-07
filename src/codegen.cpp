#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include <cstdint>
#include <format>
#include <functional>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Alignment.h>
#include <memory>
#include <set>
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

bool Codegen::parents_generated(const Class& class_) {
    for (const TypeName& parent : class_.extends)
        if (m_postponed.contains(parent)) return false;
    return true;
}

llvm::StructType* Codegen::get_struct(const TypeName& tn) {
    if (tn.ptr)
        return m_ptr;
    else
        return m_structs[stringify(tn)];
}

Codegen::MethodSignature Codegen::get_signature(const MemberDeclaration::Method& method) {
    std::vector<llvm::Type*> args { m_builder.getPtrTy(0) };
    for (const auto& arg : method.arguments)
        if (!arg.first.ptr)
            args.push_back(get_struct(arg.first));
        else
            args.push_back(m_ptr);
    llvm::Type* llvm_return;
    if (method.return_type)
        llvm_return = get_struct(*method.return_type);
    else
        llvm_return = m_builder.getVoidTy();

    return { args, llvm_return, method.return_type };
}

void Codegen::generate_classes() {
    std::set<std::string> exists = { "Program", "Bool", "Real", "Integer", "String", "StdIO" };
    for (const Class& class_ : m_ast.classes) {
        if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty()) {
            m_definitions[stringify(class_.name)] = class_;
            if (!exists.contains(stringify(class_.name))) m_postponed.insert(class_.name);
        }
    }
    for (const Class& class_ : m_ast.classes) {
        if (m_structs.contains(stringify(class_.name))) {
            exists.insert(stringify(class_.name));
            continue;
        }
        if (stringify(class_.name) != "Program") generate_class(class_);
    }
    do {
        for (const Class& class_ : m_ast.classes)
            if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty() &&
                !exists.contains(stringify(class_.name)))
                generate_class_properties(class_);
        for (const Class& class_ : m_ast.classes)
            if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty() &&
                !exists.contains(stringify(class_.name)) && m_postponed.contains(class_.name))
                generate_class_method_declarations(class_);
        for (const Class& class_ : m_ast.classes) {
            auto status = m_class_gen_statuses[stringify(class_.name)];
            if (status.props_generated) m_postponed.erase(class_.name);
        }
    } while (!m_postponed.empty());

    for (const Class& child : m_ast.classes)
        for (const Class& parent : m_ast.classes)
            if (m_parents.contains({ stringify(child.name), stringify(parent.name) }) ||
                child == parent)
                map_child_fn_ptrs(child, parent);
    for (const Class& class_ : m_ast.classes)
        if (stringify(class_.name) != "Program" && class_.name.generic_arguments.empty() &&
            !exists.contains(stringify(class_.name)))
            generate_class_method_implementations(class_);
}

void Codegen::generate_class(Class class_) {
    std::function<void(const Class&, const std::vector<TypeName>&, std::vector<int>)> map_parents =
        [&](const Class& child, const std::vector<TypeName>& parents, std::vector<int> prev_ids) {
            size_t id = 0;
            for (const TypeName& parent : parents) {
                Class& parent_class = m_definitions[stringify(parent)];
                std::vector<int> new_ids = prev_ids;
                new_ids.push_back(id++);
                m_parents[{ stringify(child.name), stringify(parent) }] = new_ids;

                map_parents(child, parent_class.extends, new_ids);
            }
        };

    map_parents(class_, class_.extends, {});
    m_parents[{ stringify(class_.name), stringify(class_.name) }] = {};

    llvm::StructType* struct_ = llvm::StructType::create(m_context, stringify(class_.name));
    m_structs[stringify(class_.name)] = struct_;
    m_class_gen_statuses[stringify(class_.name)].struct_generated = true;
}

void Codegen::generate_class_properties(Class class_) {
    if (!parents_generated(class_)) return;

    std::vector<llvm::Type*> types {};
    std::map<std::string, std::vector<int>> ids {};

    std::function<void(const Class&, std::vector<int>)> map_props = [&](const Class& parent,
                                                                        std::vector<int> prev_ids) {
        size_t i = 0;
        for (TypeName parents_parent : parent.extends) {
            std::vector<int> prop_ids = prev_ids;
            prop_ids.push_back(i++);
            map_props(m_definitions[stringify(parents_parent)], prop_ids);
        }
        for (auto member : parent.body)
            if (auto var = std::get_if<Variable>(&member.value)) {
                std::vector<int> prop_ids = prev_ids;
                prop_ids.push_back(i++);
                m_props[stringify(class_.name)][var->name.name] = { prop_ids, var->type_name };
            }
    };

    map_props(class_, {});
    for (TypeName parent : class_.extends)
        types.push_back(get_struct(parent));
    for (auto member : class_.body)
        if (auto var = std::get_if<Variable>(&member.value))
            types.push_back(get_struct(var->type_name));

    get_struct(class_.name)->setBody(types);
    m_class_gen_statuses[stringify(class_.name)].props_generated = true;
}

void Codegen::generate_class_method_declarations(Class class_) {
    if (!parents_generated(class_)) return;

    std::function<void(const Class&, std::vector<int>)> map_methods = [&](const Class& parent,
                                                                          std::vector<int> owner) {
        size_t i = 0;
        for (TypeName parents_parent : parent.extends) {
            std::vector<int> new_owner = owner;
            new_owner.push_back(i++);
            map_methods(m_definitions[stringify(parents_parent)], new_owner);
        }
        for (auto member : parent.body)
            if (auto method = std::get_if<MemberDeclaration::Method>(&member.value)) {
                MethodSignature sign = get_signature(*method);
                FuncInfo new_def;
                for (auto new_def_ : m_functions[stringify(parent.name)][method->name.name])
                    if (new_def_.args == sign.args) {
                        new_def = new_def_;
                        break;
                    }
                new_def.owner = owner;
                bool redefinition = false;
                for (auto& old_def : m_functions[stringify(class_.name)][method->name.name])
                    if (old_def.args == sign.args) {

                        old_def = new_def;
                        redefinition = true;
                        break;
                    }
                if (!redefinition) {
                    m_functions[stringify(class_.name)][method->name.name].push_back(new_def);
                    m_functions_count[stringify(class_.name)]++;
                }
            }
    };

    size_t i = 0;
    std::vector<int> owner = {};
    for (TypeName parents_parent : class_.extends) {
        std::vector<int> new_owner = owner;
        new_owner.push_back(i++);
        map_methods(m_definitions[stringify(parents_parent)], new_owner);
    }

    for (const MemberDeclaration& member : class_.body) {
        if (auto constructor_ = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
            MemberDeclaration::Constructor constructor = *constructor_;

            std::vector<llvm::Type*> args { m_builder.getPtrTy(0) };
            for (const auto& arg : constructor.arguments)
                if (!arg.first.ptr)
                    args.push_back(get_struct(arg.first));
                else
                    args.push_back(m_ptr);

            generate_function_entry(get_struct(class_.name), class_.name, args,
                std::format("this", stringify(class_.name)), stringify(class_.name), false);
        }
        if (auto method = std::get_if<MemberDeclaration::Method>(&member.value)) {
            MethodSignature sign = get_signature(*method);

            generate_function_entry(sign.llvm_return, sign.return_type, sign.args,
                method->name.name, stringify(class_.name), false);
        }
    }
    m_class_gen_statuses[stringify(class_.name)].fn_def_generated = true;
}

void Codegen::generate_class_method_implementations(Class class_) {
    if (!parents_generated(class_)) return;
    for (const MemberDeclaration& member : class_.body) {
        if (auto constructor_ = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
            MemberDeclaration::Constructor constructor = *constructor_;

            auto variables_bak = m_variables;
            m_variables = {};

            std::vector<llvm::Type*> args { m_builder.getPtrTy(0) };
            for (const auto& arg : constructor.arguments)
                if (!arg.first.ptr)
                    args.push_back(get_struct(arg.first));
                else
                    args.push_back(m_ptr);

            llvm::Function* fn;
            for (auto overload : m_functions[stringify(class_.name)]["this"])
                if (overload.args == args) {
                    fn = overload.func;
                    break;
                }

            std::string signature {};
            for (llvm::Type* arg : args)
                if (arg->isStructTy())
                    signature += arg->getStructName().str();
                else
                    signature += "Ptr";

            llvm::BasicBlock* entry = llvm::BasicBlock::Create(
                m_context, std::format("{}_this_{}_entry", stringify(class_.name), signature), fn);
            m_builder.SetInsertPoint(entry);
            auto arg_iter = fn->arg_begin();
            m_this = { arg_iter++, get_struct(class_.name), class_.name };
            for (const auto& arg : constructor.arguments) {
                m_variables[arg.second.name] = { m_builder.CreateAlloca(get_struct(arg.first)),
                    arg.first, get_struct(arg.first) };
                m_builder.CreateStore(&*arg_iter++, m_variables[arg.second.name].ptr);
            }
            for (const Statement& stmt : constructor.body) {
                generate_statement(stmt);
            }
            m_builder.CreateRet(m_builder.CreateLoad(get_struct(class_.name), m_this->ptr));

            m_this = {};
            m_variables = variables_bak;
        }
        if (auto method_ = std::get_if<MemberDeclaration::Method>(&member.value)) {
            MemberDeclaration::Method method = *method_;

            auto variables_bak = m_variables;
            m_variables = {};

            std::vector<llvm::Type*> args { m_builder.getPtrTy(0) };
            for (const auto& arg : method.arguments)
                if (!arg.first.ptr)
                    args.push_back(get_struct(arg.first));
                else
                    args.push_back(m_ptr);
            llvm::Function* fn;
            bool owned = true;
            for (auto overload : m_functions[stringify(class_.name)][method.name.name])
                if (overload.args == args) {
                    fn = overload.func;
                    owned = overload.owner.empty();
                    break;
                }
            if (!owned) continue;

            std::string signature {};
            for (llvm::Type* arg : args)
                if (arg->isStructTy())
                    signature += arg->getStructName().str();
                else
                    signature += "Ptr";

            llvm::BasicBlock* entry = llvm::BasicBlock::Create(
                m_context, std::format("{}_{}_entry", method.name.name, signature), fn);
            m_builder.SetInsertPoint(entry);

            auto arg_iter = fn->arg_begin();
            m_this = { arg_iter++, get_struct(class_.name), class_.name };
            for (const auto& arg : method.arguments) {
                m_variables[arg.second.name] = { m_builder.CreateAlloca(get_struct(arg.first)),
                    arg.first, get_struct(arg.first) };
                m_builder.CreateStore(&*arg_iter++, m_variables[arg.second.name].ptr);
            }
            for (const Statement& stmt : method.body) {
                generate_statement(stmt);
            }

            m_this = {};
            m_variables = variables_bak;
        }
    }
    m_class_gen_statuses[stringify(class_.name)].fn_impl_generated = true;
}

llvm::Function* Codegen::generate_function_entry(llvm::Type* llvm_return,
    std::optional<TypeName> return_type, std::vector<llvm::Type*> args, std::string method_name,
    std::string struct_name, bool insert) {

    std::string signature {};
    for (llvm::Type* arg : args)
        if (arg->isStructTy())
            signature += arg->getStructName().str();
        else
            signature += "Ptr";

    auto* fn_type = llvm::FunctionType::get(llvm_return, args, false);
    auto* fn =
        llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, method_name, *m_module);
    if (insert) {
        llvm::BasicBlock* entry = llvm::BasicBlock::Create(
            m_context, std::format("{}_{}_entry", method_name, signature), fn);
        m_builder.SetInsertPoint(entry);
    }
    FuncInfo new_def = { args, fn, {}, method_name, return_type };
    bool redefinition = false;
    for (auto& old_def : m_functions[struct_name][method_name])
        if (old_def.args == new_def.args) {

            old_def = new_def;
            redefinition = true;
            break;
        }
    if (!redefinition) {
        m_functions[struct_name][method_name].push_back(new_def);
        m_functions_count[struct_name]++;
    }
    return fn;
}

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_literal(
    const Expression::Literal& literal) {
    if (literal.type == Expression::Literal::Type::Int) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["Integer"]);
        return { m_builder.CreateInsertValue(
                     val, m_builder.getInt64(std::stoi(literal.value)), { 0 }),
            integer_tn };
    }
    if (literal.type == Expression::Literal::Type::Bool) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["Bool"]);
        if (literal.value == "true")
            return { m_builder.CreateInsertValue(val, m_builder.getInt1(true), { 0 }), bool_tn };
        else
            return { m_builder.CreateInsertValue(val, m_builder.getInt1(false), { 0 }), bool_tn };
    }
    if (literal.type == Expression::Literal::Type::Str) {
        llvm::Value* val = llvm::UndefValue::get(m_structs["String"]);
        llvm::Value* str = m_builder.CreateGlobalString(literal.value);
        llvm::Value* ptr = m_builder.CreateMalloc(m_builder.getInt8Ty(), m_builder.getInt8Ty(),
            m_builder.getInt64(1), m_builder.getInt64(literal.value.size()));
        m_builder.CreateMemCpy(ptr, llvm::MaybeAlign(1), str, llvm::MaybeAlign(1),
            m_builder.getInt64(literal.value.size()));
        val = m_builder.CreateInsertValue(val, ptr, 0);
        val = m_builder.CreateInsertValue(val, m_builder.getInt64(literal.value.size()), 1);
        return { val, string_tn };
    }
    if (literal.type == Expression::Literal::Type::Real) {
        return { m_builder.CreateInsertValue(llvm::UndefValue::get(m_structs["Real"]),
                     llvm::ConstantFP::get(m_builder.getDoubleTy(), std::stof(literal.value)), 0),
            real_tn };
    }
    llvm_unreachable("generate_literal called on non-literal");
};

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_method_call(
    const Expression::MethodCall& call) {
    auto [object, type] = generate_lvalue(*call.object);

    std::vector<llvm::Value*> args = { object };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg).first);

    std::vector<llvm::Type*> arg_types = { m_builder.getPtrTy(0) };
    for (size_t i = 1; i < args.size(); i++) {
        arg_types.push_back(args[i]->getType());
    }

    llvm::Function* fn;
    std::vector<int> owner;
    std::optional<TypeName> tn;
    llvm::Value* fn_call;
    for (auto overload : m_functions[stringify(type)][call.method.name])
        if (overload.args == arg_types) {
            fn = overload.func;
            owner = overload.owner;
            tn = overload.return_type;
            break;
        }

    if (!type.ptr) {
        llvm::Type* type_ = get_struct(type);
        for (int id : owner) {
            args[0] = m_builder.CreateStructGEP(type_, args[0], id);
            type_ = type_->getStructElementType(id);
        }

        fn_call = m_builder.CreateCall(fn, args);
    } else {
        std::vector<llvm::Type*> sign_args(arg_types.begin() + 1, arg_types.end());
        uint64_t vtable_id = m_vtable_fn_ids[{ stringify(type), call.method.name, sign_args }];
        llvm::Value* ptr = m_builder.CreateLoad(m_ptr, object);
        llvm::Value* child_ptr = m_builder.CreateExtractValue(ptr, 1);
        llvm::Value* fns_ptr = m_builder.CreateExtractValue(ptr, 2);
        llvm::Value* offs_ptr = m_builder.CreateExtractValue(ptr, 3);
        llvm::Value* fn_ptr = m_builder.CreateLoad(m_builder.getPtrTy(0),
            m_builder.CreateGEP(m_builder.getPtrTy(0), fns_ptr, m_builder.getInt64(vtable_id)));
        llvm::Value* offset = m_builder.CreateLoad(m_builder.getInt64Ty(),
            m_builder.CreateGEP(m_builder.getInt64Ty(), offs_ptr, m_builder.getInt64(vtable_id)));
        child_ptr = m_builder.CreateGEP(m_builder.getInt8Ty(), child_ptr, offset);
        args[0] = child_ptr;

        llvm::Type* retTy = fn->getReturnType();
        std::vector<llvm::Type*> argTys;
        for (auto* v : args)
            argTys.push_back(v->getType());

        llvm::FunctionType* ft = llvm::FunctionType::get(retTy, argTys, false);
        llvm::FunctionCallee callee(ft, fn_ptr);

        fn_call = m_builder.CreateCall(callee, args);
    }

    return { fn_call, tn };
}

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_this_call(
    const Expression::ThisCall& call) {
    llvm::Value* object = m_this->ptr;
    llvm::Type* type = m_this->llvm_ty;

    std::vector<llvm::Value*> args = { object };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg).first);

    std::vector<llvm::Type*> arg_types = { m_builder.getPtrTy(0) };
    for (size_t i = 1; i < args.size(); i++)
        arg_types.push_back(args[i]->getType());

    llvm::Function* fn;
    std::optional<TypeName> tn;
    for (auto overload : m_functions[type->getStructName().str()][call.method.name])
        if (overload.args == arg_types) {
            fn = overload.func;
            tn = overload.return_type;
            break;
        }

    llvm::Value* fn_call = m_builder.CreateCall(fn, args);
    return { fn_call, tn };
}

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_constructor_call(
    const Expression::ConstructorCall& call) {
    TypeName type = call.type_name;

    llvm::Value* val = m_builder.CreateAlloca(get_struct(type));
    m_builder.CreateStore(llvm::UndefValue::get(get_struct(type)), val);
    std::vector<llvm::Value*> args = { val };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg).first);

    std::vector<llvm::Type*> arg_types = { m_builder.getPtrTy(0) };
    for (size_t i = 1; i < args.size(); i++)
        arg_types.push_back(args[i]->getType());

    llvm::Function* fn;
    std::optional<TypeName> tn;
    for (auto overload : m_functions[stringify(type)]["this"])
        if (overload.args == arg_types) {
            fn = overload.func;
            tn = overload.return_type;
        }

    llvm::Value* fn_call = m_builder.CreateCall(fn, args);
    return { fn_call, tn };
}

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_this_access(
    const Expression::ThisAccess& access) {
    llvm::Value* val = m_builder.CreateLoad(m_this->llvm_ty, m_this->ptr);
    auto [ids, type] = m_props[stringify(m_this->tn)][access.member.name];
    for (int id : ids)
        val = m_builder.CreateExtractValue(val, id);
    return { val, type };
}

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_member_access(
    const Expression::MemberAccess& access) {
    auto [val, val_tn] = generate_expression(*access.object);
    if (!val_tn->ptr) {
        std::vector<int> ids = m_props[stringify(*val_tn)][access.member.name].first;
        for (int id : ids)
            val = m_builder.CreateExtractValue(val, id);
    } else {
        val_tn->ptr = false;
        val = m_builder.CreateExtractValue(val, 0);
        std::vector<int> ids = m_props[stringify(*val_tn)][access.member.name].first;
        llvm::Type* type_ = get_struct(*val_tn);
        for (int id : ids) {
            val = m_builder.CreateStructGEP(type_, val, id);
            type_ = type_->getStructElementType(id);
        }
        val = m_builder.CreateLoad(type_, val);
    }
    return { val, val_tn };
}

std::pair<llvm::Value*, TypeName> Codegen::generate_deref(const Expression::Deref& deref) {
    auto [val, val_tn] = generate_expression(*deref.object);
    TypeName type = { val_tn->name, val_tn->generic_arguments, false };
    llvm::Value* parent = m_builder.CreateExtractValue(val, 0);
    parent = m_builder.CreateLoad(get_struct(type), parent);
    return { parent, type };
}

std::pair<llvm::Value*, std::optional<TypeName>> Codegen::generate_expression(
    const Expression& expr) {
    if (auto literal = std::get_if<Expression::Literal>(&expr.value))
        return generate_literal(*literal);
    if (auto ident = std::get_if<Identifier>(&expr.value))
        return { m_builder.CreateLoad(
                     m_variables[ident->name].llvm_type, m_variables[ident->name].ptr),
            m_variables[ident->name].type };
    if (auto call = std::get_if<Expression::MethodCall>(&expr.value))
        return generate_method_call(*call);
    if (auto call = std::get_if<Expression::ThisCall>(&expr.value))
        return generate_this_call(*call);
    if (auto call = std::get_if<Expression::ConstructorCall>(&expr.value))
        return generate_constructor_call(*call);
    if (auto access = std::get_if<Expression::ThisAccess>(&expr.value))
        return generate_this_access(*access);
    if (auto access = std::get_if<Expression::MemberAccess>(&expr.value))
        return generate_member_access(*access);
    if (auto deref = std::get_if<Expression::Deref>(&expr.value)) return generate_deref(*deref);

    llvm_unreachable("generate_expression called on non-expression");
}

std::pair<llvm::Value*, TypeName> Codegen::generate_lvalue(const Expression& expr) {
    if (auto ident = std::get_if<Identifier>(&expr.value)) {
        return { m_variables[ident->name].ptr, m_variables[ident->name].type };
    } else if (auto access = std::get_if<Expression::ThisAccess>(&expr.value)) {
        auto [ids, prop_tn] = m_props[stringify(m_this->tn)][access->member.name];
        llvm::Value* ptr = m_this->ptr;
        llvm::Type* type = m_this->llvm_ty;
        for (int id : ids) {
            ptr = m_builder.CreateStructGEP(type, ptr, id);
            type = type->getStructElementType(id);
        };
        return { ptr, prop_tn };
    } else if (auto access = std::get_if<Expression::MemberAccess>(&expr.value)) {
        auto [ptr, type] = generate_lvalue(*access->object);
        std::vector<int> ids = m_props[stringify(type)][access->member.name].first;
        llvm::Type* type_ = get_struct(type);
        for (int id : ids) {
            ptr = m_builder.CreateStructGEP(type_, ptr, id);
            type_ = type_->getStructElementType(id);
        };
        return { ptr, type };
    } else if (auto deref = std::get_if<Expression::Deref>(&expr.value)) {
        auto [val, type] = generate_lvalue(*deref->object);
        type.ptr = false;
        llvm::Value* parent_ptr = m_builder.CreateStructGEP(m_ptr, val, 1);
        parent_ptr = m_builder.CreateLoad(m_builder.getPtrTy(0), parent_ptr);
        return { parent_ptr, type };
    } else {
        auto [val, type] = generate_expression(expr);
        llvm::AllocaInst* ptr = m_builder.CreateAlloca(get_struct(*type));
        m_builder.CreateStore(val, ptr);
        return { ptr, *type };
    }

    llvm_unreachable("generate_lvalue called on non-lvalue expression");
}

void Codegen::generate_variable(const Variable& variable) {
    std::string name = var_name();
    llvm::Type* type = get_struct(variable.type_name);

    if (variable.value && !variable.type_name.ptr) {
        llvm::Value* alloc = m_builder.CreateAlloca(type, nullptr, name);
        llvm::Value* val = generate_expression(*variable.value).first;

        if (stringify(variable.type_name) != val->getType()->getStructName()) {
            std::vector<int> ids =
                m_parents[{ val->getType()->getStructName().str(), stringify(variable.type_name) }];
            for (int id : ids)
                val = m_builder.CreateExtractValue(val, id);
        }
        m_builder.CreateStore(val, alloc);
        m_variables[variable.name.name] = { alloc, variable.type_name, type };
    } else if (variable.value) {
        llvm::Value* alloc = generate_ptr(*variable.value, variable.type_name);
        m_variables[variable.name.name] = { alloc, variable.type_name, m_ptr };
    }
}

llvm::Value* Codegen::generate_ptr(const Expression& expr, const TypeName& tn) {
    llvm::Value* alloc;
    alloc = m_builder.CreateAlloca(m_ptr);
    llvm::Value* val;
    llvm::Type* type;
    if (m_analyzer.is_lvalue(expr)) {
        auto lvalue = generate_lvalue(expr);
        val = lvalue.first;
        type = get_struct(lvalue.second);
    } else {
        val = generate_expression(expr).first;
        type = val->getType();
        llvm::Value* ptr = m_builder.CreateAlloca(type);
        m_builder.CreateStore(val, ptr);
        val = ptr;
    }
    std::pair key = { type->getStructName().str(), stringify(tn) };
    llvm::Value* parent_ptr = val;
    llvm::Value* child_ptr = val;
    llvm::Value* fns_ptr = m_fn_arrays[key];
    llvm::Value* off_ptr = m_offset_arrays[key];

    llvm::Type* type_ = type;
    for (int id : m_parents[key]) {
        parent_ptr = m_builder.CreateStructGEP(type_, parent_ptr, id);
        type_ = type_->getStructElementType(id);
    }

    m_builder.CreateStore(parent_ptr, m_builder.CreateStructGEP(m_ptr, alloc, 0));
    m_builder.CreateStore(child_ptr, m_builder.CreateStructGEP(m_ptr, alloc, 1));
    m_builder.CreateStore(fns_ptr, m_builder.CreateStructGEP(m_ptr, alloc, 2));
    m_builder.CreateStore(off_ptr, m_builder.CreateStructGEP(m_ptr, alloc, 3));
    return alloc;
}

void Codegen::generate_assignment(const Statement::Assignment& assign) {
    auto [lval, lval_tn] = generate_lvalue(assign.left);
    llvm::Value* rval = generate_expression(assign.right).first;
    // if (lval_tn.ptr) {
    //     std::pair key = { rval->getType()->getStructName().str(), stringify(lval_tn) };
    //     llvm::Value* parent_ptr = m_builder.CreateStructGEP(m_ptr, lval, 1);
    //     parent_ptr = m_builder.CreateLoad(m_builder.getPtrTy(0), parent_ptr);
    //     llvm::Value* child_ptr = parent_ptr;
    //     llvm::Value* fns_ptr = m_fn_arrays[key];
    //     llvm::Value* off_ptr = m_offset_arrays[key];
    //
    //     llvm::Type* type_ = rval->getType();
    //     for (int id : m_parents[key]) {
    //         parent_ptr = m_builder.CreateStructGEP(type_, parent_ptr, id);
    //         type_ = type_->getStructElementType(id);
    //     }
    //     m_builder.CreateStore(rval, child_ptr);
    //     m_builder.CreateStore(parent_ptr, m_builder.CreateStructGEP(m_ptr, lval, 0));
    //     m_builder.CreateStore(fns_ptr, m_builder.CreateStructGEP(m_ptr, lval, 2));
    //     m_builder.CreateStore(off_ptr, m_builder.CreateStructGEP(m_ptr, lval, 3));
    //     return;
    // }
    if (stringify(lval_tn) != rval->getType()->getStructName()) {
        std::vector<int> ids =
            m_parents[{ rval->getType()->getStructName().str(), stringify(lval_tn) }];
        for (int id : ids)
            rval = m_builder.CreateExtractValue(rval, id);
    }
    m_builder.CreateStore(rval, generate_lvalue(assign.left).first);
}

void Codegen::generate_return(const Statement::Return& ret) {
    if (ret.value)
        m_builder.CreateRet(generate_expression(*ret.value).first);
    else
        m_builder.CreateRetVoid();
}

void Codegen::generate_if(const Statement::If& if_) {
    llvm::Function* fn = m_builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* then = llvm::BasicBlock::Create(m_context, var_name(), fn);
    llvm::BasicBlock* else_ = llvm::BasicBlock::Create(m_context, var_name(), fn);
    llvm::BasicBlock* merge = llvm::BasicBlock::Create(m_context, var_name(), fn);

    llvm::Value* cond = generate_expression(if_.condition).first;
    m_builder.CreateCondBr(m_builder.CreateExtractValue(cond, 0), then, else_);

    m_builder.SetInsertPoint(then);
    for (const Statement& stmt : if_.body)
        generate_statement(stmt);
    m_builder.CreateBr(merge);

    m_builder.SetInsertPoint(else_);
    if (if_.elifs.empty())
        for (const Statement& stmt : if_.else_body)
            generate_statement(stmt);
    else {
        Expression condition = if_.elifs[0].condition;
        std::vector<Statement> body = if_.elifs[0].body;
        std::vector<Statement::If::ElIf> elifs = if_.elifs;
        elifs.erase(elifs.begin());
        Statement::If reduced = { condition, body, elifs, if_.else_body };
        generate_if(reduced);
    };
    m_builder.CreateBr(merge);

    m_builder.SetInsertPoint(merge);
}

void Codegen::generate_while(const Statement::While& while_) {
    llvm::Function* fn = m_builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* do_ = llvm::BasicBlock::Create(m_context, var_name(), fn);
    llvm::BasicBlock* merge = llvm::BasicBlock::Create(m_context, var_name(), fn);

    llvm::Value* cond = generate_expression(while_.condition).first;
    m_builder.CreateCondBr(m_builder.CreateExtractValue(cond, 0), do_, merge);

    m_builder.SetInsertPoint(do_);
    for (const Statement& stmt : while_.body)
        generate_statement(stmt);
    cond = generate_expression(while_.condition).first;
    m_builder.CreateCondBr(m_builder.CreateExtractValue(cond, 0), do_, merge);

    m_builder.SetInsertPoint(merge);
}

void Codegen::generate_super_call(const Statement::SuperCall& call) {
    TypeName type;
    if (call.parent)
        type = *call.parent;
    else
        type = m_definitions[stringify(m_this->tn)].extends[0];

    llvm::Value* val = m_this->ptr;
    llvm::Type* val_ty = m_this->llvm_ty;
    std::vector<int> ids = m_parents[{ stringify(m_this->tn), stringify(type) }];
    for (int id : ids) {
        val = m_builder.CreateStructGEP(val_ty, val, id);
        val_ty = val_ty->getStructElementType(id);
    }

    std::vector<llvm::Value*> args = { val };
    for (const Expression& arg : call.arguments)
        args.push_back(generate_expression(arg).first);

    std::vector<llvm::Type*> arg_types = { m_builder.getPtrTy(0) };
    for (size_t i = 1; i < args.size(); i++)
        arg_types.push_back(args[i]->getType());

    llvm::Function* fn;
    for (auto overload : m_functions[stringify(type)]["this"])
        if (overload.args == arg_types) {
            fn = overload.func;
        }

    m_builder.CreateCall(fn, args);
}

void Codegen::generate_statement(const Statement& stmt) {
    if (auto var = std::get_if<Variable>(&stmt.value)) generate_variable(*var);
    if (auto expr = std::get_if<Expression>(&stmt.value)) generate_expression(*expr);
    if (auto assign = std::get_if<Statement::Assignment>(&stmt.value)) generate_assignment(*assign);
    if (auto ret = std::get_if<Statement::Return>(&stmt.value)) generate_return(*ret);
    if (auto if_ = std::get_if<Statement::If>(&stmt.value)) generate_if(*if_);
    if (auto while_ = std::get_if<Statement::While>(&stmt.value)) generate_while(*while_);
    if (auto call = std::get_if<Statement::SuperCall>(&stmt.value)) generate_super_call(*call);
}

void Codegen::map_child_fn_ptrs(const Class& from, const Class& to) {
    std::vector<FuncInfo> fns = {};
    std::string from_name = stringify(from.name);
    std::string to_name = stringify(to.name);

    uint64_t i = 0;
    for (auto& overloads : m_functions[to_name])
        for (FuncInfo& overload : overloads.second) {
            fns.push_back(overload);
            std::vector<llvm::Type*> args(overload.args.begin() + 1, overload.args.end());
            GlobalFnKey key { to_name, overload.name, args };
            m_vtable_fn_ids[key] = i++;
        }

    for (FuncInfo& parent_fn : fns)
        for (auto& overloads : m_functions[from_name])
            for (FuncInfo& child_fn : overloads.second)
                if (parent_fn.args == child_fn.args) parent_fn = child_fn;

    std::vector<llvm::Constant*> funcs_init;
    std::vector<llvm::Constant*> offset_init;

    for (FuncInfo& fn : fns) {
        uint64_t offset = 0;
        const llvm::StructLayout* layout = m_dl.getStructLayout(m_structs[from_name]);
        Class class_ = m_definitions[from_name];
        for (int id : fn.owner) {
            offset += layout->getElementOffset(id);
            class_ = m_definitions[stringify(class_.extends[id])];
            layout = m_dl.getStructLayout(get_struct(class_.name));
        }

        funcs_init.push_back(fn.func);
        offset_init.push_back(llvm::ConstantInt::get(m_builder.getInt64Ty(), offset));
    }

    llvm::ArrayType* offset_arr = llvm::ArrayType::get(m_builder.getInt64Ty(), fns.size());
    llvm::ArrayType* fn_arr = llvm::ArrayType::get(m_builder.getPtrTy(), fns.size());
    llvm::GlobalVariable* offset_gv =
        new llvm::GlobalVariable(offset_arr, false, llvm::GlobalVariable::PrivateLinkage);
    llvm::GlobalVariable* fn_gv =
        new llvm::GlobalVariable(fn_arr, false, llvm::GlobalVariable::PrivateLinkage);

    offset_gv->setInitializer(llvm::ConstantArray::get(offset_arr, offset_init));
    fn_gv->setInitializer(llvm::ConstantArray::get(fn_arr, funcs_init));
    m_module->insertGlobalVariable(offset_gv);
    m_module->insertGlobalVariable(fn_gv);

    std::pair<std::string, std::string> key = { from_name, to_name };
    m_offset_arrays[key] = offset_gv;
    m_fn_arrays[key] = fn_gv;
};

void Codegen::generate() {
    generate_integer();
    generate_real();
    generate_bool();
    generate_stdio();
    generate_string();
    generate_ptr();

    generate_stdio_methods();
    generate_integer_methods();
    generate_real_methods();
    generate_string_methods();
    generate_bool_methods();

    generate_classes();

    for (const Class& class_ : m_ast.classes)
        if (stringify(class_.name) == "Program")
            for (const MemberDeclaration& member : class_.body)
                if (auto constructor = std::get_if<MemberDeclaration::Constructor>(&member.value)) {
                    llvm::FunctionType* func_type =
                        llvm::FunctionType::get(m_builder.getVoidTy(), false);

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

    // m_module->print(llvm::outs(), nullptr);
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
    m_dl = llvm::DataLayout(m_module->getDataLayout());
};
