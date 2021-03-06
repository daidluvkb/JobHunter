#include "VirtualMachine.h"
#include <memory>
#include "utils.h"
using namespace std;

VirtualMachine::VirtualMachine(string &type, int num_of_cpu, int size_of_mem, bool is_double_node, int index)
    : m_type(type),
      m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem),
      m_index(index),
      _host(nullptr)
{
    if (is_double_node)
        m_is_double_node = 0;
    else
    {
        m_is_double_node = -1;
    }
}

int VirtualMachine::getId()
{
    return m_index;
}

int VirtualMachine::getNumOfCpu()
{
    return m_num_of_cpu;
}

int VirtualMachine::getSizeOfMem()
{
    return m_size_of_mem;
}

int VirtualMachine::IsDoubleNode() const
{
    return m_is_double_node;
}

int VirtualMachine::setNode(bool isA)
{
    if(isA)
        m_is_double_node = 1;
    else
        m_is_double_node = 2;

    return m_is_double_node;
}

char VirtualMachine::getNode() 
{
    if (m_is_double_node == 1)
    {
        return 'A';
    }
    else if(m_is_double_node == 2)
    {
        return 'B';
    }
    if (m_is_double_node == -1)
    {
        // cout << 'C' << endl;
        return 'C';
    }
    // cout << 'D' << endl;
    return 'D'; 
}

shared_ptr<Host> VirtualMachine::getHost()
{
    return _host;
}

void VirtualMachine::setHost(shared_ptr<Host> &host)
{
    _host = host;
}

void VirtualMachine::checkMyself() 
{
    if (m_is_double_node==-1)
    {
        dcout << "vm dont have host" << endl;        
    }
    
    
}
