class interceptor
{
public:
    virtual void operator()(char&)=0;
    virtual void operator()(char&, int)=0;
};

class pass_through_interceptor : public interceptor
{
public:
    void operator()(char&);
};


class mod_interceptor : public interceptor
{
public:
    void operator()(char&);
    void operator()(char& c, int size);
};

class rol_interceptor : public interceptor
{
public:
    void operator()(char&);
    void operator()(char& c, int size);
};

class ror_interceptor : public interceptor
{
public:
    void operator()(char&);
    void operator()(char& c, int size);
};

class rotation_interceptor : public interceptor
{
public:
    void operator()(char&);
    void operator()(char& c, int size);
};

class multi_interceptor : public interceptor
{
public:
    void operator()(char&);
    void operator()(char& c, int size);
};