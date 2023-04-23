#include "SyntaxError.hpp"
syntax_error::syntax_error(const std::string &message) : std::runtime_error(message)
{
}