#include "Host.h"


Host::Host(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday, int index)
:m_type(type), m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem), m_cost_base(cost_base), m_cost_perday(cost_perday),
m_index(index){
    _left_cpu_A = num_of_cpu / 2;
    _left_cpu_B = _left_mem_A;
    _left_mem_A = size_of_mem / 2;
    _left_mem_B = _left_mem_A;
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


void Host::deleteVM(int id){
    if(!_vms.count(id)) return;

    if(_vms[id]->IsDoubleNode() == 0){
        _left_cpu_A += (_vms[id]->getNumOfCpu() / 2);
        _left_cpu_B += (_vms[id]->getNumOfCpu() / 2);
        _left_mem_A += (_vms[id]->getSizeOfMem() / 2);
        _left_mem_B += (_vms[id]->getSizeOfMem() / 2);
    }
    else if(_vms[id]->IsDoubleNode() == 1){
        _left_cpu_A += _vms[id]->getNumOfCpu();
        _left_mem_A += _vms[id]->getSizeOfMem();
    }
    else if(_vms[id]->IsDoubleNode() == 2){
        _left_cpu_B += _vms[id]->getNumOfCpu();
        _left_mem_B += _vms[id]->getSizeOfMem();
    }

    _vms.erase(id);

    return;
}

void Host::addVM(shared_ptr<VirtualMachine>& vm){
    _vms[vm->getId()] = vm;
    if(vm->IsDoubleNode() == 0){
        _left_cpu_A -= (vm->getNumOfCpu() / 2);
        _left_cpu_B -= (vm->getNumOfCpu() / 2);
        _left_mem_A -= (vm->getSizeOfMem() / 2);
        _left_mem_B -= (vm->getSizeOfMem() / 2);
    }
    else{
        if(_left_mem_A > _left_mem_B){
            vm->setNode(true);
            _left_cpu_A -= vm->getNumOfCpu();
            _left_mem_A -= vm->getSizeOfMem();
        }
        else{
            vm->setNode(false);
            _left_cpu_B -= vm->getNumOfCpu();
            _left_mem_B -= vm->getSizeOfMem();
        }
    }
    return;
}

int Host::getAvailableCpu(bool isDouble){
    if(isDouble)    return min(_left_cpu_A, _left_cpu_B);
    else return max(_left_cpu_A, _left_cpu_B);
}

int Host::getAvailableMem(bool isDouble){
    if(isDouble)    return min(_left_mem_A, _left_mem_B);
    else    return max(_left_mem_A, _left_mem_B);
}

bool Host::isFree(){
    if(_left_cpu_A + _left_cpu_B == m_num_of_cpu)   return true;
    return false;
}