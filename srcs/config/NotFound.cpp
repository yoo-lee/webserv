#include "NotFound.hpp"
NotFound::NotFound(const std::string &message) : std::runtime_error(message) {}
