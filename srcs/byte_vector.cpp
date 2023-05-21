#include "byte_vector.hpp"
#ifdef UNIT_TEST
#include "doctest.h"
#endif
ByteVector::ByteVector() {}

ByteVector::ByteVector(const char* bytes, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        this->push_back(bytes[i]);
    }
}

ByteVector::ByteVector(vector<char>::iterator begin, vector<char>::iterator end) : vector<char>(begin, end) {}

ByteVector::~ByteVector() {}

string ByteVector::get_str() const
{
    return string(this->begin(), this->end());
}

string ByteVector::get_str(int* size) const
{
    string bytes = string(this->begin(), this->end());
    *size = this->size();
    return (bytes);
}

size_t ByteVector::get_str(char* buf, size_t size) const
{
    size_t i = 0;
    vector<char>::const_iterator it = this->begin();
    while (it != this->end() && i < size) {
        buf[i] = *it;
        it++;
        i++;
    }
    return (i);
}

// Vector::size()と同じ、文脈上lengthの適切な場合はこちらをお使いください
size_t ByteVector::get_length() const
{
    return this->size();
}

void ByteVector::load(char const* bytes, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        this->push_back(bytes[i]);
    }
}
#include <iostream>
vector<char>::const_iterator ByteVector::find(ByteVector bv) const
{
    vector<char>::const_iterator it = begin();
    vector<char>::const_iterator it2 = bv.begin();
    while (it != this->end()) {
        if (*it == *it2) {
            it2++;
            if (it2 == bv.end()) {
                return it - bv.size() + 1;
            }
        } else {
            it2 = bv.begin();
        }
        it++;
    }
    throw std::runtime_error("not found");
}

std::ostream& operator<<(ostream& os, ByteVector& bv)
{
    for (size_t i = 0; i < bv.size(); i++) {
        os << bv[i];
    }
    return (os);
}

#ifdef UNIT_TEST
TEST_CASE("ByteVector::find normal case")
{
    ByteVector bv("hogehuga", 8);
    ByteVector bv2("gehu", 4);
    CHECK(bv.find(bv2) == bv.begin() + 2);
}

TEST_CASE("ByteVector::find duplicate bytes")
{
    ByteVector bv("hogehugahoge", 12);
    ByteVector bv2("hoge", 4);
    CHECK(bv.find(bv2) == bv.begin());
}

TEST_CASE("ByteVector::find find head bytes")
{
    ByteVector bv("hogehuga", 8);
    ByteVector bv2("hoge", 4);
    CHECK(bv.find(bv2) == bv.begin());
}

TEST_CASE("ByteVector::find find tail bytes")
{
    ByteVector bv("hogehuga", 8);
    ByteVector bv2("huga", 4);
    CHECK(bv.find(bv2) == bv.begin() + 4);
}

TEST_CASE("ByteVector::find4 find head 1 byte")
{
    ByteVector bv("hogehuga", 8);
    ByteVector bv2("h", 1);
    CHECK(bv.find(bv2) == bv.begin());
}

TEST_CASE("ByteVector::find find tail 1 byte")
{
    ByteVector bv("hogehuga", 8);
    ByteVector bv2("a", 1);
    CHECK(bv.find(bv2) == bv.end() - 1);
}

TEST_CASE("ByteVector::find find not found")
{
    ByteVector bv("hogehuga", 8);
    ByteVector bv2("test", 4);
    CHECK_THROWS_AS(bv.find(bv2), std::runtime_error);
}
#endif