#ifndef _VM_H_
#define _VM_H_
#include <iostream>
#include <string>
#include <memory>
//#include "Host.h"
using namespace std;

class Host;
class VirtualMachine
{
private:
    const string m_type;
    const int m_num_of_cpu;
    const int m_size_of_mem;
    const bool m_is_double_node;
    const int m_index;
    shared_ptr<Host> _host;

public:
    //        VirtualMachine();
    VirtualMachine(string &type, int num_of_cpu, int size_of_mem, bool is_double_node, int index);
    int getId();
    int getNumOfCpu();
    int getSizeOfMem();
    bool IsDoubleNode();
    shared_ptr<Host> &getHost(); // get the host's pointer
    void setHost(shared_ptr<Host> &host);
};

#endif