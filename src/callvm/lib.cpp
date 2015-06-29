#include "lib.h"
#include "ast.hpp"
#include "parser.hpp"

#include <string>

namespace callvm {
    std::string parse_and_stringize(std::string const& src) {
        parser::callvm_parser parser;
        auto ast = parser.parse(src);
        return ast::visitor::stringize(ast);
    }
} // namespace callvm
