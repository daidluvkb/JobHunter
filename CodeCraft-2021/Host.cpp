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

        smallest = min(smallest, _left_cpu_A - (vm->cpu / 2));
        smallest = min(smallest, _left_cpu_B - (vm->cpu / 2));
        smallest = min(smallest, _left_mem_A - (vm->mem / 2));
        smallest = min(smallest, _left_mem_B - (vm->mem / 2));

    }
    else{
        if(_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->cpu){

            smallest = min(smallest, _left_cpu_A - vm->cpu);
            smallest = min(smallest, _left_mem_A - vm->mem);
        }
        else if(_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->cpu){
       
            smallest = min(smallest, _left_cpu_B - vm->cpu);
            smallest = min(smallest, _left_mem_B - vm->mem);
        }        
    }
    return smallest;
}

int Host::getLargestCapacityAfterAdd(const shared_ptr<VirtualMachine>& vm) const
{
    int largest = 0;
    if (vm->IsDoubleNode() == 0)
    {

        largest = max(largest, _left_cpu_A - (vm->cpu / 2));
        largest = max(largest, _left_cpu_B - (vm->cpu / 2));
        largest = max(largest, _left_mem_A - (vm->mem / 2));
        largest = max(largest, _left_mem_B - (vm->mem / 2));
    }
    else
    {
        if (_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->cpu)
        {

            largest = max(largest, _left_cpu_A - vm->cpu);
            largest = max(largest, _left_mem_A - vm->mem);
        }
        else if (_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->cpu)
        {

            largest = max(largest, _left_cpu_B - vm->cpu);
            largest = max(largest, _left_mem_B - vm->mem);
        }
    }
    return largest;
}



