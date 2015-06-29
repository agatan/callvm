#include "lib.h"
#include "ast.hpp"
#include "parser.hpp"
#include "codegen.h"

#include <boost/variant/apply_visitor.hpp>

#include <string>

namespace callvm {
std::string parse_and_stringize(std::string const& src) {
    parser::callvm_parser parser;
    auto ast = parser.parse(src);
    return ast::visitor::stringize(ast);
}

void parse_and_codegen(std::string const& src,
                       codegen::llvm_codegenerator& gen) {
    parser::callvm_parser parser;
    auto ast = parser.parse(src);
    boost::apply_visitor(gen, ast);
}
}  // namespace callvm
