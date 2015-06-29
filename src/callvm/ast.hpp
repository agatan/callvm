#ifndef __CALLVM_AST_HPP_
#define __CALLVM_AST_HPP_

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <string>
#include <memory>

class int_expr;
class float_expr;
class binop_expr;

using any_expr = boost::variant<int_expr, float_expr,
                       boost::recursive_wrapper<binop_expr>>;

class int_expr {
   private:
    int val;

   public:
    int_expr(int v) : val(v) {}
    int get_val() const { return val; }
};

class float_expr {
   private:
    float val;

   public:
    float_expr(float x) : val(x) {}
    float get_val() const { return val; }
};

class binop_expr {
   private:
    std::string op;
    any_expr lhs;
    any_expr rhs;

   public:
    binop_expr(std::string const& op, any_expr lhs, any_expr rhs)
        : op(op), lhs(lhs), rhs(rhs) {}
};

#endif
