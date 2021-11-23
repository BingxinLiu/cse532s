#include <iostream>

#include "interceptor.hpp"

void
mod_interceptor::operator()(char& c)
{
    return;
}

void
mod_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res - size;
    std::cout << "[" << std::hex << (0xFF & c) << "|";
    unsigned char res = ((unsigned char)c);
    res = ((int) res) + size;
    std::cout << std::hex << (0xFF & res) << "]";
    
    c = (char) res;
}

void
pass_through_interceptor::operator()(char& c)
{
    return;

}

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

void
ror_interceptor::operator()(char& c)
{
    return;

}


void
ror_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res - size;
    std::cout << "[" << std::hex << (0xFF & c) << "|";
    unsigned char res = ((unsigned char)c) >> size;
    res |= ((unsigned char)c) << (8 - size);
    std::cout << std::hex << (0xFF & res) << "]";
    c = (char) res;

}

void
rotation_interceptor::operator()(char& c)
{
    return;

}

void
rotation_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res - size;
    if ( size > 0 )
    {
        std::cout << "[" << std::hex << (0xFF & c) << "|";
        unsigned char res = ((unsigned char)c) >> size;
        res |= ((unsigned char)c) << (8 - size);
        std::cout << std::hex << (0xFF & res) << "]";
        c = (char) res;
    } else if ( size < 0 ) 
    {
        size = -size;
        std::cout << "[" << std::hex << (0xFF & c) << "|";
        unsigned char res = ((unsigned char)c) << size;
        res |= ((unsigned char)c) >> (8 - size);
        std::cout << std::hex << (0xFF & res) << "]";
        
        c = (char) res;
    } else 
    {
        return;
    }
}

void
multi_interceptor::operator()(char& c)
{
    return;

}

void
multi_interceptor::operator()(char& c, int size)
{
    // char res = c;
    // res = res - size;
    if ( size > 0 )
    {
        std::cout << "[" << std::hex << (0xFF & c) << "|";
        char res = (char) ((unsigned int)c * size);
        std::cout << std::hex << (0xFF & res) << "]";
        c = res;
    } else if ( size < 0 ) 
    {
        size = -size;
        std::cout << "[" << std::hex << (0xFF & c) << "|";
        char res = (char) ((unsigned int)c / size);
        std::cout << std::hex << (0xFF & res) << "]";
        
        c = res;
    } else 
    {
        return;
    }
}