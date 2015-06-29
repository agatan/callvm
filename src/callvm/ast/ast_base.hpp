#ifndef __CALLVM_AST_BASE_HPP
#define __CALLVM_AST_BASE_HPP

namespace callvm {
namespace ast {

class ast_base {
   public:
    int line;
    int col;
    int length;
};

}  // namespace ast
}  // namespace callvm
#endif