void Host::getAbnormalCapcityAfterAdd(const shared_ptr<VirtualMachine> &vm, int &small, int &large) const
//计算add之后是否会把host置于一个不平衡的状态：
//如果会让host更不平衡，就记录这个不平衡程度
//如果让host某个节点往平衡的方向变化了，那就返回变化节点的平衡程度
//平衡程度：结点余量（cpu/mem）最小值和对应（mem/cpu）最大值
{
    int tmpdata[4];
    if (vm->IsDoubleNode() == 0)
    {
        tmpdata[0] = _left_cpu_A - (vm->cpu / 2);
        tmpdata[1] = _left_cpu_B - (vm->cpu / 2);
        tmpdata[2] = _left_mem_A - (vm->mem / 2);
        tmpdata[3] = _left_mem_B - (vm->mem / 2);    
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
        if (A && B)
        {
            if (_left_cpu_A >= _left_cpu_B)
            {
                tmpdata[0] = _left_cpu_A - vm->cpu;
                tmpdata[1] = _left_cpu_B;
                tmpdata[2] = _left_mem_A - vm->mem;
                tmpdata[3] = _left_mem_B;

            }
            else
            {
                tmpdata[0] = _left_cpu_A;
                tmpdata[1] = _left_cpu_B - vm->cpu;
                tmpdata[2] = _left_mem_A;
                tmpdata[3] = _left_mem_B - vm->mem;
            }
        }
        else if (!A && !B)
        {
            small = INT8_MAX;
            large = 0;
            return;
        }
        else if (!A)
        {
            tmpdata[0] = _left_cpu_A;
            tmpdata[1] = _left_cpu_B - vm->cpu;
            tmpdata[2] = _left_mem_A;
            tmpdata[3] = _left_mem_B - vm->mem;
            if (_left_cpu_B < 0 || _left_mem_B < 0)
                dcout << "overflow\n";
        }
        else
        {
            tmpdata[0] = _left_cpu_A - vm->cpu;
            tmpdata[1] = _left_cpu_B;
            tmpdata[2] = _left_mem_A - vm->mem;
            tmpdata[3] = _left_mem_B;
            if (_left_cpu_A < 0 || _left_mem_A < 0)
                dcout << "overflow\n";
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
    const int cpu = _vms[id]->cpu;
    const int mem = _vms[id]->mem;
    int node = _vms[id]->IsDoubleNode();
    if(node == 0){
        _left_cpu_A += (cpu / 2);
        _left_cpu_B += (cpu / 2);
        _left_mem_A += (mem / 2);
        _left_mem_B += (mem / 2);
    }
    else if(node == 1){
        _left_cpu_A += cpu;
        _left_mem_A += mem;
    }
    else if(node == 2){
        _left_cpu_B += cpu;
        _left_mem_B += mem;
    }

    _vms.erase(id);

    return;
}

int Host::getIndex() 
{
    return m_index;
}

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


bool Host::addVM(shared_ptr<VirtualMachine>& vm){
    
    bool success = false;
    if(vm->IsDoubleNode() == 0){
        // cout << "vm: " << vm->cpu << '\t' << vm->mem<< endl;
        // print();
        _left_cpu_A -= (vm->cpu / 2);
        _left_cpu_B -= (vm->cpu / 2);
        _left_mem_A -= (vm->mem / 2);
        _left_mem_B -= (vm->mem / 2);
        // print();
        success = true;
    }
    else{
        if(_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->cpu){
            vm->setNode(true);
            _left_cpu_A -= vm->cpu;
            _left_mem_A -= vm->mem;
            success = true;
        }
        else if(_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->cpu){
            vm->setNode(false);
            _left_cpu_B -= vm->cpu;
            _left_mem_B -= vm->mem;
            success = true;
        }        
    }
    if(success)
        _vms[vm->getId()] = vm;
    return success;
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

bool Host::addVM_try(shared_ptr<VirtualMachine> &vm)
{
    bool success = false;
    if (vm->IsDoubleNode() == 0)
    {
        // cout << "vm: " << vm->cpu << '\t' << vm->mem<< endl;
        // print();
        if (getAvailableCpu(true) >= (vm->cpu / 2) &&
            getAvailableMem(true) >= (vm->mem / 2))
        {
            /* code */
            _left_cpu_A -= (vm->cpu / 2);
            _left_cpu_B -= (vm->cpu / 2);
            _left_mem_A -= (vm->mem / 2);
            _left_mem_B -= (vm->mem / 2);
            // print();
            success = true;
        }
    }
    else
    {
        // if (getAvailableCpu(false) >= vm->cpu &&
        //     getAvailableMem(false) >= vm->mem)
        // {
        //     if (_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->cpu)
        //     {
        //         vm->setNode(true);
        //         _left_cpu_A -= vm->cpu;
        //         _left_mem_A -= vm->mem;
        //         if (_left_cpu_A < 0 || _left_mem_A < 0)
        //             cout << "overflow\n";
        //         success = true;
        //     }

        //     else if (_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->cpu)
        //     {
        //         vm->setNode(false);
        //         _left_cpu_B -= vm->cpu;
        //         _left_mem_B -= vm->mem;
        //         if (_left_cpu_B < 0 || _left_mem_B < 0)
        //             cout << "overflow\n";
        //         success = true;
        //     }
        //     else 
        //     {
        //         // cout << "else" << endl;
        //     }
        // }
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
        success = true;
        if (A && B)
        {
            if (abs(_left_cpu_A - cpu - _left_cpu_B) + abs(_left_mem_A - mem - _left_mem_B) <=
                abs(_left_cpu_B - cpu - _left_cpu_A) + abs(_left_mem_B - mem - _left_mem_A))
            {
                vm->setNode(true);
                _left_cpu_A -= vm->cpu;
                _left_mem_A -= vm->mem;
            }
            else
            {
                vm->setNode(false);
                _left_cpu_B -= vm->cpu;
                _left_mem_B -= vm->mem;
            }
        }
        else if (!A && !B)
        {
            success = false;
        }
        else if (!A)
        {
            vm->setNode(false);
            _left_cpu_B -= vm->cpu;
            _left_mem_B -= vm->mem;
            if (_left_cpu_B < 0 || _left_mem_B < 0)
                dcout << "overflow\n";
        }
        else
        {
            vm->setNode(true);
            _left_cpu_A -= vm->cpu;
            _left_mem_A -= vm->mem;
            if (_left_cpu_A < 0 || _left_mem_A < 0)
                dcout << "overflow\n";
        }
    }
    if (success)
        _vms[vm->getId()] = vm;
    return success;
}

bool Host::addVM_try_migrate(shared_ptr<VirtualMachine> &vm)
{
    bool success = false;
    if (vm->IsDoubleNode() == 0)
    {
        // cout << "vm: " << vm->cpu << '\t' << vm->mem<< endl;
        // print();
        if (getAvailableCpu(true) >= (vm->cpu / 2) &&
            getAvailableMem(true) >= (vm->mem / 2))
        {
            /* code */
            vm->getHost()->deleteVM(vm->getId());
            _left_cpu_A -= (vm->cpu / 2);
            _left_cpu_B -= (vm->cpu / 2);
            _left_mem_A -= (vm->mem / 2);
            _left_mem_B -= (vm->mem / 2);
            // print();
            success = true;
        }
    }
    else
    {
        // if (getAvailableCpu(false) >= vm->cpu &&
        //     getAvailableMem(false) >= vm->mem)
        // {
        //     if (_left_mem_A >= _left_mem_B && _left_cpu_A >= vm->cpu)
        //     {
        //         vm->setNode(true);
        //         _left_cpu_A -= vm->cpu;
        //         _left_mem_A -= vm->mem;
        //         if (_left_cpu_A < 0 || _left_mem_A < 0)
        //             cout << "overflow\n";
        //         success = true;
        //     }

        //     else if (_left_mem_B >= _left_mem_A && _left_cpu_B >= vm->cpu)
        //     {
        //         vm->setNode(false);
        //         _left_cpu_B -= vm->cpu;
        //         _left_mem_B -= vm->mem;
        //         if (_left_cpu_B < 0 || _left_mem_B < 0)
        //             cout << "overflow\n";
        //         success = true;
        //     }
        //     else
        //     {
        //         // cout << "else" << endl;
        //     }
        // }
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
        success = true;
        if (A && B)
        {
            vm->getHost()->deleteVM(vm->getId());
            if (_left_cpu_A >= _left_cpu_B)
            {
                vm->setNode(true);
                _left_cpu_A -= vm->cpu;
                _left_mem_A -= vm->mem;
            }
            else
            {
                vm->setNode(false);
                _left_cpu_B -= vm->cpu;
                _left_mem_B -= vm->mem;
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
            _left_cpu_B -= vm->cpu;
            _left_mem_B -= vm->mem;
            if (_left_cpu_B < 0 || _left_mem_B < 0)
                dcout << "overflow\n";
        }
        else
        {
            vm->getHost()->deleteVM(vm->getId());
            vm->setNode(true);
            _left_cpu_A -= vm->cpu;
            _left_mem_A -= vm->mem;
            if (_left_cpu_A < 0 || _left_mem_A < 0)
                dcout << "overflow\n";
        }
    }
    if (success)
        _vms[vm->getId()] = vm;
    return success;
}
bool Host::addVM_opt(shared_ptr<VirtualMachine>& vm, char& Node){
    if(Node == 'D'){
        _left_cpu_A -= (vm->cpu / 2);
        _left_cpu_B -= (vm->cpu / 2);
        _left_mem_A -= (vm->mem / 2);
        _left_mem_B -= (vm->mem / 2);
        _vms[vm->getId()] = vm;
        return true;
    }
    else if(Node == 'A'){
        _left_cpu_A -= vm->cpu;
        _left_mem_A -= vm->mem;
        _vms[vm->getId()] = vm;
        vm->setNode(true);
        return true;
    }
    else if(Node == 'B'){
        _left_cpu_B -= vm->cpu;
        _left_cpu_B -= vm->mem;
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
    dcout << "left cpu :" << _left_cpu_A << '\t' << _left_cpu_B << endl;
    dcout << "left mem :" << _left_mem_A << '\t' << _left_mem_B << endl;
}
bool Host::checkMyself() const
{
    int right = true;
    int cpuA = 0, cpuB = 0, memA = 0, memB = 0; 
    for (auto &i : _vms)
    {
        if (i.second->getHost()->getIndex()!= m_index)
        {
            dcout << "vm host not match" << endl;
            right = false;
        }
        
        int cpu = i.second->cpu, mem = i.second->mem;
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
        dcout << "used cpu: " << cpuA << '\t' << cpuB << endl;
        dcout << "used mem: " << memA << '\t' << memB << endl;
        print();
        dcout << "sum: " << m_num_of_cpu << '\t' << m_size_of_mem << endl;
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