#include "split.hpp"
#include <algorithm>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;


void Split::split(std::string &str, std::string delimiter)
{
    size_t del_len = delimiter.size();
    std::string::size_type offset = std::string::size_type(0);
    std::string::size_type pos;
    while(1){
        pos = str.find(delimiter, offset);
        if (pos == string::npos){
            string last = str.substr(offset);
            //if (last != delimiter && last.size() != 0)
                this->vec.push_back(str.substr(offset));
            break;
        }
        if (pos - offset > 0)
            this->vec.push_back(str.substr(offset, pos - offset));
        offset = pos + del_len;
    }

}

Split::Split(std::string &str, std::string delimiter)
{
    size_t del_len = delimiter.size();
    if (del_len == 0)
    {
        this->vec.push_back(str);
        return ;
    }
    this->split(str, delimiter);
}

void Split::concat(std::string &str, std::string delimiter)
{
    string concat = *(this->vec.end()) + str;
    this->vec.pop_back();
    this->split(concat, delimiter);
}

Split::iterator Split::begin()
{
    return (this->vec.begin());
}

Split::iterator Split::end()
{
    return (this->vec.end());
}

std::string& Split::operator[](size_t i)
{
    return (this->vec[i]);
}

size_t Split::size()
{
    return (this->vec.size());
}
