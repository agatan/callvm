#ifndef __CALLVM_TYPE_CHECKER_H_
#define __CALLVM_TYPE_CHECKER_H_

#include "type.hpp"
#include "../ast/ast.hpp"

#include <boost/variant/static_visitor.hpp>
#include <boost/optional.hpp>

namespace callvm {
namespace semantics {
namespace type {

class type_check_error {
   public:
    ast::any_expr& lhs;
    ast::any_expr& rhs;
};

class type_checker : public boost::static_visitor<> {
   public:
    void operator()(ast::int_expr&) {}
    void operator()(ast::double_expr&) {}
    void operator()(ast::binop_expr&);
};

class type_inferer : public boost::static_visitor<any_type> {
   public:
    any_type operator()(ast::int_expr& i);
    any_type operator()(ast::double_expr& x);
    any_type operator()(ast::binop_expr&);
};

void type_check(ast::any_expr& ast);

}  // namespace type
}  // namespace semantics
}  // namespace callvm

#endif
