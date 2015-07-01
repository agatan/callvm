#include "codegen.h"
#include "ast/ast.hpp"
#include "semantics/type_checker.h"

#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_os_ostream.h>
#include <boost/variant/apply_visitor.hpp>

#include <string>
#include <iostream>

namespace callvm {
namespace codegen {

llvm_codegenerator::llvm_codegenerator(std::string const& mod_name,
                                       llvm::LLVMContext& ctx)
    : ctx(ctx), module(mod_name, ctx), builder(ctx) {}

bool llvm_codegenerator::generate(ast::any_expr& ast,
                                  std::string const& func_name) {
    semantics::type::primitive_type expr_type =
        semantics::type::infer_expression_type(ast);
    llvm::FunctionType* ft;
    if (expr_type == semantics::type::primitive_type::Int) {
        std::cout << "this is an integer expression" << std::endl;
        ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx),
                                     llvm::ArrayRef<llvm::Type*>(), false);
    } else {
        std::cout << "this is an float expression" << std::endl;
        ft = llvm::FunctionType::get(llvm::Type::getFloatTy(ctx),
                                     llvm::ArrayRef<llvm::Type*>(), false);
    }
    if (!ft) {
        std::cerr << "null 1" << std::endl;
    }
    f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, func_name,
                               &module);
    if (!f) {
        std::cerr << "null 2" << std::endl;
    }

    llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, "entry", f);
    if (!bb) {
        std::cerr << "null 3" << std::endl;
    }
    builder.SetInsertPoint(bb);
    auto retval = boost::apply_visitor(*this, ast);
    if (!retval) return false;
    builder.CreateRet(retval);

    llvm::raw_os_ostream s(std::cerr);
    if (llvm::verifyFunction(*f, &s)) {
        std::cerr << "something goes wrong in function" << std::endl;
        return false;
    }
    return true;
}

llvm::Value* llvm_codegenerator::operator()(ast::int_expr const& i) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), i.get_val());
}

llvm::Value* llvm_codegenerator::operator()(ast::float_expr const& x) {
    return llvm::ConstantFP::get(ctx, llvm::APFloat(static_cast<float>(x.get_val())));
}

llvm::Value* llvm_codegenerator::operator()(ast::binop_expr const& binop) {
    auto lhs = boost::apply_visitor(*this, binop.get_lhs());
    auto rhs = boost::apply_visitor(*this, binop.get_rhs());
    bool is_float_expr =
        lhs->getType()->isFloatTy() || rhs->getType()->isFloatTy();

    if (is_float_expr && lhs->getType()->isIntegerTy()) {
        lhs = builder.CreateSIToFP(lhs, llvm::Type::getFloatTy(ctx), "casttmp");
    }
    if (is_float_expr && rhs->getType()->isIntegerTy()) {
        rhs = builder.CreateSIToFP(rhs, llvm::Type::getFloatTy(ctx), "casttmp");
    }

    if (!is_float_expr) {
        if (binop.get_op() == "+") {
            return builder.CreateAdd(lhs, rhs, "addtmp");
        } else if (binop.get_op() == "-") {
            return builder.CreateSub(lhs, rhs, "subtmp");
        } else if (binop.get_op() == "*") {
            return builder.CreateMul(lhs, rhs, "multmp");
        } else if (binop.get_op() == "/") {
            return builder.CreateSDiv(lhs, rhs, "divtmp");
        }
    } else {
        if (binop.get_op() == "+") {
            return builder.CreateFAdd(lhs, rhs, "addtmp");
        } else if (binop.get_op() == "-") {
            return builder.CreateFSub(lhs, rhs, "subtmp");
        } else if (binop.get_op() == "*") {
            return builder.CreateFMul(lhs, rhs, "multmp");
        } else if (binop.get_op() == "/") {
            return builder.CreateFDiv(lhs, rhs, "divtmp");
        }
    }
    return nullptr;
}

}  // namespace codegen
}  // namespace callvm
