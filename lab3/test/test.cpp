#include "../utilities/threadsafe_io.hpp"
int main()
{
    (*threadsafe_io::get_instance()) << ">?>>" << 123;
    return 0;
}