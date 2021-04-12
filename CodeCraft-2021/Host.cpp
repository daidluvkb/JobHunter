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

int Host::getSmallestCapacityAfterAdd(const shared_ptr<VirtualMachine> &vm) const
{
    int smallest = INT16_MAX;
    if(vm->IsDoubleNode() == 0){

        smallest = min(smallest, _left_cpu_A - (vm->getNumOfCpu() / 2));
        smallest = min(smallest, _left_cpu_B - (vm->getNumOfCpu() / 2));
        smallest = min(smallest, _left_mem_A - (vm->getSizeOfMem() / 2));
        smallest = min(smallest, _left_mem_B - (vm->getSizeOfMem() / 2));

    }
    else{
        if(_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->getNumOfCpu()){

            smallest = min(smallest, _left_cpu_A - vm->getNumOfCpu());
            smallest = min(smallest, _left_mem_A - vm->getSizeOfMem());
        }
        else if(_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->getNumOfCpu()){
       
            smallest = min(smallest, _left_cpu_B - vm->getNumOfCpu());
            smallest = min(smallest, _left_mem_B - vm->getSizeOfMem());
        }        
    }
    return smallest;
}

int Host::getLargestCapacityAfterAdd(const shared_ptr<VirtualMachine>& vm) const
{
    int largest = 0;
    if (vm->IsDoubleNode() == 0)
    {

        largest = max(largest, _left_cpu_A - (vm->getNumOfCpu() / 2));
        largest = max(largest, _left_cpu_B - (vm->getNumOfCpu() / 2));
        largest = max(largest, _left_mem_A - (vm->getSizeOfMem() / 2));
        largest = max(largest, _left_mem_B - (vm->getSizeOfMem() / 2));
    }
    else
    {
        if (_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->getNumOfCpu())
        {

            largest = max(largest, _left_cpu_A - vm->getNumOfCpu());
            largest = max(largest, _left_mem_A - vm->getSizeOfMem());
        }
        else if (_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->getNumOfCpu())
        {

            largest = max(largest, _left_cpu_B - vm->getNumOfCpu());
            largest = max(largest, _left_mem_B - vm->getSizeOfMem());
        }
    }
    return largest;
}



void Host::getAbnormalCapcityAfterAdd(const shared_ptr<VirtualMachine> &vm, int &small, int &large) const
{
    int largest = 0;
    int tmpdata[4];
    const int vm_cpu = vm->cpu;
    const int vm_mem = vm->mem;
    if (vm->IsDoubleNode() == 0)
    {
        tmpdata[0] = _left_cpu_A - (vm_cpu / 2);
        tmpdata[1] = _left_cpu_B - (vm_cpu / 2);
        tmpdata[2] = _left_mem_A - (vm_mem / 2);
        tmpdata[3] = _left_mem_B - (vm_mem / 2);
    }
    else
    {
        
        tmpdata[0] = _left_cpu_A ;
        tmpdata[1] = _left_cpu_B ;
        tmpdata[2] = _left_mem_A ;
        tmpdata[3] = _left_mem_B ;
        bool A = false, B = false;
        if (_left_cpu_A >= vm_cpu && _left_mem_A >= vm_mem)
        {
            A = true;
        }
        if (_left_cpu_B >= vm_cpu && _left_mem_B >= vm_mem)
        {
            B = true;
        }
        if (A && B)
        {
            if (_left_cpu_A >= _left_cpu_B)
            {
                tmpdata[0] = _left_cpu_A - vm_cpu;
                tmpdata[2] = _left_mem_A - vm_mem;
            }
            else
            {
                tmpdata[1] = _left_cpu_B - vm_cpu;
                tmpdata[3] = _left_mem_B - vm_mem;
            }
        }
        else if (!A)
        {
            tmpdata[1] = _left_cpu_B - vm_cpu;
            tmpdata[3] = _left_mem_B - vm_mem;
        }
        else
        {
            tmpdata[0] = _left_cpu_A - vm_cpu;
            tmpdata[2] = _left_mem_A - vm_mem;
        }
    }
    small = tmpdata[0];
    for (size_t i = 1; i < 4; i++)
    {
        small = min(small, tmpdata[i]);
    }
    large = small == tmpdata[0] ? tmpdata[2] : large;
    large = small == tmpdata[2] ? tmpdata[0] : large;
    large = small == tmpdata[1] ? tmpdata[3] : large;
    large = small == tmpdata[3] ? tmpdata[1] : large;
}

