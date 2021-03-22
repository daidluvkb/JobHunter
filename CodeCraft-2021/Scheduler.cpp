#include "Scheduler.h"
#include <float.h>
#include <cmath>

Scheduler::Scheduler(/* args */)
{
   cost = 0;
   _today_add_arrangement_num = 0;
}

Scheduler::~Scheduler()
{
}

void Scheduler::setHostCandidates(unordered_map<string, HostInfo> &hostInfos) 
{
    if(_host_candidates.size())
        return;
    _host_cheapest = 0;
    _host_candidates.reserve(hostInfos.size());
    for (auto itr = hostInfos.begin(); itr != hostInfos.end(); itr++)
    {
        _host_candidates.emplace_back(itr->second);
    }
    for (size_t i = 0; i < _host_candidates.size() - 1; i++)
    {
        for (size_t j = i + 1; j < _host_candidates.size(); j++)
        {
            if(_host_candidates[i].basicCost > _host_candidates[j].basicCost)
            {
                swap(_host_candidates[i], _host_candidates[j]);
            }
        }
        
    }
    dcout << "sorted hosts candidate according to basic cost"<< endl;
}


void Scheduler::deleteVM(vector<int> &vms) 
{
    for (int i = 0; i < vms.size(); i++)
        deleteVM(vms[i]);
}

void Scheduler::addVM_opt(vector<shared_ptr<VirtualMachine>> &vms)
{
    for (size_t i = 0; i < vms.size(); i++)
    {
        int id = vms[i]->getId();
        _vms.insert(make_pair(id, vms[i]));
        addVM_opt(vms[i]);
        if (!vms[i]->IsDoubleNode())
            _today_add_arrangement << "(" << vms[i]->getHost()->getIndex() << ")\n";
        else
            _today_add_arrangement << "(" << vms[i]->getHost()->getIndex() << ", " << vms[i]->getNode() << ")\n";
        _today_add_arrangement_num++;
    }
}

void Scheduler::addVM(vector<shared_ptr<VirtualMachine>> &vms) 
{
    for (size_t i = 0; i < vms.size(); i++)
    {
        int id = vms[i]->getId();
        _vms.insert(make_pair(id, vms[i]));
        addVM(vms[i]);
        if (!vms[i]->IsDoubleNode())
            _today_add_arrangement << "(" << vms[i]->getHost()->getIndex() << ")\n";
        else
            _today_add_arrangement << "(" << vms[i]->getHost()->getIndex() << ", " << vms[i]->getNode() << ")\n";
        _today_add_arrangement_num ++;
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
        if (host->isFree())
        {
            auto i = _busy_host.begin();
            for (; i != _busy_host.end(); i++)
            {
                if ((*i)->getIndex() == host->getIndex())
                {
                    _busy_host.erase(i);
                    break;
                }
            }
            if (i == _busy_host.end())
            {                
                _addHostToFree(host);
            }
            else
            {
                return;
            }            
        }
        return;
    }
    return;
}

void Scheduler::addVM_opt(shared_ptr<VirtualMachine>& vm){
    bool is_double = !vm->IsDoubleNode();
    double vm_mem_to_cpu = vm->getSizeOfMem() / vm->getNumOfCpu();
    double min_Host_mem_to_cpu = DBL_MAX;
    int index = 0;
    char Node = 'D';
    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (is_double)
        {
            if (_busy_host[i]->getAvailableCpu(true) >= (vm->getNumOfCpu() / 2) &&
                _busy_host[i]->getAvailableMem(true) >= (vm->getSizeOfMem() / 2))
            {
                if(abs((_busy_host[i]->getAvailableMem(true) / _busy_host[i]->getAvailableCpu(true)) - vm_mem_to_cpu)
                 < abs(min_Host_mem_to_cpu - vm_mem_to_cpu)){
                    min_Host_mem_to_cpu = _busy_host[i]->getAvailableMem(true) / _busy_host[i]->getAvailableCpu(true);
                    index = i;
                }
            }
        }
        else
        {
            if (_busy_host[i]->getAvailableCpuA() >= vm->getNumOfCpu() &&
                _busy_host[i]->getAvailableMemA() >= vm->getSizeOfMem()){
                    if(abs((_busy_host[i]->getAvailableMemA() / _busy_host[i]->getAvailableCpuA()) - vm_mem_to_cpu)
                     < abs(min_Host_mem_to_cpu - vm_mem_to_cpu)){
                        min_Host_mem_to_cpu = _busy_host[i]->getAvailableMemA() / _busy_host[i]->getAvailableCpuA();
                        index = i;
                        Node = 'A';
                }
            }
            if(_busy_host[i]->getAvailableCpuB() > vm->getNumOfCpu() &&
               _busy_host[i]->getAvailableMemB() > vm->getSizeOfMem()){
                    if(abs((_busy_host[i]->getAvailableMemB() / _busy_host[i]->getAvailableCpuB()) - vm_mem_to_cpu)
                     < abs(min_Host_mem_to_cpu - vm_mem_to_cpu)){
                        min_Host_mem_to_cpu = _busy_host[i]->getAvailableMemB() / _busy_host[i]->getAvailableCpuB();
                        index = i;
                        Node = 'B';

                   }
               }

        }
        if(min_Host_mem_to_cpu != DBL_MAX){
            _busy_host[index]->addVM_opt(vm, Node);
            vm->setHost(_busy_host[index]);
        }
    }
    for (auto i = _free_host.begin(); i != _free_host.end(); i++)
    {
        if ((*i)->addVM_try(vm))
        {
            _busy_host.emplace_back(*i);
            vm->setHost(*i);
            _free_host.erase(i);
            return;
        }
        
    }
    shared_ptr<const HostInfo> host;
    if (is_double)
        host = chooseAHost(vm->getNumOfCpu(), vm->getSizeOfMem());
    else
    {
        host = chooseAHost(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
    }
    if (host)
    {
        auto hst = _buyAHost_immedidate(*host);
        if (hst->addVM_try(vm))
        {
            _busy_host.emplace_back(hst);
            vm->setHost(hst);
        }
        else
        {
            dcout << "host cant contain vm??\n";
        }

    } //不太安全但好像逻辑安全

    // host->addVM(vm);
    // vm->setHost(host);
    // _free_host.pop_back();
    return;    

}

