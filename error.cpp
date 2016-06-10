#include "error.hpp"

namespace crowd {
ERROR_TYPE Exception::type() const { return type_; };
}