double Host::getRemainCapacityAfterAdd(const shared_ptr<VirtualMachine> &vm, int &cpu, int &mem) const
{
     int largest = 0;
    int tmpdata[4];
    const int vm_cpu = vm->cpu;
    const int vm_mem = vm->mem;

    if (!vm || vm->IsDoubleNode() == 0)
    {
        tmpdata[0] = _left_cpu_A - (vm_cpu / 2);
        tmpdata[1] = _left_cpu_B - (vm_cpu / 2);
        tmpdata[2] = _left_mem_A - (vm_mem / 2);
        tmpdata[3] = _left_mem_B - (vm_mem / 2);
        if (_left_cpu_A + _left_mem_A <= _left_mem_B + _left_cpu_B) //A剩余少 按照A算
        {
            cpu = tmpdata[0];
            mem = tmpdata[2];
        }
        else
        {
            cpu = tmpdata[1];
            mem = tmpdata[3];
        }
    }
    else //单节点
    {
        
        tmpdata[0] = _left_cpu_A ;
        tmpdata[1] = _left_cpu_B ;
        tmpdata[2] = _left_mem_A ;
        tmpdata[3] = _left_mem_B ;
        bool A = false, B = false;
        if (_left_cpu_A >= vm_cpu && _left_mem_A >= vm_mem)
        {
            A = true;
        }
        if (_left_cpu_B >= vm_cpu && _left_mem_B >= vm_mem)
        {
            B = true;
        }
        if (A && B)
        {
            if (_left_cpu_A + _left_mem_A >= _left_cpu_B + _left_mem_B) //A的容量更多
            {
                B = false;
            }
            else
            {
                A = false;
            }
        }
        if (!A)
        {
            tmpdata[1] = _left_cpu_B - vm_cpu;
            cpu = tmpdata[1];
            tmpdata[3] = _left_mem_B - vm_mem;
            mem = tmpdata[3];
        }
        else
        {
            tmpdata[0] = _left_cpu_A - vm_cpu;
            cpu = tmpdata[0];
            tmpdata[2] = _left_mem_A - vm_mem;
            mem = tmpdata[2];
        }
    }

    double noload = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        noload += tmpdata[i];
    }
    return noload / (m_num_of_cpu + m_size_of_mem);
}

bool Host::addVMs(vector<shared_ptr<VirtualMachine>>& vms) 
{
    int &cpuA = _left_cpu_A, &memA = _left_mem_A;
    int &cpuB = _left_cpu_B, &memB = _left_mem_B;
    const int vmssize = vms.size();
    _vms.reserve(vms.size()+_vms.size());
    // _vms.rehash(vmssize+_vms.size());
    for (size_t i = 0; i < vmssize; i++)
    {
        // cout << vms[i]->getId()<< endl;
        int id = vms[i]->getId();
        shared_ptr<VirtualMachine> vm = vms[i];
        _vms[id] = vm;
        
        const int cpu = vm->cpu;
        const int mem = vm->mem;        
        bool is_double = vm->IsDoubleNode() == 0;
        if (is_double)
        {
            cpuA -= cpu / 2;
            cpuB -= cpu / 2;
            memA -= mem / 2;
            memB -= mem / 2;
        }
        else
        {
            if (abs(cpuA - cpu - cpuB) + abs(memA - mem - memB) <= abs(cpuB - cpu - cpuA) + abs(memB - mem - memA) )
            {
                vm->setNode(true);
                cpuA -= cpu;
                memA -= mem;
            }
            else
            {
                vm->setNode(false);
                cpuB -= cpu;
                memB -= mem;
            }
        }
    } 
    return true;  
}
void Host::getAbnormalCapcity(int &small, int &large) const
{
    int tmpdata[4] = {_left_cpu_A, _left_cpu_B, _left_mem_A, _left_mem_B};
    small = tmpdata[0];
    for (size_t i = 1; i < 4; i++)
    {
        small = min(small, tmpdata[i]);
    }
    large = small == tmpdata[0] ? tmpdata[2] : large;
    large = small == tmpdata[2] ? tmpdata[0] : large;
    large = small == tmpdata[1] ? tmpdata[3] : large;
    large = small == tmpdata[3] ? tmpdata[1] : large;
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
    const int cpu = vm->cpu; const int mem = vm->mem;
    if (vm->IsDoubleNode() == 0)
    {
        // cout << "vm: " << vm->getNumOfCpu() << '\t' << vm->getSizeOfMem()<< endl;
        // print();
        if (getAvailableCpu(true) >= (cpu / 2) &&
            getAvailableMem(true) >= (mem / 2))
        {
            /* code */
            _left_cpu_A -= (cpu / 2);
            _left_cpu_B -= (cpu / 2);
            _left_mem_A -= (mem / 2);
            _left_mem_B -= (mem / 2);
            // print();
            success = true;
        }
    }
    else
    {
        bool A = false, B = false;
        if (_left_cpu_A >= cpu && _left_mem_A >= mem)
        {
            A = true;
        }
        if (_left_cpu_B >= cpu && _left_mem_B >= mem)
        {
            B = true;
        }
        success = true;
        if (A && B)
        {
            if (_left_cpu_A + _left_mem_A >= _left_cpu_B + _left_mem_B)
            {
                vm->setNode(true);
                _left_cpu_A -= cpu;
                _left_mem_A -= mem;
            }
            else
            {
                vm->setNode(false);
                _left_cpu_B -= cpu;
                _left_mem_B -= mem;
            }
        }
        else if (!A && !B)
        {
            success = false;
        }
        else if (!A)
        {
            vm->setNode(false);
            _left_cpu_B -= cpu;
            _left_mem_B -= mem;
            // if (_left_cpu_B < 0 || _left_mem_B < 0)
            //     dcout << "overflow\n";
        }
        else
        {
            vm->setNode(true);
            _left_cpu_A -= cpu;
            _left_mem_A -= mem;
            // if (_left_cpu_A < 0 || _left_mem_A < 0)
            //     dcout << "overflow\n";
        }
    }
    if (success)
        _vms[vm->m_index] = vm;
    return success;
}

