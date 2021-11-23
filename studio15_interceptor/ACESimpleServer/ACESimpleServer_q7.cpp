#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "ace/Event_Handler.h"

#include "ace/Reactor.h"

#include "../interceptor_composer.hpp"

#include <algorithm>

using namespace std;

#define SUCCESS 0
#define BUFFER_SIZE 256

static size_t client_counter = 0;

class server_read;
class server_accept;

class server_read : public ACE_Event_Handler
{
    bool has_not_released ;
    ACE_SOCK_Stream* ace_sock_stream;
    size_t id;
    interceptor_composer* server_composer;
    vector<interceptor*> interceptors;
public:
    server_read() : has_not_released(true), id(client_counter), server_composer(nullptr)
    {
        cout << "new server read" << endl;
    }
    ~server_read()
    {

        cout << "release server read and sock stream for reader " << to_string(id) << endl;
        delete ace_sock_stream;
        for (vector<interceptor*>::iterator it = interceptors.begin();
        it != interceptors.end();
        ++it)
        {
            delete (*it);
        }
        delete server_composer;
            
    }
    
    void set_composer(interceptor_composer* composer, vector<interceptor*> interceptors)
    {
        this->server_composer = composer;
        this->interceptors = interceptors;
    }
    
    void set(ACE_SOCK_Stream* ss)
    {
        this->ace_sock_stream = ss;
    }

    virtual ACE_HANDLE get_handle() const
    {
        cout << "ace_sock_stream.get_handle" << endl;
        return this->ace_sock_stream->get_handle();
    }

    virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE)
    {
        char buffer[BUFFER_SIZE];
        size_t recv_len = 0;

        recv_len = this->ace_sock_stream->recv(&buffer, BUFFER_SIZE);
        if ( recv_len <= 0 )
        {
            cout << "The socket is closed." << endl;
            int ret;
            ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::NULL_MASK);
            if ( ret < 0)
                cout << "remove handler failed" << endl;
            ret = this->ace_sock_stream->close();
            if ( ret < 0)
                cout << "close socket failed" << endl;
            cout << "reader remove self" << endl;
            return -1;
        }
        cout << "receive " << recv_len << "[" << string(buffer) << "]" << endl;
        if ( recv_len != 2 && this->server_composer != nullptr )
        {
            for (size_t i = 0; i < recv_len; ++i)
                this->server_composer->execute(buffer[i]);
            buffer[recv_len] = '\0';
        }
        string recv_str(buffer);
        cout << "RECV [" << recv_str << "]" << endl;
        if ( recv_len == 2 && stoi(recv_str) == 0 )
        {
            cout << "Receive new client" << endl;
            cout << "Client Counter: " << client_counter << endl;
            string send_str = to_string(client_counter);
            cout << "send: " << send_str << endl;
            if ( this->ace_sock_stream->send_n(&client_counter, sizeof(client_counter)) < 0 )
                cout << "Error: Can not send client id" << endl; 
            client_counter++;
        }


        //ACE_Reactor::instance()->run_reactor_event_loop();
        return 0;
    }

    virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
    {
        cout << "handle close in reader "; 
        if ( mask & READ_MASK )
        {
            cout << "with READ_MASK";
            if ( this->has_not_released )
            {
                cout << "try to delete this" << endl;
                delete this;
            
            }
        }       
        cout << endl;
        //int ret;
        // ret = ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::DONT_CALL);
        // if ( ret < 0 )
        //     cout << "Warnning: Can not remove this from reactor" << endl;
        // cout << "remove reader done" << endl;
        // ret = this->ace_sock_stream.close();
        // if ( ret < 0 )
        //     cout << "Warnning: Can not close this ace_sock_stream" << endl;

        //cout << "delete this done" << endl;
        return 0;
    }



};

vector<server_read*> server_readers;

