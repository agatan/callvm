#ifndef __CALLVM_CODEGEN_H_
#define __CALLVM_CODEGEN_H_

#include "ast.hpp"

#include <boost/variant/static_visitor.hpp>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include <string>

namespace callvm {
namespace codegen {

class llvm_codegenerator final : public boost::static_visitor<llvm::Value*> {
   public:
    llvm_codegenerator(std::string const&, llvm::LLVMContext&);
    bool generate(ast::any_expr const&, std::string const&);
    llvm::Module* get_module() { return &module; }
    llvm::IRBuilder<>& get_builder() { return builder; }

    llvm::Value* operator()(ast::int_expr const&);
    llvm::Value* operator()(ast::double_expr const&);
    llvm::Value* operator()(ast::binop_expr const&);

   private:
    llvm::LLVMContext& ctx;
    llvm::Module module;
    llvm::IRBuilder<> builder;
    llvm::Function* f;
};

}  // namespace codegen
}  // namespace callvm

#endif
