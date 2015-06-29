#ifndef __CALLVM_CODEGEN_H_
#define __CALLVM_CODEGEN_H_

#include "ast.hpp"

#include <boost/variant/static_visitor.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include <string>

namespace callvm {
namespace codegen {

class llvm_codegenerator final : public boost::static_visitor<> {
   public:
    llvm_codegenerator(std::string const& name, llvm::LLVMContext& ctx)
        : module(name, ctx), builder(ctx) {}
    llvm::Module& get_module() { return module; }
    llvm::IRBuilder<>& get_builder() { return builder; }

    void operator()(ast::int_expr const&);
    void operator()(ast::double_expr const&);
    void operator()(ast::binop_expr const&);

   private:
    llvm::Module module;
    llvm::IRBuilder<> builder;
};

}  // namespace codegen
}  // namespace callvm

#endif
