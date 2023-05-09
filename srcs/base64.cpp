#include "base64.hpp"
#include <iostream>
#include <stdio.h>

using std::string;
using std::cout;
using std::endl;


static char to_base64(unsigned char c)
{
    int d = c;
    cout << "to base64 c=" << d << endl;
    if (c <= 25){
        return ('A' + c);
    }else if (c <= 51){
        return ('a' + c - 26);
    }else if (c <= 61){
        return ('0' + c - 52);
    }else if (c == 61){
        return ('+');
    }
    return ('/');
}

std::string Base64::encode(std::string str)
{

    size_t size = ((str.size() / 3) + 1) * 4;
    char bits[size+1]={0};
    const char *char_str = str.c_str();
    unsigned char c;
    size_t i = 0;
    size_t k = 0;
    unsigned char bit = 0;
    unsigned char rest = 0;
    int pos = 0;
    unsigned char rest_array[] = {0b00000000, 0b00000001, 0b00000011, 0b00000111, 0b00001111, 0b00011111, 0b00111111, 0b01111111, 0b11111111};
    while(char_str[i])
    {
        c = (unsigned char)char_str[i];
        pos = pos - 6;
        if (pos < 0)
            pos += 8;
        bit = rest | (c >> pos);
        rest = (c & rest_array[pos]) << (6-pos);
        bits[k++] = to_base64(bit);
        if (pos == 6){
            bits[k++] = to_base64(rest);
            rest = 0;
            pos = 0;
        }
        i++;
    }
    if (pos != 6){
        bits[k++] = to_base64(rest);
    }
    while(size > k){
        bits[k++] = '=';
    }
    bits[size] = 0;
    return string(bits);
}


