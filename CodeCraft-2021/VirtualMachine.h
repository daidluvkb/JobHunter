#include <iostream>
#include <string>
#include "Host.h"
#include <memory>
using namespace std;

class VirtualMachine{
    private:
        string m_type;
        int m_num_of_cpu;
        int m_size_of_mem;
        const bool m_is_double_node;
        shared_ptr <Host> _host;
    public:

        VirtualMachine();
        VirtualMachine(string& type, int num_of_cpu, int size_of_mem, bool is_double_node);
        
        int getNumOfCpu();
        int getSizeOfMem();
        bool IsDoubleNode();
        shared_ptr<Host> & getHost();
        void setHost(shared_ptr<Host> & host);
};