#include "sync_os.hpp"

Sync_Output_Stream::Sync_Output_Stream(ostream& os) \
    : os(os)
{}

void 
Sync_Output_Stream::print(const string& str)
{
    lock_guard<mutex> print_lock(print_mutex);
    this->os << str << endl;
}
