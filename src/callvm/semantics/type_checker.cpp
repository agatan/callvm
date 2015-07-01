#include "type_checker.h"

namespace callvm {
namespace semantics {
namespace type {

void type_checker::operator()(ast::binop_expr& ast) {
    type_inferer i;
    boost::apply_visitor(i, ast.get_lhs());
    boost::apply_visitor(i, ast.get_rhs());
    return;
}

any_type type_inferer::operator()(ast::int_expr&) {
    return any_type{primitive_type::Int};
}

any_type type_inferer::operator()(ast::double_expr&) {
    return any_type{primitive_type::Float};
}

any_type type_inferer::operator()(ast::binop_expr& ast) {
    auto l_type = boost::apply_visitor(*this, ast.get_lhs());
    auto r_type = boost::apply_visitor(*this, ast.get_rhs());
    if (is_equal(primitive_type::Float, l_type) ||
        is_equal(primitive_type::Float, r_type)) {
        // even if one of operands is of type float, binop type should be float.
        ast.set_type(primitive_type::Float);
        return any_type{primitive_type::Float};
    }
    ast.set_type(primitive_type::Int);
    return any_type{primitive_type::Int};
}

void type_check(ast::any_expr& ast) {
    type_checker t;
    boost::apply_visitor(t, ast);
    type_inferer i;
    boost::apply_visitor(i, ast);
}

}  // namespace type
}  // namespace semantics
}  // namespace callvm
