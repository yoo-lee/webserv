#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <vector>

class Split
{
  public:
    Split(std::string& str, std::string delimiter);
    typedef std::vector<std::string>::iterator iterator;
    iterator begin();
    iterator end();
    size_t size();
    std::string& operator[](size_t i);
    void concat(std::string& str, std::string delimiter);

  private:
    std::vector<std::string> vec;
    void split(std::string& str, std::string delimiter);
};

#endif
