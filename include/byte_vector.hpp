#ifndef BYTE_VECTOR_H
#define BYTE_VECTOR_H
#include <ostream>
#include <stddef.h>
#include <string>
#include <vector>
using std::ostream;
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
    string get_array(int* size) const;

    string get_array() const;
    // bufにはarrayを最大size分コピーし, コピーしたサイズを返します
    size_t get_array(char* buf, size_t size) const;
    size_t get_length() const;
    void load(char const* bytes, size_t size);
    vector<char>::const_iterator find(ByteVector bv) const;

    friend std::ostream& operator<<(ostream& os, ByteVector& bv);
};

#endif /* BYTE_VECTOR_H */
