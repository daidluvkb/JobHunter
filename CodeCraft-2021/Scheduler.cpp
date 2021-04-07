#include "Scheduler.h"
#include <float.h>
#include <cmath>
#include <assert.h>

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
        int id = vms[i]->m_index;
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
    // for (size_t i = 0; i < vms.size(); i++)
    const size_t vmssize = vms.size();
    for (size_t i = 0; i < vmssize; i++)
    {
        int id = vms[i]->m_index;
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
                    i = _busy_host.erase(i);
                    _addHostToFree(host);
                    return;
                }
            }
            cout << "else\n";
            // if (i != _busy_host.end())
            // {     
                // _addHostToFree(host);           
            // }
            // else
            // {
            //     return;
            // }
        }
        return;
    }
    return;
}

void Scheduler::addVM_opt(shared_ptr<VirtualMachine>& vm){
    bool is_double = !vm->IsDoubleNode();
    double vm_mem_to_cpu = (double)vm->getSizeOfMem() / (double)vm->getNumOfCpu();
    double min_Host_mem_to_cpu = DBL_MAX;
    int sum_cpu_mem = vm->getNumOfCpu() + vm->getSizeOfMem();
    int left_cpu_mem = INT32_MAX;

    int index = 0;
    char Node = 'D';
    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (_busy_host[i]->addVM_try(vm))
        {            
            vm->setHost(_busy_host[i]);
            return ;
        }
    }
    // for (size_t i = 0; i < _busy_host.size(); i++)
    // {
    //     if (is_double)
    //     {
    //         if (_busy_host[i]->getAvailableCpu(true) >= (vm->getNumOfCpu() / 2) &&
    //             _busy_host[i]->getAvailableMem(true) >= (vm->getSizeOfMem() / 2))
    //         {
    //             if(abs(((double)_busy_host[i]->getAvailableMem(true) / (double)_busy_host[i]->getAvailableCpu(true)) - vm_mem_to_cpu)
    //              < abs(min_Host_mem_to_cpu - vm_mem_to_cpu)){
    //                 min_Host_mem_to_cpu = (double)_busy_host[i]->getAvailableMem(true) / (double)_busy_host[i]->getAvailableCpu(true);
    //                 index = i;
    //             }
    //         }
    //     }
    //     else
    //     {
    //         if (_busy_host[i]->getAvailableCpuA() >= vm->getNumOfCpu() &&
    //             _busy_host[i]->getAvailableMemA() >= vm->getSizeOfMem()){
    //                 if(abs(((double)_busy_host[i]->getAvailableMemA() / (double)_busy_host[i]->getAvailableCpuA()) - vm_mem_to_cpu)
    //                  < abs(min_Host_mem_to_cpu - vm_mem_to_cpu)){
    //                     min_Host_mem_to_cpu = (double)_busy_host[i]->getAvailableMemA() / (double)_busy_host[i]->getAvailableCpuA();
    //                     index = i;
    //                     Node = 'A';
    //             }
    //         }
    //         if(_busy_host[i]->getAvailableCpuB() > vm->getNumOfCpu() &&
    //            _busy_host[i]->getAvailableMemB() > vm->getSizeOfMem()){
    //                 if(abs(((double)_busy_host[i]->getAvailableMemB() / (double)_busy_host[i]->getAvailableCpuB()) - vm_mem_to_cpu)
    //                  < abs(min_Host_mem_to_cpu - vm_mem_to_cpu)){
    //                     min_Host_mem_to_cpu = (double)_busy_host[i]->getAvailableMemB() / (double)_busy_host[i]->getAvailableCpuB();
    //                     index = i;
    //                     Node = 'B';

    //                }
    //            }

    //     }
    // }
    // if(min_Host_mem_to_cpu != DBL_MAX){
    //     _busy_host[index]->addVM_opt(vm, Node);
    //     vm->setHost(_busy_host[index]);
    //     return;
    // }

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
        // host = chooseAHost(vm->getNumOfCpu(), vm->getSizeOfMem());
        host = chooseAHost_(vm->getNumOfCpu(), vm->getSizeOfMem());
    else
    {
        // host = chooseAHost(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
        host = chooseAHost_(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
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
            cout << "host cant contain vm??\n";
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

    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (_busy_host[i]->addVM_try(vm))
        {            
            vm->setHost(_busy_host[i]);
            return ;
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
    shared_ptr<const HostInfo> host_;
    if (is_double)
    {
        host = chooseAHost(vm->cpu, vm->mem);
        // host_ = chooseAHost_(vm->cpu, vm->mem);
        // Host tmphost(*host, -1);
        // Host tmphost_(*host_, -1);
        // int small, large;
        // int small_, large_;
        // tmphost.getAbnormalCapcityAfterAdd(vm, small, large);
        // tmphost_.getAbnormalCapcityAfterAdd(vm, small_, large_);
        // if (false)//(small < 5 && large > 70)
        // {
        //     host = chooseAHost_(vm->cpu, vm->mem);
        // }
    }
    else
    {
        host = chooseAHost(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
        host_ = chooseAHost_(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
        Host tmphost(*host, -1);
        Host tmphost_(*host_, -1);
        int small, large;
        int small_, large_;
        tmphost.getAbnormalCapcityAfterAdd(vm, small, large);//small和large 改成cpu和mem
        tmphost_.getAbnormalCapcityAfterAdd(vm, small_, large_);
        if (small < 1 && large > 70)
        {
            host = chooseAHost_(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
        }
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
            cout << "host cant contain vm??\n";
        }

    } //不太安全但好像逻辑安全

    return;
}

void Scheduler::addVM_bystep(shared_ptr<VirtualMachine> &vm)
{
    int id = vm->m_index;
    _vms.insert(make_pair(id, vm));

    addVM(vm);

    // if (_busy_host.size() + _free_host.size() != _hosts.size())
    // {
    //     cout << "host num not match\n";
    // }
    
    // if (!vm->getHost()->checkMyself())
    // {
    //     dcout << "Host wrong" << endl;
    // }
}

void Scheduler::addVM_bystep_dp(shared_ptr<VirtualMachine> &vm) 
{
    int id = vm->m_index;
    _vms.emplace(make_pair(id, vm));
    addVM_dp(vm);
}
void Scheduler::addVM_dp(shared_ptr<VirtualMachine>& vm)
{
    const int busyhost_size = _busy_host.size();
    for (size_t i = 0; i < busyhost_size; i++)
    {
        if (_busy_host[i]->isAbleToAddVM(vm))
        {
            int small = INT32_MAX, large = 0;
            int small_ = INT32_MAX, large_ = 0;
            _busy_host[i]->getAbnormalCapcity(small_, large_);
            _busy_host[i]->getAbnormalCapcityAfterAdd(vm, small, large);

            if (small < 6 && large > 70 && small_ != small && large != large_)
            {
                continue;
            }
            
            _busy_host[i]->addVM_try(vm);
            vm->setHost(_busy_host[i]);
            return;
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
    _vm_buffer.emplace_back(vm);
    return;
}
shared_ptr<vector<vector<int>>> Scheduler::buyHostsfor_itr(vector<shared_ptr<VirtualMachine>> vms)
{
    shared_ptr<vector<vector<int>>> res = make_shared<vector<vector<int>>>(); //记录组号-组成员在vms中下标
    vector<int> memo;        //记录当前每一组的组号-价格

    if (vms.empty())
        return res;

    shared_ptr<const HostInfo> host_1 = chooseAHost_(vms[0]->cpu, vms[0]->mem);
    res->emplace_back(vector<int>({0}));
    memo.emplace_back(host_1->basicCost + host_1->dailyCost);
    const int vms_size = vms.size();
    for (int i = 1; i < vms_size; i++)
    {
        shared_ptr<const HostInfo> host_i;
        shared_ptr<VirtualMachine> &vm_i = vms[i];
        if (vm_i->IsDoubleNode() == 0)
            host_i = chooseAHost_(vm_i->cpu, vm_i->mem);
        else
            host_i = chooseAHost_(vm_i->cpu * 2, vm_i->mem * 2);

        int excessExpense = 0;
        // if (!host_i)
        // {
        //     excessExpense = INT32_MAX;
        // }
        // else
        {
            excessExpense = host_i->basicCost + host_i->dailyCost;
        }
        
        const int res_size = res->size();
        int idx = res_size;
        for (size_t j = 0; j < res_size; j++)
        {
            auto &resj = res->at(j);
            const int res_j_size = resj.size();
            vector<shared_ptr<VirtualMachine>> tmpres(res_j_size);
            for (int k = 0; k < res_j_size; k++)
            {
                tmpres[k] = vms[resj[k]];
            }
            tmpres.emplace_back(vms[i]);
            shared_ptr<const HostInfo> costAfter = chooseAHost(tmpres);
            if (costAfter)
            {
                int excessExpense_j = costAfter->dailyCost + costAfter->basicCost; //合在一起花的钱
                if (excessExpense_j < memo[j] + excessExpense)
                {
                    memo[j] = excessExpense_j;
                    idx = j;
                } 
            }     //else
        }
        // vms.emplace_back(vm_last);
        if (idx == res_size) //单买更好
        {
            res->emplace_back(vector<int>({i}));
            memo.emplace_back(excessExpense);
        }
        else
        {
            res->at(idx).emplace_back(i); //在对应组加入最后一个vm的下标记录
        }
    }
    return res;
}

void Scheduler::clearVmBuffer() 
{
    if (_vm_buffer.empty())
        return;
    auto grouping = buyHostsfor_itr(_vm_buffer);
    // for (size_t i = 1; i <= grouping.size(); i++)
    // {
    //     vector<shared_ptr<VirtualMachine>> group_vm;
    //     auto& vm_group_i = grouping[i];
    //     for (size_t j = 0; j < grouping[i].size(); j++)
    //     {
    //         group_vm.emplace_back(_vm_buffer[vm_group_i[j]]);
    //     }
        
    //     auto host_info = chooseAHost(group_vm);
    //     auto host = _buyAHost_immedidate(*host_info);
    //     if (host)
    //     {
    //         host->addVMs(group_vm);
    //         for (size_t j = 0; j < group_vm.size(); j++)
    //         {
    //             group_vm[j]->setHost(host);
    //         }
    //         _busy_host.emplace_back(host);
    //     }
    // }    
    const size_t grouping_size = grouping->size();
    for (size_t i = 0; i < grouping_size; i++)
    {
        vector<shared_ptr<VirtualMachine>> group_vm;
        auto& vm_group_i = grouping->at(i);
        const size_t grouping_i_size = grouping->at(i).size();
        for (size_t j = 0; j < grouping_i_size; j++)
        {
            group_vm.emplace_back(_vm_buffer[vm_group_i[j]]);
        }
        
        auto host_info = chooseAHost(group_vm);//ljm这里要改
        auto host = _buyAHost_immedidate(*host_info);
        if (host)
        {
            host->addVMs(group_vm);
            const size_t group_vm_size = group_vm.size();
            for (size_t j = 0; j < group_vm_size; j++)
            {
                group_vm[j]->setHost(host);
            }
            _busy_host.emplace_back(host);
        }
    }    
    // vector<shared_ptr<VirtualMachine>> empty_vec(0);
    // _vm_buffer.swap(empty_vec);
    _vm_buffer.clear();
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
    _migrateVMNumPerDay = 0;
    _host_num_lastday = _hosts.size();
    
    
    // int vmsnum = 0;
    // for (size_t i = 0; i < _hosts.size(); i++)
    // {
    //     _hosts[i]->checkMyself();
    //     vmsnum += _hosts[i]->getNumOfVM();
    //     if (_hosts[i]->getIndex() != i)
    //     {
    //         dcout << "host index wrong\n";
    //     }        
    // }
    // if (vmsnum != _vms.size())
    // {
    //     dcout << "vm num wrong\n";
    // }
    
    // getTodayDailyCost();
    // for (size_t i = 0; i < _free_host.size(); i++)
    // {
    //     if (!_free_host[i]->isFree())
    //     {
    //         cout << "host is not free\n";
    //     }        
    // }    

 
    
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
        // stringstream debugcheck; //ljm
        if (!vm->IsDoubleNode())
        {
            _today_add_arrangement << "(" << vm->getHost()->getIndex() << ")\n";
            // debugcheck << "(" << vm->getHost()->getIndex() << ")\n";
        }
        else
        {
            _today_add_arrangement << "(" << vm->getHost()->getIndex() << ", " << vm->getNode() << ")\n";
            // debugcheck << "(" << vm->getHost()->getIndex() << ", " << vm->getNode() << ")\n";
        }
        // string stringcheck = debugcheck.str();
        _today_add_arrangement_num++;
    }
    _arrangement_info_eachday.emplace(_today_add_arrangement.str());
    // printf("%s", _today_add_arrangement.str().c_str());
    // fflush(stdout);
    return _today_add_arrangement_num;
}

vector<shared_ptr<const HostInfo>> Scheduler::getNewPurchasedHosts() 
{
    int today_purchased_num = 0;
    stringstream todaypurchase;
    todaypurchase << "(purchase, " << _today_purchased_hosts.size() << ")\n";
    // printf("(purchase, %d)\n", _today_purchased_hosts.size());  
      
    for (auto i : _today_purchased_hosts)
    {
        // printf("(%s, %d)\n", i.first.c_str(), i.second);
        todaypurchase << "(" << i.first << ", " << i.second << ")\n";
        
        int cnt = i.second;
        for (size_t j = _host_num_lastday; j < _hosts.size(); j++)
        {
            if (_hosts[j]->getType() == i.first)
            {
                _hosts[j]->setIndex(today_purchased_num + _host_num_lastday);
                today_purchased_num++;
                if (--cnt)
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
    _purchase_info_eachday.emplace(todaypurchase.str());
    return vector<shared_ptr<const HostInfo>>();
}

shared_ptr<const HostInfo> Scheduler::chooseAHost(const vector<shared_ptr<VirtualMachine>> &vms) 
{
    int cpuA = 0, memA = 0;
    int cpuB = 0, memB = 0;
    for (size_t i = 0; i < vms.size(); i++)
    {
        const int cpu = vms[i]->cpu;
        const int mem = vms[i]->mem;
        bool is_double = vms[i]->IsDoubleNode() == 0;
        if (is_double)
        {
            cpuA += cpu / 2;
            cpuB += cpu / 2;
            memA += mem / 2;
            memB += mem / 2;
        }
        else
        {
            if (abs(cpuA + cpu - cpuB) + abs(memA + mem - memB) <= abs(cpuB + cpu - cpuA) + abs(memB + mem - memA))
            {
                cpuA += cpu;
                memA += mem;
            }
            else
            {
                cpuB += cpu;
                memB += mem;
            }
        }
    }
    auto host = chooseAHost_(max(cpuA, cpuB) * 2, max(memA, memB) * 2);
    return host;
}

void Scheduler::getTodayMigration() 
{
//    printf( "(migration, 0)\n");
//    fflush(stdout);
//    cout <<  "(migration, " << get_migrateVMNumPerDay() << ")" << endl;
//    cout <<  "(migration, " << 0 << ")" << endl;
    // printf("(migration, %d)\n", get_migrateVMNumPerDay());

    // printf("%s", _todayMigrationInfo.str().c_str());
_migration_info_eachday.emplace("(migration, " + to_string(get_migrateVMNumPerDay()) + ")\n" +
                                _todayMigrationInfo.str());
_todayMigrationInfo.str("");
//    printMigrateInfo();
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

shared_ptr<const HostInfo> Scheduler::chooseAHost_(const int cpu, const int mem){
     double mem_to_cpu = (double)mem / (double)cpu;
    double gap = DBL_MAX;
    int count = 3;  //取三个最接近的
    int count_ = 10; //取十个可以装入的
    int index = -1;  //满足条件的下标
    int index_ = -1; //以防万一
    // int min_dailycost = INT32_MAX;
    for (size_t i = 0; i < _host_candidates.size() && count > 0 && count_ > 0; i++)
    {
        if (_host_candidates[i].cpu >= cpu && _host_candidates[i].mem >= mem)
        {
            index_ = index_ == -1 ? i : index_;
            if (fabs(((double)_host_candidates[i].mem / (double)_host_candidates[i].cpu) - mem_to_cpu) < gap && ((double)mem / (double)_host_candidates[i].mem) > 0.4 /*&& _host_candidates[i].dailyCost < min_dailycost*/)
            {
                gap = fabs(((double)_host_candidates[i].mem / (double)_host_candidates[i].cpu) - mem_to_cpu);
                // min_dailycost = _host_candidates[i].dailyCost;
                index = i;
                count--;
            }
            count_--;
        }
    }
    if (index != -1)
        return make_shared<HostInfo>(_host_candidates[index]);
    else if (index_ != -1)
        return make_shared<HostInfo>(_host_candidates[index_]);
    return nullptr;
}

shared_ptr<const HostInfo> Scheduler::chooseAHost_opt2(const int cpu, const int mem) 
{

    
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
//    for (size_t i = 0; i < _vms.size(); i++)
//    {
//        // if (!_vms[i]->getHost())
//        // {
//        //     cout << "vm dont have host"<< endl;
//        // }
//
//        // if (_vms[i]->getHost()->getIndex() >= _hosts.size() || _vms[i]->getHost()->getIndex() < 0)
//        // {
//        //     cout << "wrong vm host  id"<< endl;
//        // }
//        _vms[i]->checkMyself();
//    }
    for(auto it:_hosts){
//        cout <<"selfcheck\n";
        (*it).checkMyself();
//        if(!(*it).checkMyself()){
//            cout << "wrong migrate" << endl;
//        };
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
        cost += _busy_host[i]->getCostPerDay();
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

void Scheduler::printADayInfo() 
{
    printf("%s", _purchase_info_eachday.front().c_str());
    printf("%s", _migration_info_eachday.front().c_str());
    printf("%s", _arrangement_info_eachday.front().c_str());
    _purchase_info_eachday.pop();
    _migration_info_eachday.pop();
    _arrangement_info_eachday.pop();
    fflush(stdout);
}

void Scheduler::printRemainInfo()
{
    while (!_purchase_info_eachday.empty())
    {
        printf("%s", _purchase_info_eachday.front().c_str());
        printf("%s", _migration_info_eachday.front().c_str());
        printf("%s", _arrangement_info_eachday.front().c_str());
        _purchase_info_eachday.pop();
        _migration_info_eachday.pop();
        _arrangement_info_eachday.pop();
    }
    fflush(stdout);
}

bool Scheduler::isMigrationOver5_1000() const
{
    return _migrateVMNumPerDay >= ((int)(_vms.size() * 0.03 - 2));
}


bool Scheduler::migrateVM(shared_ptr<VirtualMachine>& vm, shared_ptr<Host>& targetHost) {
    /*
     * migrate the vm to the target host
     */
    // if(isMigrationOver5_1000())
    //     return false;
    if(targetHost->addVM_try_migrate(vm)){
//        vm->getHost()->deleteVM(vm->m_index);
        vm->setHost(targetHost);
//        targetHost->addVM(vm);
        return true;
    }
    return false;
}


bool Scheduler::freeHost(shared_ptr<Host> &host) {
    /*
     * free the host, delete from the busy list, add to the free list
     */
    if (!host->isFree())
    {
        // cout << "Not free yet" << endl;
        return false;
    }

    for (auto it = _busy_host.begin(); it != _busy_host.end(); it++)
    {
        if ((*it)->getIndex() == host->getIndex())
        {
            _busy_host.erase(it);
            _free_host.emplace_back(host);
            return true;
        }
    }
    return false;
}

bool Scheduler::chooseAHostToFree(shared_ptr<Host> &leastBusyHost)
{
    /*
     * iterate the migrate_list, choose a host to free
     */
    //    shared_ptr<Host> leastBusyHost = _migrate_list[0];
    //    for(auto it = leastBusyHost->get_vms().begin(); it!= leastBusyHost->get_vms().end(); ){
    //    for(auto it : leastBusyHost->get_vms()){
        const int leastBusyHost_index = leastBusyHost->getIndex();
    auto it = leastBusyHost->get_vms().begin();
    while (it != leastBusyHost->get_vms().end())
    {
        if(isMigrationOver5_1000())
            break;
        shared_ptr<VirtualMachine> vmToFree((*it).second);
        bool flag = false;
        for (int i = _migrate_list.size() - 1; leastBusyHost_index != _migrate_list[i]->getIndex(); --i)
        //在migrate_list找一个daily最便宜的塞进去/不要碰到自己的这个host
        {
            shared_ptr<Host>& hostToMigrate =_migrate_list[i];
            if (migrateVM(vmToFree, hostToMigrate))
            { // migrate success
                _migrateVMNumPerDay++;

                flag = true;
                //                it++;
                it = leastBusyHost->get_vms().begin();
                char node = vmToFree->getNode();
                if (node == 'D')
                {
                    _todayMigrationInfo << "(" << vmToFree->m_index << ", " << hostToMigrate->getIndex() << ")\n";
                }
                else
                {
                    _todayMigrationInfo << "(" << vmToFree->m_index << ", " << hostToMigrate->getIndex() << ", " << node << ")\n";
                }
                //                    if(_migrateVMNumPerDay >= ((5 * _vms.size())/ 1000 -1)) return false;
                break;
            }
        }
        if (!flag)//如果有一个失败了那就不折腾了 说明这个无法搬空
        {
            break;
        }
    }
    // if the host is empty, put it in the free list
    // if (freeHost(leastBusyHost))
    //     return true;
    if (leastBusyHost->isFree())
    {
        auto i = _busy_host.begin();
        for (; i != _busy_host.end(); i++)
        {
            if ((*i)->getIndex() == leastBusyHost_index)
            {
                i = _busy_host.erase(i);
                _addHostToFree(leastBusyHost);
                return true;
            }
        }
    }
    return false;
}

void Scheduler::oneDayMigration() {
    /*
     * This method handles one day's all migration begin from the migration_list
     * first maintain the migration list everyday
     */
    // double noload = 0.0;
    // for (auto &i : _busy_host)
    // {
    //      noload += i->getNoLoadRatio();
    // }
    // noload /= _busy_host.size();
    // cout <<"before: "<< noload << endl;
    // hist no load rate in each busy host

    _migrate_list = _busy_host;
    //    cout << "_host:" << _hosts.size() << " free:" << _free_host.size() << " busy:" << _busy_host.size() << " migrate:" << _migrate_list.size() << endl;
    std::sort(_migrate_list.begin(), _migrate_list.end(), cmp);
    int cnt = min(250, (int)_migrate_list.size());
    int cnttmp = 0;
    for (auto itr = _migrate_list.begin(); itr != _migrate_list.end() ; )
    {
        //        cout << (*itr)->getType() << endl;
        if (isMigrationOver5_1000())
            break;
        shared_ptr<Host> &freeHost = (*itr);

        if (chooseAHostToFree(freeHost))
        {
            // free success
            _migrate_list.erase(itr);

        }
        else
        {
            itr++;
            cnt--;
            if (!cnt)
            {
                break;
            }
            // itr++;
            // break;
        }
        //        break;
    }
    // noload = 0.0;
    // for (auto &i : _busy_host)
    // {
    //      noload += i->getNoLoadRatio();
    // }
    // noload /= _busy_host.size();
    // cout <<"after: "<< noload << endl;
    // for (size_t i = 0; i < _busy_host.size(); i++)
    // {
    //     auto & h = _busy_host[i];
    //     if (_busy_host[i]->isFree())
    //     {
    //         cout << "host is free\n";
    //     }
    // }
    
}

int Scheduler::get_migrateVMNumPerDay() const {
    return _migrateVMNumPerDay;
}

bool Scheduler::cmp(shared_ptr<Host> & a, shared_ptr<Host> & b) {
    // return a->getVmNum() < b->getVmNum();
    return a->m_cost_perday > b->m_cost_perday;
    return (a->getAvailableCpuA() + a->getAvailableCpuB()) > (b->getAvailableCpuA() + b->getAvailableCpuB());
}

void Scheduler::printMigrateInfo() {
    printf("%s", _todayMigrationInfo.str().c_str());
    fflush(stdout);
    _todayMigrationInfo.str("");
} 