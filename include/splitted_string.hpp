#ifndef SPLITTED_STRING_H
#define SPLITTED_STRING_H

#include <string>
#include <vector>

class SplittedString
{
  public:
    SplittedString(std::string& str, std::string delimiter);
    typedef std::vector<std::string>::iterator iterator;
    iterator begin();
    iterator end();
    size_t size();
    std::string& operator[](size_t i);
    void concat(std::string& str, std::string delimiter);
    friend std::ostream& operator<<(std::ostream& os, SplittedString& sp);

  private:
    std::vector<std::string> _splitted_string;
    void split(std::string& str, std::string delimiter);
};

#endif /* SPLITTED_STRING_H */
