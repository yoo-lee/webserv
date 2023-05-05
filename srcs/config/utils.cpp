#include <sstream>
#include <string>
int my_stoi(std::string const &s)
{
    std::stringstream ss(s);
    int n;
    ss >> n;
    return n;
}
