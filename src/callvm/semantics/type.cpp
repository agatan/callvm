#include "type.hpp"
#include <boost/variant/static_visitor.hpp>

namespace callvm {
namespace semantics {
namespace type {

    template<typename T>
    bool is_equal(T const& lhs, any_type const& rhs) {
        return boost::apply_visitor(equal(), lhs, rhs);
    }

}  // namespace type
}  // namespace semantics
}  // namespace callvm
