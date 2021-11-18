#include "interceptor.hpp"

class pass_through_interceptor : public interceptor
{
    void operator()(char&);
};