bool Host::addVM_try_migrate(shared_ptr<VirtualMachine> &vm)
{
    bool success = false;
    int mem = vm->mem, cpu = vm->cpu;
    if (vm->IsDoubleNode() == 0)
    {
        // cout << "vm: " << cpu << '\t' << mem<< endl;
        // print();
        if (getAvailableCpu(true) >= (cpu / 2) &&
            getAvailableMem(true) >= (mem / 2))
        {
            /* code */
            vm->getHost()->deleteVM(vm->getId());
            _left_cpu_A -= (cpu / 2);
            _left_cpu_B -= (cpu / 2);
            _left_mem_A -= (mem / 2);
            _left_mem_B -= (mem / 2);
            // print();
            success = true;
        }
    }
    else
    {
    
        bool A = false, B = false;
        if (_left_cpu_A >= cpu && _left_mem_A >= mem)
        {
            A = true;
        }
        if (_left_cpu_B >= cpu && _left_mem_B >= mem)
        {
            B = true;
        }
        success = true;
        if (A && B)
        {
            vm->getHost()->deleteVM(vm->getId());
            if (_left_cpu_A + _left_mem_A >= _left_cpu_B + _left_mem_B)
            {
                vm->setNode(true);
                _left_cpu_A -= cpu;
                _left_mem_A -= mem;
            }
            else
            {
                vm->setNode(false);
                _left_cpu_B -= cpu;
                _left_mem_B -= mem;
            }
        }
        else if (!A && !B)
        {
            success = false;
        }
        else if (!A)
        {
            vm->getHost()->deleteVM(vm->getId());
            vm->setNode(false);
            _left_cpu_B -= cpu;
            _left_mem_B -= mem;
            // if (_left_cpu_B < 0 || _left_mem_B < 0)
            //     dcout << "overflow\n";
        }
        else
        {
            vm->getHost()->deleteVM(vm->getId());
            vm->setNode(true);
            _left_cpu_A -= cpu;
            _left_mem_A -= mem;
            // if (_left_cpu_A < 0 || _left_mem_A < 0)
            //     dcout << "overflow\n";
        }
    }
    if (success)
        _vms[vm->getId()] = vm;
    return success;
}
bool Host::addVM_opt(shared_ptr<VirtualMachine>& vm, char& Node){
    if(Node == 'D'){
        _left_cpu_A -= (vm->getNumOfCpu() / 2);
        _left_cpu_B -= (vm->getNumOfCpu() / 2);
        _left_mem_A -= (vm->getSizeOfMem() / 2);
        _left_mem_B -= (vm->getSizeOfMem() / 2);
        _vms[vm->getId()] = vm;
        return true;
    }
    else if(Node == 'A'){
        _left_cpu_A -= vm->getNumOfCpu();
        _left_mem_A -= vm->getSizeOfMem();
        _vms[vm->getId()] = vm;
        vm->setNode(true);
        return true;
    }
    else if(Node == 'B'){
        _left_cpu_B -= vm->getNumOfCpu();
        _left_cpu_B -= vm->getSizeOfMem();
        _vms[vm->getId()] = vm;
        vm->setNode(false);
        return true;
    }
    return false;
}

