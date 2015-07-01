#ifndef __CALLVM_TYPE_CHECKER_H_
#define __CALLVM_TYPE_CHECKER_H_

#include "type.hpp"
#include "../ast/ast.hpp"

#include <boost/variant/static_visitor.hpp>
#include <boost/optional.hpp>

namespace callvm {
namespace semantics {
namespace type {

class type_inferer : public boost::static_visitor<primitive_type> {
   public:
    primitive_type operator()(ast::int_expr& i);
    primitive_type operator()(ast::float_expr& x);
    primitive_type operator()(ast::binop_expr&);
};

primitive_type infer_expression_type(ast::any_expr& ast);

}  // namespace type
}  // namespace semantics
}  // namespace callvm

#endif
