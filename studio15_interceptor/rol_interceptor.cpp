#include <iostream>

#include "rol_interceptor.hpp"


void
rol_interceptor::operator()(char& c)
{
    return;

}

void
rol_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res + size;
    std::cout << "[" << std::hex << (0xFF & c) << "|";
    unsigned char res = ((unsigned char)c) << size;
    res |= ((unsigned char)c) >> (8 - size);
    std::cout << std::hex << (0xFF & res) << "]";
    
    c = (char) res;

}