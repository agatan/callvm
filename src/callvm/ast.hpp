#ifndef __CALLVM_AST_HPP_
#define __CALLVM_AST_HPP_

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/static_visitor.hpp>

#include <string>
#include <memory>

namespace callvm {
namespace ast {

class int_expr;
class double_expr;
class binop_expr;

using any_expr =
    boost::variant<int_expr, double_expr, boost::recursive_wrapper<binop_expr>>;

class int_expr {
   private:
    int val;

   public:
    int_expr() : val(0) {}
    int_expr(int v) : val(v) {}
    int get_val() const { return val; }
};

class double_expr {
   private:
    double val;

   public:
    double_expr() : val(0) {}
    double_expr(float x) : val(x) {}
    double get_val() const { return val; }
};

class binop_expr {
   private:
    std::string op;
    any_expr lhs;
    any_expr rhs;

   public:
    binop_expr(std::string const& op, any_expr lhs, any_expr rhs)
        : op(op), lhs(lhs), rhs(rhs) {}
    std::string const& get_op() const { return op; }
    any_expr const& get_lhs() const { return lhs; }
    any_expr const& get_rhs() const { return rhs; }
};

namespace visitor {
class stringizer : public boost::static_visitor<std::string> {
   public:
    std::string operator()(int_expr const&) const;
    std::string operator()(double_expr const&) const;
    std::string operator()(binop_expr const&) const;
};

std::string stringize(ast::any_expr const&);
}  // namespace visitor

}  // namespace ast
}  // namespace callvm
#endif
