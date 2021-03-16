#include "Server.h"

Server::Server():m_cost_base(0), m_cost_perday(0){
}

Server::Server(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday)
:m_type(type), m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem), m_cost_base(cost_base), m_cost_perday(cost_perday){
}

int Server::getNumOfCpu(){
    return m_num_of_cpu;
}

int Server::getSizeOfMem(){
    return m_size_of_mem;
}

int Server::getCostBase(){
    return m_cost_base;
}

int Server::getCostPerDay(){
    return m_cost_perday;
}