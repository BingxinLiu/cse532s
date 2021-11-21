#include "interceptor.hpp"

class ror_interceptor : public interceptor
{
    void operator()(char&);
    void operator()(char& c, int size);
};