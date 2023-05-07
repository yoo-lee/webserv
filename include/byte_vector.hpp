#ifndef BYTE_VECTOR_H
#define BYTE_VECTOR_H
#include <stddef.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

class ByteVector : public vector<char>
{
  private:
  public:
    ByteVector();
    ByteVector(const char* bytes, size_t size);
    ~ByteVector();

    // sizeには返す配列のサイズを格納します
    string get_array(int* size);

    string get_array();
    // bufにはarrayを最大size分コピーし, コピーしたサイズを返します
    size_t get_array(char* buf, size_t size);
    size_t get_length();
    void load(char const* bytes, size_t size);
};

#endif /* BYTE_VECTOR_H */
