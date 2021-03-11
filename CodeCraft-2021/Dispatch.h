#ifndef _DISPATCH_
#define _DISPATCH_
class Dispatch
{
private:
    /* data */
public:
    Dispatch(/* args */);
    ~Dispatch();
};

class BuyServer : public Dispatch
{
private:
    /* data */
public:
    BuyServer(/* args */);
    ~BuyServer();
};

class DeployVM : public Dispatch
{
};

#endif