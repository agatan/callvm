#include "parser.hpp"
#include "ast.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

#include <string>

namespace callvm {
namespace parser {

// Helper {{{
namespace helper {
template <typename Iter>
inline auto line_pos_iterator(Iter i) {
    return boost::spirit::line_pos_iterator<Iter>(i);
}
}  // namespace helper
// }}}

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

using namespace qi::labels;

template <class Iter>
class callvm_grammar
    : public qi::grammar<Iter, ast::any_expr(), ascii::space_type> {
   public:
    callvm_grammar() : callvm_grammar::base_type(expression) {
        int_expr = qi::int_[_val = phx::construct<ast::int_expr>(_1)];
        double_expr =
            qi::real_parser<double, qi::strict_real_policies<double>>()
                [_val = phx::construct<ast::double_expr>(_1)];
        primary %=
            double_expr | int_expr | ('(' >> expression >> ')')[_val = _1];
        mul_expr =
            primary[_val = _1] >>
            *(('*' >>
               primary)[_val = phx::construct<ast::binop_expr>("*", _val, _1)] |
              ('/' >>
               primary)[_val = phx::construct<ast::binop_expr>("/", _val, _1)]);
        add_expr =
            mul_expr[_val = _1] >>
            *(('+' >>
               primary)[_val = phx::construct<ast::binop_expr>("+", _val, _1)] |
              ('-' >>
               primary)[_val = phx::construct<ast::binop_expr>("-", _val, _1)]);
        expression %= add_expr;
    }

   private:
    template <class T>
    using rule = qi::rule<Iter, T, ascii::space_type>;

    rule<ast::int_expr()> int_expr;
    rule<ast::double_expr()> double_expr;
    rule<ast::any_expr()> primary;
    rule<ast::any_expr()> mul_expr;
    rule<ast::any_expr()> add_expr;
    rule<ast::any_expr()> expression;
};

ast::any_expr callvm_parser::parse(std::string const &src) const {
    auto begin = helper::line_pos_iterator(std::begin(src));
    using iterator_t = decltype(begin);
    auto end = helper::line_pos_iterator(std::end(src));
    callvm_grammar<iterator_t> grammar;
    ast::any_expr result;

    if (!qi::phrase_parse(begin, end, grammar, ascii::space, result) ||
        begin != end) {
        std::cerr << "error: " << std::string(begin, end) << std::endl;
        throw "syntax error";
    }
    return result;
}

}  // namespace parser
}  // namespace callvm
