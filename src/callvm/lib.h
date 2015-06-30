#ifndef __LILIS_LIB_H_
#define __LILIS_LIB_H_

#include <string>
#include "codegen.h"
#include "semantics/type_checker.h"

namespace callvm {
std::string parse_and_stringize(std::string const&);
void parse_and_codegen(std::string const&, codegen::llvm_codegenerator&);
ast::any_expr parse(std::string const&);


}  // namespace callvm

#endif
