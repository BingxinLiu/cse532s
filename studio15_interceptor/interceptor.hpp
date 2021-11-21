
class interceptor
{
public:
    virtual void operator()(char&)=0;
    virtual void operator()(char&, int)=0;
};