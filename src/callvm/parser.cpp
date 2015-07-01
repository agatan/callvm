#include "parser.hpp"
#include "ast/ast.hpp"

#define BOOST_SPIRIT_USE_PHOENIX_V3

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/fusion/include/vector.hpp>

#include <string>
#include <type_traits>
#include <memory>

namespace callvm {
namespace parser {

// Helper {{{
namespace helper {
template <typename Iter>
inline auto line_pos_iterator(Iter i) {
    return boost::spirit::line_pos_iterator<Iter>(i);
}

template <class T, class Iter>
void set_position_impl(T &node, Iter const before, Iter const after,
                       Iter const src) {
    auto d = std::distance(before.base(), after.base());
    node.line = boost::spirit::get_line(before);
    node.col = boost::spirit::get_column(src, before);
    node.length = d < 0 ? 0 : d;
}

template <class Iter>
struct set_position : public boost::static_visitor<> {
    Iter const before;
    Iter const after;
    Iter const src;

    set_position(Iter const before, Iter const after, Iter const src)
        : before(before), after(after), src(src) {}

    template <class T>
    void operator()(T &node) const {
        set_position_impl(node, before, after, src);
    }
};

template <typename Iter>
struct annotation_f {
    typedef void result_type;
    annotation_f(Iter const src) : src(src) {}

    template <typename Val, typename First, typename Last>
    void operator()(Val &v, First f, Last l) const {
        do_annotate(v, f, l);
    }

   private:
    Iter const src;

    void do_annotate(ast::any_expr &ast, Iter f, Iter l) const {
        set_position<Iter> s(f, l, src);
        boost::apply_visitor(s, ast);
    }

    template <typename T>
    void do_annotate(std::shared_ptr<T> &ast, Iter f, Iter l) const {
        do_annotate(*ast);
    }

    template <typename T>
    void do_annotate(T &ast, Iter f, Iter l) const {
        set_position_impl(ast, f, l, src);
    }
};

}  // namespace helper
// }}}

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

using namespace qi::labels;

template <class Iter>
class callvm_grammar
    : public qi::grammar<Iter, ast::any_expr(), ascii::space_type> {
    template <typename T>
    struct make_shared_ptr_lazy {
        typedef std::shared_ptr<T> result_type;

        template <typename... Args>
        auto operator()(Args &&... args) const -> result_type {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };

    template <typename T, typename... Args>
    auto make_shared_ptr(Args &&... args) {
        return phx::function<make_shared_ptr_lazy<T>>()(
            std::forward<Args>(args)...);
    }

   public:
    callvm_grammar(Iter const src)
        : callvm_grammar::base_type(expression), annotate(src) {
        int_expr = qi::int_[_val = phx::construct<ast::int_expr>(_1)];

        float_expr =
            qi::real_parser<float, qi::strict_real_policies<float>>()
                [_val = phx::construct<ast::float_expr>(_1)];

        primary_expr %= float_expr | int_expr | '(' >> expression >> ')';

        mul_expr = primary_expr[_val = _1] >>
                   *((qi::string("*") | qi::string("/")) >>
                     primary_expr)[_val = phx::construct<ast::binop_expr>(
                                       _1, _val, _2)];

        add_expr =
            mul_expr[_val = _1] >>
            *((qi::string("+") | qi::string("-")) >>
              mul_expr)[_val = phx::construct<ast::binop_expr>(_1, _val, _2)];

        expression %= add_expr;

        qi::on_success(int_expr, annotate(_val, _1, _3));
        qi::on_success(float_expr, annotate(_val, _1, _3));
        qi::on_success(primary_expr, annotate(_val, _1, _3));
        qi::on_success(mul_expr, annotate(_val, _1, _3));
        qi::on_success(add_expr, annotate(_val, _1, _3));
        qi::on_success(expression, annotate(_val, _1, _3));
    }

   private:
    template <class T>
    using rule = qi::rule<Iter, T, ascii::space_type>;

    phx::function<helper::annotation_f<Iter>> annotate;

    rule<ast::int_expr()> int_expr;
    rule<ast::float_expr()> float_expr;
    rule<ast::any_expr()> primary_expr;
    rule<ast::any_expr()> mul_expr;
    rule<ast::any_expr()> add_expr;
    rule<ast::any_expr()> expression;
};

ast::any_expr callvm_parser::parse(std::string const &src) const {
    auto begin = helper::line_pos_iterator(std::begin(src));
    using iterator_t = decltype(begin);
    auto end = helper::line_pos_iterator(std::end(src));
    callvm_grammar<iterator_t> grammar(begin);
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