void Scheduler::addVM(shared_ptr<VirtualMachine>& vm) 
{

    bool is_double = !vm->IsDoubleNode();
    /*
    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (_busy_host[i]->getAvailableCpu(is_double) > vm->getNumOfCpu() &&
            _busy_host[i]->getAvailableMem(is_double) > vm->getSizeOfMem())
        {
            dcout << "_busy_host:" << _busy_host.size() << endl;
            dcout << _busy_host[i]->getAvailableCpu(is_double) << '\t' << vm->getNumOfCpu() << endl;
            dcout << _busy_host[i]->getAvailableMem(is_double) << '\t' << vm->getSizeOfMem() << endl;
            _busy_host[i]->addVM(vm);
            vm->setHost(_busy_host[i]);
            return;
        }
    }
    */

    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (_busy_host[i]->addVM_try(vm))
        {            
            vm->setHost(_busy_host[i]);
            return ;
        }
        
        // if (is_double)
        // {
        //     if (_busy_host[i]->getAvailableCpu(true) >= (vm->getNumOfCpu() / 2) &&
        //         _busy_host[i]->getAvailableMem(true) >= (vm->getSizeOfMem() / 2))
        //     {
        //         _busy_host[i]->addVM(vm);
        //         vm->setHost(_busy_host[i]);
        //         return;
        //     }
        // }
        // else
        // {
        //     if (_busy_host[i]->getAvailableCpu(false) >= vm->getNumOfCpu() &&
        //         _busy_host[i]->getAvailableMem(false) >= vm->getSizeOfMem())
        //         if (_busy_host[i]->addVM(vm))
        //         {
        //             vm->setHost(_busy_host[i]);
        //             return;
        //         }
        // }
    }

    // if (!_free_host.size()) //
    // {
    //     shared_ptr<const HostInfo> host;
    //     if(is_double)
    //         host = chooseAHost(vm->getNumOfCpu(), vm->getSizeOfMem());
    //     else
    //     {
    //         host = chooseAHost(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
    //     }

    //     if (host)
    //     {
    //         _buyAHost_opt(*host);
    //         //record buying action
    //     }
    //     else
    //     {
    //         buyHosts(vm->getNumOfCpu(), vm->getSizeOfMem());//host column must be bigger than vm require
    //     }
    // }

    //auto host = _free_host[_free_host.size() - 1];//
    for (auto i = _free_host.begin(); i != _free_host.end(); i++)
    {
        if ((*i)->addVM_try(vm))
        {
            _busy_host.emplace_back(*i);
            vm->setHost(*i);
            _free_host.erase(i);
            return;
        }
        
    }
    shared_ptr<const HostInfo> host;
    if (is_double)
        host = chooseAHost(vm->getNumOfCpu(), vm->getSizeOfMem());
    else
    {
        host = chooseAHost(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
    }
    if (host)
    {
        auto hst = _buyAHost_immedidate(*host);
        if (hst->addVM_try(vm))
        {
            _busy_host.emplace_back(hst);
            vm->setHost(hst);
        }
        else
        {
            dcout << "host cant contain vm??\n";
        }

    } //不太安全但好像逻辑安全

    // host->addVM(vm);
    // vm->setHost(host);
    // _free_host.pop_back();
    return;
}

