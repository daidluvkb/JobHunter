#include "VirtualMachine.h"
#include <memory>
using namespace std;

VirtualMachine::VirtualMachine(string& type, int num_of_cpu, int size_of_mem, bool is_double_node, int index)
:m_type(type), m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem), m_is_double_node(is_double_node), m_index(index){
}

int VirtualMachine::getId(){
    return m_index;
}

int VirtualMachine::getNumOfCpu(){
    return m_num_of_cpu;
}

int VirtualMachine::getSizeOfMem(){
    return m_size_of_mem;
}

int VirtualMachine::IsDoubleNode(){
    return m_is_double_node;
}



int VirtualMachine::setNode(bool left){
    if(left)    m_is_double_node = 1;
    else m_is_double_node = 2;
    return m_is_double_node;
}

shared_ptr<Host> &VirtualMachine::getHost() {
    return _host;
}


void VirtualMachine::setHost(shared_ptr<Host> & host){
    _host = host;
}

