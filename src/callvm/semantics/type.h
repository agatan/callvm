#ifndef __CALLVM_TYPE_H_
#define __CALLVM_TYPE_H_

#include <boost/variant/variant.hpp>

namespace callvm {
namespace semantics {
namespace type {

enum class primitive_type_id { Int, Float };

class primitive_type;
class unknow_type;

using any_type = boost::variant<primitive_type, unknow_type>;

class primitive_type {
   public:
    primitive_type(primitive_type_id id) : id(id) {}
    primitive_type get_primitive_type_id() const { return id; }
    primitive_type(): id(primitive_type_id::Int) {}

   private:
    primitive_type_id id;
};

class unknow_type {
   public:
    static unknow_type create() {
        return unknow_type(next_id++);
    }
    static void reset() { unknow_type::next_id = 0; }
    int get_id() const { return id; }
    unknow_type(): id(0) {}

   private:
    static int next_id;
    unknow_type(int id) : id(id) {}
    int id;
};

}  // namespace type
}  // namespace semantics
}  // namespace callvm

#endif
