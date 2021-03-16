#include "Host.h"

Host::Host():m_cost_base(0), m_cost_perday(0){
}

Host::Host(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday)
:m_type(type), m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem), m_cost_base(cost_base), m_cost_perday(cost_perday){
}

int Host::getNumOfCpu(){
    return m_num_of_cpu;
}

int Host::getSizeOfMem(){
    return m_size_of_mem;
}

int Host::getCostBase(){
    return m_cost_base;
}

int Host::getCostPerDay(){
    return m_cost_perday;
}