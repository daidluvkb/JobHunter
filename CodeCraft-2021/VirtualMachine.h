#include <iostream>
#include <string>

using namespace std;

class VirtualMachine{
    private:
        string m_type;
        int m_num_of_cpu;
        int m_size_of_mem;
        const bool m_is_double_node;
    public:
        VirtualMachine();
        VirtualMachine(string& type, int num_of_cpu, int size_of_mem, bool is_double_node);
        
        int getNumOfCpu();
        int getSizeOfMem();
        bool IsDoubleNode();
};