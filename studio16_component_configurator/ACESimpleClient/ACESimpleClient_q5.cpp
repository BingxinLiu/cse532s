#include <thread>
#include "ACESimpleClient_q5.hpp"

using namespace std;

#define SUCCESS 0

int 
main(int argc, char* argv[])
{
    service_wrapper service;
    bool stop_flag = false;

    string command_line;
    string command;
    string mm_address;
    string label;
    while (true)
    {
        getline(cin, command_line);
        stringstream ss(command_line);
        if (ss >> command)
        { 
            if (command == "rename")
            {
                if (ss >> mm_address >> label)
                {
                    singleton::GetInstance()->update((ACE_Service_Object *)stol(mm_address, nullptr, 16), label);
                    continue;
                }
                
            }
            if (command == "init")
            {
                service.init(argc, argv);
                continue;
            }
            if (command == "fini")
            {
                service.fini();
                continue;
            }
            if (command == "suspend")
            {
                service.suspend();
                continue;
            }
            if (command == "resume")
            {
                service.resume();
                continue;
            }
            if (command == "info")
            {
                service.info(nullptr, 0);
                continue;
            }
            if (command == "quit")
            {
                service.info(nullptr, 0);
                break;
            }
        }
        cout << "Can not parse the command [" << ss.str() << "]" << endl;

    }

    cout << "ui released" << endl;

    return SUCCESS;
}