int Host::getAvailableCpu(bool isDouble){
    if(isDouble)    return min(_left_cpu_A, _left_cpu_B);
    else return max(_left_cpu_A, _left_cpu_B);
}

int Host::getAvailableMem(bool isDouble){
    if(isDouble)    return min(_left_mem_A, _left_mem_B);
    else    return max(_left_mem_A, _left_mem_B);
}

int Host::getAvailableCpuA(){
    return _left_cpu_A;
}
int Host::getAvailableCpuB(){
    return _left_cpu_B;
}
int Host::getAvailableMemA(){
    return _left_mem_A;
}
int Host::getAvailableMemB(){
    return _left_mem_B;
}

bool Host::isFree(){
    if(_left_cpu_A + _left_cpu_B == m_num_of_cpu)   return true;
    return false;
}

void Host::print() const
{
//     cout << "left cpu :" << _left_cpu_A << '\t' << _left_cpu_B << endl;
//     cout << "left mem :" << _left_mem_A << '\t' << _left_mem_B << endl;
}
bool Host::checkMyself() const
{
    int right = true;
    int cpuA = 0, cpuB = 0, memA = 0, memB = 0; 
    for (auto &i : _vms)
    {
        // if (i.second->getHost()->getIndex()!= m_index)
        // {
        //     cout << "vm host not match" << endl;
        //     right = false;
        // }
        
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
            right = false;
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
        right = false;
    }
    return right;
}

unordered_map<int, shared_ptr<VirtualMachine>> & Host::get_vms(){
    return _vms;
}
//     switch (Node)
//     {
//     case 'D':
//         return _left_cpu_A + _left_cpu_B + _left_mem_A + _left_mem_B - vm_sum;
//         break;
//     case 'A':
    
//     default:
//         break;
//     }
// }
bool Host::isAbleToAddVM(shared_ptr<VirtualMachine> &vm) 
{
    bool success = false;
    if (vm->IsDoubleNode() == 0)
    {
        if (getAvailableCpu(true) >= (vm->cpu / 2) &&
            getAvailableMem(true) >= (vm->mem / 2))
        {
            success = true;
        }
    }
    else
    {
        int mem = vm->mem, cpu = vm->cpu;
        bool A = false, B = false;
        if (_left_cpu_A >= cpu && _left_mem_A >= mem)
        {
            A = true;
        }
        if (_left_cpu_B >= cpu && _left_mem_B >= mem)
        {
            B = true;
        }
        if (A || B)
        {
            success = true;
        }
    }
    return success;
}

int Host::getVmNum() const
{
    return _vms.size();
}

double Host::getNoLoadRatio() const
{
    return (_left_cpu_A + _left_cpu_B + _left_mem_A + _left_mem_B) / (double)(m_num_of_cpu + m_size_of_mem);
}

bool Host::addVMs_try(vector<shared_ptr<VirtualMachine>>& vms)
{
    int cpuA = _left_cpu_A, memA = _left_mem_A;
    int cpuB = _left_cpu_B, memB = _left_mem_B;
    const int vmssize = vms.size();
//    _vms.reserve(vms.size()+_vms.size());
    // _vms.rehash(vmssize+_vms.size());
    for (size_t i = 0; i < vmssize; i++)
    {
        // cout << vms[i]->getId()<< endl;
        int id = vms[i]->getId();
        shared_ptr<VirtualMachine> vm = vms[i];
        const int cpu = vm->cpu;
        const int mem = vm->mem;
        bool is_double = vm->IsDoubleNode() == 0;
        if (is_double)
        {
            cpuA -= cpu / 2;
            cpuB -= cpu / 2;
            memA -= mem / 2;
            memB -= mem / 2;
        }
        else
        {
            if (abs(cpuA - cpu - cpuB) + abs(memA - mem - memB) <= abs(cpuB - cpu - cpuA) + abs(memB - mem - memA) )
            {
                cpuA -= cpu;
                memA -= mem;
            }
            else
            {
                cpuB -= cpu;
                memB -= mem;
            }
        }
    }
    if(cpuA >= 0 && cpuB >=0 && memA >=0 && memB >=0)
        return true;
    else{
        return false;
    }
}