#include "NotFound.hpp"
not_found::not_found(const std::string &message) : std::runtime_error(message)
{
}
