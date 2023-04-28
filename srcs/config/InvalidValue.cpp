#include "InvalidValue.hpp"
InvalidValue::InvalidValue(const std::string &message) : std::runtime_error(message) {}