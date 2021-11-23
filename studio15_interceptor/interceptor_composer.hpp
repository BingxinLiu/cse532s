#include <iostream>
#include <vector>
#include <functional>

#include "interceptor.hpp"

using namespace std;

class interceptor_composer
{
    vector<std::_Bind_helper<false, void (interceptor::*)(char&, int), interceptor*&, const std::_Placeholder<1>&, int&>::type> chain;
public:
    void compose(interceptor* intercptr, int size)
    {
        chain.push_back(std::bind(
            (void(interceptor::*)(char&, int)) &interceptor::operator(),
            intercptr,
            std::placeholders::_1,
            size
        ));
    };
    void execute(char& c)
    {
        for (vector<std::_Bind_helper<false, void (interceptor::*)(char&, int), interceptor*&, const std::_Placeholder<1>&, int&>::type>::iterator it = chain.begin();
        it != chain.end();
        ++it
        )
        {
            (*it)(c);
        }
    }

};


//auto func6 = std::bind(static_cast<void(Derived::*)(int)const>(&Derived::Base::f1), std::cref(derived), std::placeholders::_1); 