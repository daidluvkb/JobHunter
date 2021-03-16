#include "Scheduler.h"
Scheduler::Scheduler(/* args */)
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::deletVM(vector<int> &vms) 
{
    for (int i = 0; i < vms.size(); i++)
        deleteVM(vms[i]);
}

void Scheduler::addVM(vector<shared_ptr<VirtualMachine>> &vms) 
{
    for (size_t i = 0; i < vms.size(); i++)
    {
        int id = vms[i]->getId();
        _vms.insert(make_pair(id, vms[i]));
        addVM(vms[i]);
    }    
}

void Scheduler::deleteVM(const int id)
{
    auto itr = _vms.find(id);
    if (itr != _vms.end())
    {
        shared_ptr<Host> host = itr->second->getHost();
        host->deleteVM(id);
        _vms.erase(itr);
        return;
    }
}

void Scheduler::addVM(shared_ptr<VirtualMachine>& vm) 
{

    bool is_double = vm->IsDoubleNode();
    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (_busy_host[i]->getAvailableCpu(is_double) > vm->getNumOfCpu() &&
            _busy_host[i]->getAvailableMem(is_double) > vm->getSizeOfMem())
        {
            vm->setHost(_busy_host[i]);
            _busy_host[i]->addVM(vm);
            return;
        }        
    }

    if(_free_host.size())//
    {
        auto host = _free_host[_free_host.size() - 1];
        _busy_host.push_back(host);
        host->addVM(vm);
        vm->setHost(host);
        _free_host.pop_back();
        return;
    }
    buyAHost(vm->getNumOfCpu(), vm->getSizeOfMem());
}

void Scheduler::sumRequest(int &cpu, int &mem, const vector<shared_ptr<VirtualMachine>> &request) 
{
    cpu = 0;
    mem = 0;
    for(auto & req: request)
    {
        cpu += req->getNumOfCpu();
        mem += req->getSizeOfMem();
    }
}