void Scheduler::addVM_bystep(shared_ptr<VirtualMachine> &vm)
{
    int id = vm->getId();
    _vms.insert(make_pair(id, vm));
    addVM_opt(vm);

    if (!vm->getHost()->checkMyself())
    {
        dcout << "Host wrong" << endl;
    }
}

void Scheduler::shutHost(shared_ptr<Host> &host)
{
    _free_host.emplace_back(host);
}


/**
 * describe: only make a choice, dont buy(record in everyday purchased list)
*/

void Scheduler::buyHosts(const int cpu, const int mem) 
{
    if(!_host_candidates.size())
    {
        dcout << "has no host candidate , please check input hostInfos" << endl;
        return;
    }
    const HostInfo &chosen_host = _host_candidates[0];
    int num = chosen_host.cpu / cpu + 1;
    num = max(num, chosen_host.mem / mem + 1);
    for (size_t i = 0; i < num; i++)
    {
        // _hosts.emplace_back(chosen_host);
        // _free_host.emplace_back(make_shared<Host>(chosen_host, _hosts.size() - 1));
        // _new_purchased_hosts.emplace_back(chosen_host);
        _buyAHost(chosen_host);
    }
}

void Scheduler::declareANewDay() 
{
    _host_num_lastday = _hosts.size();
    for (auto &i : _free_host)
    {
        if (!i->isFree())
        {
            dcout << "free hosts wrong\n";
        }        
    }
    
    int vmsnum = 0;
    for (size_t i = 0; i < _hosts.size(); i++)
    {
        _hosts[i]->checkMyself();
        vmsnum += _hosts[i]->getNumOfVM();
        if (_hosts[i]->getIndex() != i)
        {
            dcout << "host index wrong\n";
        }        
    }
    if (vmsnum != _vms.size())
    {
        dcout << "vm num wrong\n";
    }
    
    getTodayDailyCost();
    _today_purchased_hosts.clear();
    // _today_add_arrangement.clear();
    _today_add_arrangement.str("");
    _today_add_arrangement_num = 0;
    // cout << "today arrange: " << _today_add_arrangement.str();
    // fflush(stdout);
}

int Scheduler::getTodayAddVMArrangment(vector<shared_ptr<VirtualMachine>>&vms) 
{
    for (size_t i = 0; i < vms.size(); i++)
    {
        auto& vm = vms[i];
        stringstream debugcheck; //ljm
        if (!vm->IsDoubleNode())
        {
            _today_add_arrangement << "(" << vm->getHost()->getIndex() << ")\n";
            debugcheck << "(" << vm->getHost()->getIndex() << ")\n";
        }
        else
        {
            _today_add_arrangement << "(" << vm->getHost()->getIndex() << ", " << vm->getNode() << ")\n";
            debugcheck << "(" << vm->getHost()->getIndex() << ", " << vm->getNode() << ")\n";
        }
        string stringcheck = debugcheck.str();
        _today_add_arrangement_num++;
    }
    printf("%s", _today_add_arrangement.str().c_str());
    fflush(stdout);
    return _today_add_arrangement_num;
}

vector<shared_ptr<const HostInfo>> Scheduler::getNewPurchasedHosts() 
{
    int today_purchased_num = 0;
    printf("(purchase, %d)\n", _today_purchased_hosts.size());  
      
    for (auto i : _today_purchased_hosts)
    {
        printf("(%s, %d)\n", i.first.c_str(), i.second);
        fflush(stdout);
        int cnt = i.second;
        for (size_t j = _host_num_lastday; j < _hosts.size(); j++)
        {
            if (_hosts[j]->getType() == i.first)
            {
                _hosts[j]->setIndex(today_purchased_num + _host_num_lastday);
                today_purchased_num++;
                if (cnt--)
                {
                    continue;
                }
                else
                {
                    break;
                }                
            }            
        }        
    }    
    return vector<shared_ptr<const HostInfo>>();
}

void Scheduler::getTodayMigration() 
{
    printf( "(migration, 0)\n");
    fflush(stdout);
    // cout << "(migration, 0)"<< endl;
}
/**
 * describe: only make a choice, dont buy(record in everyday purchased list)
*/
shared_ptr<const HostInfo> Scheduler::chooseAHost(const int cpu, const int mem) 
{
    for (size_t i = 0; i < _host_candidates.size(); i++)
    {
        if (_host_candidates[i].cpu >= cpu && _host_candidates[i].mem >= mem)
        {
            return make_shared<HostInfo>(_host_candidates[i]);
        }
    }
    return nullptr;
}

