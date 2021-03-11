#ifndef _VIRTUAL_MACHINE_
#define _VIRTUAL_MACHINE_
#include <iostream>
#include <string>
using namespace std;

class VirtualMachine
{
public:
    VirtualMachine(const string &type, int num_of_cpu, int size_of_mem, bool double_node);

private:
    string _id;
    string _type;
    const int _num_of_cpu;
    const int _size_of_mem;
    const bool _is_double_node;
};
#endif