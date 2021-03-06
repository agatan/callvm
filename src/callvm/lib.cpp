#include "lib.h"
#include "ast/ast.hpp"
#include "parser.hpp"
#include "codegen.h"
#include "semantics/type_checker.h"

#include <boost/variant/apply_visitor.hpp>

#include <string>
#include <memory>
#include <iostream>

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
    if(!gen.generate(ast, "func")) {
        std::cerr << "null 6" << std::endl;
    }
}

ast::any_expr parse(std::string const& src) {
    parser::callvm_parser parser;
    return parser.parse(src);
}

}  // namespace callvm
