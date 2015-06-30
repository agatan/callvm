#include "codegen.h"
#include "ast/ast.hpp"

#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <boost/variant/apply_visitor.hpp>

#include <string>

namespace callvm {
namespace codegen {

llvm_codegenerator::llvm_codegenerator(std::string const& mod_name,
                                       llvm::LLVMContext& ctx)
    : ctx(ctx), module(mod_name, ctx), builder(ctx) {}

bool llvm_codegenerator::generate(ast::any_expr const& ast,
                                  std::string const& func_name) {
    llvm::FunctionType* ft = llvm::FunctionType::get(
        llvm::Type::getVoidTy(ctx), llvm::ArrayRef<llvm::Type*>(), false);
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
    return true;
}

llvm::Value* llvm_codegenerator::operator()(ast::int_expr const& i) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), i.get_val());
}

llvm::Value* llvm_codegenerator::operator()(ast::double_expr const& x) {
    return llvm::ConstantFP::get(ctx, llvm::APFloat(x.get_val()));
}

llvm::Value* llvm_codegenerator::operator()(ast::binop_expr const& binop) {
    auto lhs = boost::apply_visitor(*this, *binop.get_lhs());
    if (!lhs) {
        std::cerr << "null 4" << std::endl;
    }
    auto rhs = boost::apply_visitor(*this, *binop.get_rhs());
    if (!rhs) {
        std::cerr << "null 5" << std::endl;
    }
    if (binop.get_op() == "+") {
        return builder.CreateAdd(lhs, rhs, "addtmp");
    } else if (binop.get_op() == "-") {
        return builder.CreateSub(lhs, rhs, "subtmp");
    } else if (binop.get_op() == "*") {
        return builder.CreateMul(lhs, rhs, "multmp");
    } else if (binop.get_op() == "/") {
        return builder.CreateSDiv(lhs, rhs, "divtmp");
    }
    return nullptr;
}

}  // namespace codegen
}  // namespace callvm
