#include "debugging.hpp"

using namespace std;

std::ostream& operator<<(std::ostream& os, event event_)
{
    switch (event_)
    {
    case func_call_another_func: os << "calling"; break;
    case func_be_called: os << "called"; break;
    case func_return: os << "returning"; break;
    case func_call_returned: os << "returned"; break;
    case new_object_dyn_alloc_heap: os << "allocate"; break;
    case new_object_heap_dyn_allocated: os << "allocated"; break;
    case deallocate_dyn_object: os << "deallocate"; break;
    case dyn_object_deallocated: os << "deallocated"; break;
    case acquire_mutex: os << "acquire mutex"; break;
    case mutex_acquired: os << "mutex acquired"; break;
    case release_mutex: os << "release mutex"; break;
    case mutex_released: os << "mutex released"; break;
    case thread_wait_on_cond_v: os << "wait"; break;
    case thread_wake_up_on_cond_v: os << "wake"; break;
    case thread_notify_one: os << "notify one"; break;
    case thread_notify_all: os << "notify all"; break;
    case unknown_event: os << "unknown"; break;
    default:
        os << "Error: undefined behaviour\n";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const state_snapshot& ss)
{
    os << "|---------------\n";
    os << "| " << ss.event << " \n";
    os << "| id: " << ss.thread_id << " \n";
    if (ss.this_object_ptr != nullptr)
        os << "| object: " << ss.this_object_ptr << " \n";
    if (ss.current_function != nullptr)
        os << "| current function: " << ss.current_function << " \n";
    if (ss.additional_object != nullptr)
        os << "| additional object: " << ss.additional_object << " \n";
    os << "|---------------\n";
    return os;
}


Event_logger::Event_logger() : os(cout) {}
Event_logger::Event_logger(ostream& os) : os(os) {}

void
Event_logger::register_(void* ptr, const string& str)
{
    lock_guard<mutex> lock_data(this->data_lock);
    (this->ptr_str_map).insert(make_pair(ptr, str));
}

void
Event_logger::register_(thread::id id, size_t event_buffer_size)
{
    lock_guard<mutex> lock_data(this->data_lock);
    if ( this->id_event_count_pool.find(id) != this->id_event_count_pool.end()  ) return;
    this->id_event_count_pool[id] = 0;
    // this->id_event_pool[id] = vector<pair<chrono::time_point<std::chrono::system_clock>, event>>();
    this->id_event_pool[id].reserve(event_buffer_size);
    this->id_buffer_size_pool[id] = event_buffer_size;
}

void 
Event_logger::reset(thread::id id, size_t event_buffer_size)
{
    lock_guard<mutex> lock_data(this->data_lock);
    if ( this->id_event_count_pool.find(id) == this->id_event_count_pool.end()  ) return;

    this->id_event_count_pool[id] = 0;
    this->id_event_pool[id].clear();
    this->id_event_pool[id].reserve(event_buffer_size);
    this->id_buffer_size_pool[id] = event_buffer_size;
}

void
Event_logger::unregister_(void* ptr)
{
    lock_guard<mutex> lock_data(this->data_lock);
    map<void*, const string>::iterator it = (this->ptr_str_map).find(ptr);
    if ( it != this->ptr_str_map.end() )
        this->ptr_str_map.erase(it);
}

void 
Event_logger::record(thread::id id, event event)
{
    lock_guard<mutex> lock_data(this->data_lock);
    if ( this->id_event_count_pool.find(id) == this->id_event_count_pool.end()  ) return;
    size_t current_cout = this->id_event_count_pool[id]++;
    size_t current_buffer_size = this->id_buffer_size_pool[id];
    if (current_cout/current_buffer_size >= 1)
        this->id_event_pool[id][current_cout%current_buffer_size] = make_pair(chrono::system_clock::now().time_since_epoch().count(), event);
    else
        this->id_event_pool[id].push_back(make_pair(chrono::system_clock::now().time_since_epoch().count(), event));
}

void
Event_logger::print()
{
    for (map<thread::id, vector<pair<long int, event>>>::iterator map_it = this->id_event_pool.begin(); map_it != this->id_event_pool.end(); ++map_it)
    {
        for (vector<pair<long int, event>>::iterator vec_it = (*map_it).second.begin(); vec_it != (*map_it).second.end(); ++vec_it)
        {
            this->os << "[" << (*vec_it).first << "] " << "[" << (*map_it).first << "] " << (*vec_it).second << endl;
        }
    }
}




Event_logger& 
Event_logger::operator<<(const state_snapshot ss)
{
    lock_guard<mutex> lock_data(this->data_lock);
    lock_guard<mutex> lock_os(this->print_lock);
    this->os << ss;
    map<void*, const string>::iterator it;
    it = this->ptr_str_map.find(ss.this_object_ptr);
    if ( it != this->ptr_str_map.end() )
        this->os << "| this object info:" << (*it).second << "\n";
    it = this->ptr_str_map.find(ss.current_function);
    if ( it != this->ptr_str_map.end() )
            this->os << "| current function info:" << (*it).second << "\n";
    it = this->ptr_str_map.find(ss.additional_object);
    if ( it != this->ptr_str_map.end() )
        this->os << "| additional object info:" << (*it).second << "\n";
    this->os << "|---------------" << std::endl;

    return *this;

}