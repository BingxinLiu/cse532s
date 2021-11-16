#include <iostream>
#include <mutex>

using namespace std;

class Sync_Output_Stream
{
    ostream& os;
    mutex print_mutex;

public:
    Sync_Output_Stream(ostream& os = cout);
    void print(const string& str);
};