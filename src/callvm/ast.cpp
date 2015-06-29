#include "ast.hpp"

#include <boost/variant/apply_visitor.hpp>

#include <string>

namespace callvm {
namespace ast {
namespace visitor {
    std::string stringizer::operator()(int_expr const& i) const {
        return std::to_string(i.get_val());
    }

    std::string stringizer::operator()(double_expr const& x) const {
        return std::to_string(x.get_val());
    }

    std::string stringizer::operator()(binop_expr const& bin) const {
        return "(" + boost::apply_visitor(stringizer(), bin.get_lhs()) +
            " " + boost::apply_visitor(stringizer(), bin.get_rhs()) +
            ")";
    }
}  // namespace visitor
}  // namespace ast
}  // namespace callvm
