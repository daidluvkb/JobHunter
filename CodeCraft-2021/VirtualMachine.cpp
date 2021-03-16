#include "VirtualMachine.h"

VirtualMachine::VirtualMachine()
:m_is_double_node(false){}

VirtualMachine::VirtualMachine(string& type, int num_of_cpu, int size_of_mem, bool is_double_node)
:m_type(type), m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem), m_is_double_node(is_double_node){
}

int VirtualMachine::getNumOfCpu(){
    return m_num_of_cpu;
}

int VirtualMachine::getSizeOfMem(){
    return m_size_of_mem;
}

bool VirtualMachine::IsDoubleNode(){
    return m_is_double_node;
}