#ifndef _SERVER_
#define _SERVER_
#include <iostream>
#include <string>
using namespace std;

class Server
{
public:
Server(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday);
private:
    string _id;
    string _type;
    int _num_of_cpu;
    int _size_of_mem;
    const int _cost_base;
    const int _cost_perday;

};
#endif