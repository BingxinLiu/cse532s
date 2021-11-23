#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <mutex>
#include "ace/Service_Object.h"

using namespace std;

class singleton
{
    recursive_mutex cout_mutex;
    list<ACE_Service_Object*> handlers;
    list<string> labels;
    static singleton* singleton_;
    singleton() {}
public:
    singleton(singleton &other) = delete;
    void operator=(const singleton &) = delete;

    static singleton *GetInstance()
    {
        if(singleton_==nullptr)
        {
            singleton_ = new singleton();
        }
        return singleton_;
    }
    void update(ACE_Service_Object* handle, string label)
    {
        lock_guard<recursive_mutex> lock(this->cout_mutex);
        list<string>::iterator labels_it = labels.begin();
        for (list<ACE_Service_Object*>::iterator it = handlers.begin();
                it != handlers.end();
                ++it, ++labels_it)
        {
            if ( (*it) == handle )
            {
                (*labels_it) = label;
                this->print();
                return;
            }
                
        }
        handlers.push_back(handle);
        labels.push_back(label);
        this->print();
    }
    void print()
    {
        lock_guard<recursive_mutex> lock(this->cout_mutex);
        list<string>::iterator labels_it = labels.begin();
        for (list<ACE_Service_Object*>::iterator it = handlers.begin();
                it != handlers.end();
                ++it, ++labels_it)
        {
            cout << "[" << (*labels_it) << "|" << (*it) << "]";
        }
        cout << endl;
    }


};

singleton* singleton::singleton_ = nullptr;