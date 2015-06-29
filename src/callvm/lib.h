#ifndef __LILIS_LIB_H_
#define __LILIS_LIB_H_

#include <string>
#include "codegen.h"

namespace callvm {
std::string parse_and_stringize(std::string const&);
void parse_and_codegen(std::string const&, codegen::llvm_codegenerator&);
}  // namespace callvm

#endif
