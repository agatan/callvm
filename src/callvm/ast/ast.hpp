#ifndef __CALLVM_AST_HPP_
#define __CALLVM_AST_HPP_

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/static_visitor.hpp>

#include <string>
#include <memory>

#include "ast_base.hpp"
#include "../semantics/type.hpp"

namespace callvm {
namespace ast {

class int_expr;
class float_expr;
class binop_expr;

using any_expr =
    boost::variant<int_expr, float_expr, boost::recursive_wrapper<binop_expr>>;

class expr_base : public ast_base {
   public:
    semantics::type::any_type const& get_type() const { return *type; }
    void set_type(semantics::type::any_type const& ty) { type = std::make_shared<semantics::type::any_type>(ty); }

    template<typename T>
    expr_base(T const&& ty) : type(std::make_shared<semantics::type::any_type>(ty)) {}
    expr_base() : type(std::make_shared<semantics::type::any_type>(semantics::type::unknow_type())) {}

   private:
    std::shared_ptr<semantics::type::any_type> type;
};

class int_expr : public expr_base {
   private:
    int val;

   public:
    int_expr() : val(0) {}
    int_expr(int v)
        : expr_base(semantics::type::primitive_type(
              semantics::type::primitive_type::Int)),
          val(v) {}
    int get_val() const { return val; }
};

class float_expr : public expr_base {
   private:
    float val;

   public:
    float_expr() : val(0) {}
    float_expr(float x)
        : expr_base(semantics::type::primitive_type(
              semantics::type::primitive_type::Float)),
          val(x) {}
    float get_val() const { return val; }
};

class binop_expr : public expr_base {
   private:
    std::string op;
    any_expr lhs;
    any_expr rhs;

   public:
    binop_expr(std::string const& op, any_expr& lhs, any_expr& rhs)
        : expr_base(semantics::type::unknow_type()),
          op(op),
          lhs(lhs),
          rhs(rhs) {}
    std::string const& get_op() const { return op; }
    any_expr const& get_lhs() const { return lhs; }
    any_expr& get_lhs() { return lhs; }
    any_expr const& get_rhs() const { return rhs; }
    any_expr& get_rhs() { return rhs; }
};

namespace visitor {
class stringizer : public boost::static_visitor<std::string> {
   public:
    std::string operator()(int_expr const&) const;
    std::string operator()(float_expr const&) const;
    std::string operator()(binop_expr const&) const;
};

std::string stringize(ast::any_expr const);
}  // namespace visitor

}  // namespace ast
}  // namespace callvm
#endif
