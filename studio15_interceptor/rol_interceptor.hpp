#include "interceptor.hpp"

class rol_interceptor : public interceptor
{
    void operator()(char&);
    void operator()(char& c, int size);
};