#include "byte_vector.hpp"
ByteVector::ByteVector() {}

ByteVector::ByteVector(const char* bytes, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        this->push_back(bytes[i]);
    }
}

ByteVector::~ByteVector() {}

string ByteVector::get_array()
{
    return string(this->begin(), this->end());
}

string ByteVector::get_array(int* size)
{
    string bytes = string(this->begin(), this->end());
    *size = this->size();
    return (bytes);
}

size_t ByteVector::get_array(char* buf, size_t size)
{
    size_t i = 0;
    vector<char>::iterator it = this->begin();
    while (it != this->end() && i < size) {
        buf[i] = *it;
        it++;
        i++;
    }
    return (i);
}

// Vector::size()と同じ、文脈上lengthの適切な場合はこちらをお使いください
size_t ByteVector::get_length()
{
    return this->size();
}

void ByteVector::load(char const* bytes, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        this->push_back(bytes[i]);
    }
}

std::ostream& operator<<(ostream& os, ByteVector& bv)
{
    for (size_t i = 0; i < bv.size(); i++) {
        os << bv[i];
    }
    return (os);
}
