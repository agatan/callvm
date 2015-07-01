#ifndef __CALLVM_TYPE_H_
#define __CALLVM_TYPE_H_

#include <boost/variant/variant.hpp>

#include <memory>

namespace callvm {
namespace semantics {
namespace type {

enum class primitive_type { Int, Float };

class unknow_type;

using any_type = boost::variant<primitive_type, unknow_type>;

class unknow_type {
   public:
    unknow_type() : inner_val(nullptr) {}
    unknow_type(any_type const& ty)
        : inner_val(std::make_shared<any_type>(ty)) {}

   private:
    std::shared_ptr<any_type> inner_val;
};

}  // namespace type
}  // namespace semantics
}  // namespace callvm

#endif
