#ifndef __CALLVM_PARSER_H_
#define __CALLVM_PARSER_H_

#include <string>
#include <memory>

#include "ast.hpp"

namespace callvm {
namespace parser {
class callvm_parser final {
   public:
    ast::any_expr parse(std::string const& src) const;
};
}  // namespace parser
}  // namespace callvm

#endif
