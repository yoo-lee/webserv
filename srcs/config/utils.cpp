#include <sstream>
#include <string>
int myStoi(std::string s)
{
    std::stringstream ss(s);
    int n;
    ss >> n;
    return n;
}

template <typename T>
std::string myToString(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string toFormatString(int n)
{
    return (n >= 1 && n <= 9 ? "0" + myToString(n) : myToString(n));
}
