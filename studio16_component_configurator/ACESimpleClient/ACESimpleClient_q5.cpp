#include <thread>
#include "ACESimpleClient_q5.hpp"

using namespace std;

#define SUCCESS 0

int 
main(int argc, char* argv[])
{
    client clnt(argc, argv);
    bool stop_flag = false;

    std::thread t = std::thread([&](){
        while(!stop_flag)
        {
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
                        clnt.init(argc, argv);
                        continue;
                    }
                    if (command == "fini")
                    {
                        clnt.fini();
                        continue;
                    }
                    if (command == "suspend")
                    {
                        clnt.suspend();
                        continue;
                    }
                    if (command == "resume")
                    {
                        clnt.resume();
                        continue;
                    }
                    if (command == "info")
                    {
                        clnt.info(nullptr, 0);
                        continue;
                    }
                }
                cout << "Can not parse the command [" << ss.str() << "]" << endl;

            }
        }
    });

    // Build your client program and run your client and server programs to confirm that they still communicate (with the client now running repeatedly). Your client should call the server and send its message about every 3 seconds
    cout << "release ui" << endl;
    stop_flag = true;
    if (t.joinable()) t.join();

    return SUCCESS;
}