class server_accept : public ACE_Event_Handler
{
    ACE_SOCK_Acceptor& acceptor;
    public:
    server_accept(ACE_SOCK_Acceptor acceptor) : acceptor(acceptor) {}
    ~server_accept()
    {
        cout << "release server acceptor" << endl;
    }

    virtual ACE_HANDLE get_handle() const
    {
        cout << "acceptor get handle" << endl;
        return acceptor.get_handle();
    }

    virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE)
    {
        cout << "acceptor handle input" << endl;


        ACE_SOCK_Stream* ace_sock_stream = new ACE_SOCK_Stream;

        if ( acceptor.accept(*ace_sock_stream) < 0 )
        {
            cout << "Error: Can not accept the ace_sock_stream" << endl;
            delete ace_sock_stream;
            return EISCONN;
        }

        server_read* reader = new server_read;

        vector<interceptor*> interceptors;
        rotation_interceptor* r_inter = new rotation_interceptor;
        multi_interceptor* m_inter = new multi_interceptor;
        interceptors.push_back(r_inter);
        interceptors.push_back(m_inter);

        interceptor_composer* composer = new interceptor_composer;
        composer->compose(r_inter, -3);
        composer->compose(m_inter, -2);
        

        reader->set(ace_sock_stream);
        reader->set_composer(composer, interceptors);
        ACE_Reactor::instance()->register_handler(reader, ACE_Event_Handler::READ_MASK);

        //server_readers.push_back(reader);
        
        // this step is neccessary
        //ACE_Reactor::instance()->run_reactor_event_loop();
        cout << "handle accept done" << endl;
        return 0;
    }

    virtual int handle_signal (int signal, siginfo_t* = 0, ucontext_t* = 0)
    {
        cout << "handle signal" << endl;
        int ret;
        ret = ACE_Reactor::instance()->end_reactor_event_loop();
        if ( ret < 0)
            cout << "Error in ACE_Reactor::instance()->end_reactor_event_loop() with error code: " << ret << endl;
        //debug
        cout << "acceptor end_reactor_event_loop()" << endl;
        ret = ACE_Reactor::instance()->close();
        if ( ret < 0)
            cout << "Error in ACE_Reactor::instance()->close() with error code: " << ret << endl;
        cout << "end and close done" << endl;
        return 0;
    }

    virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
    {
        if ( (mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
            cout << "handle close with ACCEPT_MASK and SIGNAL_MASK" << endl;
        if ( !(mask & ACCEPT_MASK) && (mask & SIGNAL_MASK) )
            cout << "handle close with SIGNAL_MASK" << endl;
        if ( (mask & ACCEPT_MASK) && !(mask & SIGNAL_MASK) )
            cout << "handle close with ACCEPT_MASK" << endl;

        return 0;
    }
};

int 
main(int argc, char* argv[])
{
    if (argc != 1)
    {
        cout << "Error: Too many command line arguments." << endl;
        return EINVAL;
    }
     
    string arg(argv[argc - 1]);
    cout << arg << endl;

    ACE_TCHAR buffer[BUFFER_SIZE];
    ACE_INET_Addr address(8086, ACE_LOCALHOST);
    ACE_SOCK_Acceptor acceptor;
    if ( acceptor.open(address, 1) < 0)
    {
        address.addr_to_string(buffer, BUFFER_SIZE, 1);
        cout << "Error: Can not listen on " << buffer << endl;
        return EINVAL;
    }
    address.addr_to_string(buffer, BUFFER_SIZE, 1);
    cout << "Start listening on " << buffer << endl;

    server_accept server(acceptor);

    ACE_Reactor::instance()->register_handler(&server, ACE_Event_Handler::ACCEPT_MASK);
    ACE_Reactor::instance()->register_handler(SIGINT, &server);

    ACE_Reactor::instance()->run_reactor_event_loop();

    // for(vector<server_read*>::iterator it = server_readers.begin(); it != server_readers.end(); ++it)
    //     delete (*it);

    cout << "Server Stopped" << endl;

    return SUCCESS;
}