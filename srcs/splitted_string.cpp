#include "splitted_string.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void SplittedString::split(std::string& str, std::string delimiter)
{
    size_t del_len = delimiter.size();
    std::string::size_type offset = std::string::size_type(0);
    std::string::size_type pos;
    while (1) {
        pos = str.find(delimiter, offset);
        if (pos == string::npos) {
            string last = str.substr(offset);
            this->_splitted_string.push_back(str.substr(offset));
            break;
        }
        if (pos - offset > 0)
            this->_splitted_string.push_back(str.substr(offset, pos - offset));
        offset = pos + del_len;
    }
}

SplittedString::SplittedString(std::string& str, std::string delimiter)
{
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }
    this->split(str, delimiter);
}

void SplittedString::concat(std::string& str, std::string delimiter)
{
    string concat = *(this->_splitted_string.end()) + str;
    this->_splitted_string.pop_back();
    this->split(concat, delimiter);
}

SplittedString::iterator SplittedString::begin()
{
    return (this->_splitted_string.begin());
}

SplittedString::iterator SplittedString::end()
{
    return (this->_splitted_string.end());
}

std::string& SplittedString::operator[](size_t i)
{
    return (this->_splitted_string[i]);
}

size_t SplittedString::size()
{
    return (this->_splitted_string.size());
}

std::ostream& operator<<(std::ostream& os, SplittedString& sp)
{
    std::vector<std::string>::iterator ite = sp.begin();
    std::vector<std::string>::iterator end = sp.end();
    for (; ite != end; ite++) {
        os << "[" << *ite << "]";
    }
    return (os);
}