unsigned long long Scheduler::getCost() 
{
    for (size_t i = 0; i < _hosts.size(); i++)
    {
        cost += _hosts[i]->getCostBase();
    }    
    return cost;
}

void Scheduler::checkVMS() 
{
    for (size_t i = 0; i < _vms.size(); i++)
    {
        // if (!_vms[i]->getHost())
        // {
        //     cout << "vm dont have host"<< endl;
        // }
        
        // if (_vms[i]->getHost()->getIndex() >= _hosts.size() || _vms[i]->getHost()->getIndex() < 0)
        // {
        //     cout << "wrong vm host  id"<< endl;
        // }
        
    }
    
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

void Scheduler::_buyAHost(const HostInfo &host)
{
    shared_ptr<Host> newhost = make_shared<Host>(host, _hosts.size());
    // dcout << "debug host  " << _hosts.size()<< endl;
    // exit(0);
    _hosts.emplace_back(newhost);
    
    
    _free_host.emplace_back(newhost);
    recordPurchasedHost(host);
}

shared_ptr<Host> Scheduler::_buyAHost_immedidate(const HostInfo& host) 
{
    // int index = _hosts.size() - _today_purchased_hosts.size() ;
    shared_ptr<Host> newhost = make_shared<Host>(host, _hosts.size());
    // dcout << "debug host  " << _hosts.size()<< endl;
    // exit(0);
    _hosts.emplace_back(newhost);   
    
    // _free_host.emplace_back(newhost);
    recordPurchasedHost(host);
    return newhost;
}
void Scheduler::_buyAHost_opt(const HostInfo &host)
{
    shared_ptr<Host> newhost = make_shared<Host>(host, _hosts.size());
    // dcout << "debug host  " << _hosts.size()<< endl;
    // exit(0);
    _hosts.emplace_back(newhost);
    auto i = _free_host.begin();
    for (; i != _free_host.end(); i++)
    {
        if ((*i)->getCostPerDay() >= host.dailyCost)
        {
            _free_host.insert(i, newhost);
            break;
        }
    }
    if (i == _free_host.end())
    {
        _free_host.emplace_back(newhost);
    }
    recordPurchasedHost(host);
}

void Scheduler::_addHostToFree(shared_ptr<Host>& host)
{
    auto i = _free_host.begin();
    for (; i != _free_host.end(); i++)
    {
        if ((*i)->getCostPerDay() >= host->getCostPerDay())
        {
            _free_host.insert(i, host);
            return;
        }
    }
    if (i == _free_host.end())
    {
        _free_host.emplace_back(host);
    }
}

int Scheduler::getTodayDailyCost() 
{
    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        cost+=_busy_host[i]->getCostPerDay();
    }
    return 0;
}

void Scheduler::recordPurchasedHost(const HostInfo& host) 
{
    //  _today_purchased_hosts_vec.emplace_back(host.type);
    auto r = _today_purchased_hosts.find(host.type);
    if (r == _today_purchased_hosts.end())
    {
        _today_purchased_hosts.insert(make_pair(host.type, 1));
    }
    else
    {
        r->second += 1;
    }
}


void Scheduler::migrateVM(shared_ptr<VirtualMachine>& vm, shared_ptr<Host>& targetHost) {
    /*
     * migrate the vm to the target host
     */
    vm->getHost()->deleteVM(vm->getId());
    vm->setHost(targetHost);
    targetHost->addVM(vm);
}


void Scheduler::freeHost(shared_ptr<Host> &host) {
    /*
     * free the host, delete from the busy list, add to the free list
     */
    if(!host->isFree()){
        cout << "Not free yet" << endl;
        exit(-1);
    }
    _free_host.emplace_back(host);
    for(auto it = _busy_host.begin(); it != _busy_host.end(); it++){
        if((*it)->getIndex() == host->getIndex()){
            _busy_host.erase(it);
        }
    }
}


shared_ptr<Host> Scheduler::chooseAHostToFree() {
    /*
     * iterate the migrate_list, choose a host to free
     */
    shared_ptr<Host> leastBusyHost = _migrate_list[0];
    for(auto it : leastBusyHost->get_vms()){
        shared_ptr<VirtualMachine> vmToFree(it.second);
        bool flag = false;
        for(int i = _migrate_list.size()-1; i>=0; i--){
            shared_ptr<Host> hostToMigrate(_migrate_list[i]);
            if(hostToMigrate->addVM_try(vmToFree)){
                break;
            }
        }
        if(!flag){
            break;
        }
    }
}