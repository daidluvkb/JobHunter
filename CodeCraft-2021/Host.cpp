#include "Host.h"


Host::Host(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday, int index)
:m_type(type), m_num_of_cpu(num_of_cpu), m_size_of_mem(size_of_mem), m_cost_base(cost_base), m_cost_perday(cost_perday),
m_index(index){
    _left_cpu_A = num_of_cpu / 2;
    _left_cpu_B = _left_cpu_A;
    _left_mem_A = size_of_mem / 2;
    _left_mem_B = _left_mem_A;
}

Host::Host(const HostInfo &hostinfo, const int index)
    : m_type(hostinfo.type),
      m_num_of_cpu(hostinfo.cpu), m_size_of_mem(hostinfo.mem),
      m_cost_base(hostinfo.basicCost), m_cost_perday(hostinfo.dailyCost),
      m_index(index)
{    
    _left_cpu_A = m_num_of_cpu / 2;
    _left_cpu_B = _left_cpu_A;
    _left_mem_A = m_size_of_mem / 2;
    _left_mem_B = _left_mem_A;
}

int Host::getNumOfCpu()
{
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

int Host::getIndex() 
{
    return m_index;
}

bool Host::addVM(shared_ptr<VirtualMachine>& vm){
    
    bool success = false;
    if(vm->IsDoubleNode() == 0){
        // cout << "vm: " << vm->getNumOfCpu() << '\t' << vm->getSizeOfMem()<< endl;
        // print();
        _left_cpu_A -= (vm->getNumOfCpu() / 2);
        _left_cpu_B -= (vm->getNumOfCpu() / 2);
        _left_mem_A -= (vm->getSizeOfMem() / 2);
        _left_mem_B -= (vm->getSizeOfMem() / 2);
        // print();
        success = true;
    }
    else{
        if(_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->getNumOfCpu()){
            vm->setNode(true);
            _left_cpu_A -= vm->getNumOfCpu();
            _left_mem_A -= vm->getSizeOfMem();
            success = true;
        }
        else if(_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->getNumOfCpu()){
            vm->setNode(false);
            _left_cpu_B -= vm->getNumOfCpu();
            _left_mem_B -= vm->getSizeOfMem();
            success = true;
        }        
    }
    if(success)
        _vms[vm->getId()] = vm;
    return success;
}

bool Host::addVM_try(shared_ptr<VirtualMachine> &vm)
{
    bool success = false;
    if (vm->IsDoubleNode() == 0)
    {
        // cout << "vm: " << vm->getNumOfCpu() << '\t' << vm->getSizeOfMem()<< endl;
        // print();
        if (getAvailableCpu(true) > (vm->getNumOfCpu() / 2) &&
            getAvailableMem(true) > (vm->getSizeOfMem() / 2))
        {
            /* code */
            _left_cpu_A -= (vm->getNumOfCpu() / 2);
            _left_cpu_B -= (vm->getNumOfCpu() / 2);
            _left_mem_A -= (vm->getSizeOfMem() / 2);
            _left_mem_B -= (vm->getSizeOfMem() / 2);
            // print();
            success = true;
        }
    }
    else
    {
        if (getAvailableCpu(false) > vm->getNumOfCpu() &&
            getAvailableMem(false) > vm->getSizeOfMem())
        {
            if (_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->getNumOfCpu())
            {
                vm->setNode(true);
                _left_cpu_A -= vm->getNumOfCpu();
                _left_mem_A -= vm->getSizeOfMem();
                success = true;
            }
            else if (_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->getNumOfCpu())
            {
                vm->setNode(false);
                _left_cpu_B -= vm->getNumOfCpu();
                _left_mem_B -= vm->getSizeOfMem();
                success = true;
            }
        }
    }
    if (success)
        _vms[vm->getId()] = vm;
    return success;
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

void Host::print() const
{
    cout << "left cpu :" << _left_cpu_A << '\t' << _left_cpu_B << endl;
    cout << "left mem :" << _left_mem_A << '\t' << _left_mem_B << endl;
}
void Host::checkMyself() const
{
    int cpuA = 0, cpuB = 0, memA = 0, memB = 0; 
    for (auto &i : _vms)
    {
        if (i.second->getHost().get()!= this)
        {
            cout << "vm host not match" << endl;
        }
        
        int cpu = i.second->getNumOfCpu(), mem = i.second->getSizeOfMem();
        if (i.second->IsDoubleNode() == 0)
        {
            cpuA += cpu / 2;
            cpuB += cpu / 2;
            memA += mem / 2;
            memB += mem / 2;
        }
        else if(i.second->IsDoubleNode() == 1)
        {
            cpuA += cpu;
            memA += mem;
        }
        else if (i.second->IsDoubleNode() == 2)
        {
            cpuB += cpu;
            memB += mem;
        }else
        {
            cout << "vm wrong" << endl;
        }        
    }
    if (cpuA + _left_cpu_A != m_num_of_cpu / 2 ||
        cpuB + _left_cpu_B != m_num_of_cpu / 2 ||
        memB + _left_mem_B != m_size_of_mem / 2 ||
        memA + _left_mem_A != m_size_of_mem / 2)
    {
        cout << "used cpu: " << cpuA << '\t' << cpuB << endl;
        cout << "used mem: " << memA << '\t' << memB << endl;
        print();
        cout << "sum: " << m_num_of_cpu << '\t' << m_size_of_mem << endl;
    }
}

