#include "type_checker.h"

namespace callvm {
namespace semantics {
namespace type {

primitive_type type_inferer::operator()(ast::int_expr&) {
    return primitive_type::Int;
}

primitive_type type_inferer::operator()(ast::float_expr&) {
    return primitive_type::Float;
}

primitive_type type_inferer::operator()(ast::binop_expr& ast) {
    auto l_type = boost::apply_visitor(*this, ast.get_lhs());
    auto r_type = boost::apply_visitor(*this, ast.get_rhs());
    if (primitive_type::Float == l_type ||
        primitive_type::Float == r_type) {
        // even if one of operands is of type float, binop type should be float.
        ast.set_type(primitive_type::Float);
        return primitive_type::Float;
    }
    ast.set_type(primitive_type::Int);
    return primitive_type::Int;
}

primitive_type infer_expression_type(ast::any_expr& ast) {
    type_inferer i;
    return boost::apply_visitor(i, ast);
}

}  // namespace type
}  // namespace semantics
}  // namespace callvm
