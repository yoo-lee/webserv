#include "SyntaxError.hpp"
SyntaxError::SyntaxError(const std::string &message) : std::runtime_error(message)
{
}