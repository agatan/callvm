#ifndef __CALLVM_CODEGEN_H_
#define __CALLVM_CODEGEN_H_

#include "ast.hpp"

#include <boost/variant/static_visitor.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

namespace callvm {
namespace codegen {

class llvm_codegenerator final : public boost::static_visitor<> {
   public:
   private:
    llvm::Module module;
};

}  // namespace codegen
}  // namespace callvm

#endif
