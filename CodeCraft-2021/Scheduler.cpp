#include "Scheduler.h"

Scheduler::Scheduler(/* args */)
{
   
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

    bool is_double = !vm->IsDoubleNode();
    /*
    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (_busy_host[i]->getAvailableCpu(is_double) > vm->getNumOfCpu() &&
            _busy_host[i]->getAvailableMem(is_double) > vm->getSizeOfMem())
        {
        //     dcout << "_busy_host:" << _busy_host.size() << endl;
        //     dcout << _busy_host[i]->getAvailableCpu(is_double) << '\t' << vm->getNumOfCpu() << endl;
        //     dcout << _busy_host[i]->getAvailableMem(is_double) << '\t' << vm->getSizeOfMem() << endl;
            _busy_host[i]->addVM(vm);
            vm->setHost(_busy_host[i]);
            return;
        }
    }
    */

    for (size_t i = 0; i < _busy_host.size(); i++)
    {
        if (is_double)
        {
            if (_busy_host[i]->getAvailableCpu(true) > (vm->getNumOfCpu() / 2) &&
                _busy_host[i]->getAvailableMem(true) > (vm->getSizeOfMem() / 2))
            {
                _busy_host[i]->addVM(vm);
                vm->setHost(_busy_host[i]);
                return;
            }
        }
        else
        {
            if (_busy_host[i]->getAvailableCpu(false) > vm->getNumOfCpu() &&
                _busy_host[i]->getAvailableMem(false) > vm->getSizeOfMem())
                if (_busy_host[i]->addVM(vm))
                {
                    vm->setHost(_busy_host[i]);
                    return;
                }
        }
    }

    if (!_free_host.size()) //
    {
        shared_ptr<const HostInfo> host;
        if(is_double)
            host = chooseAHost(vm->getNumOfCpu(), vm->getSizeOfMem());
        else
        {
            host = chooseAHost(vm->getNumOfCpu() * 2, vm->getSizeOfMem() * 2);
        }

        if (host){
            _buyAHost(*host);            
            //record buying action
            }
        else
        {
            buyHosts(vm->getNumOfCpu(), vm->getSizeOfMem());//host column must be bigger than vm require
        }
    }

    auto host = _free_host[_free_host.size() - 1];
    _busy_host.emplace_back(host);
    host->addVM(vm);
    vm->setHost(host);
    _free_host.pop_back();
    return;
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
        cout << "has no host candidate , please check input hostInfos" << endl;
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
    for (size_t i = 0; i < _hosts.size(); i++)
    {
        _hosts[i]->checkMyself();
    }
    
    _today_purchased_hosts.clear();
    // _today_add_arrangement.clear();
    _today_add_arrangement.str("");
    // cout << "today arrange: " << _today_add_arrangement.str();
    // fflush(stdout);
}

void Scheduler::getTodayAddVMArrangment()
{
    printf("%s", _today_add_arrangement.str().c_str());
    fflush(stdout);
}

vector<shared_ptr<const HostInfo>> Scheduler::getNewPurchasedHosts() 
{
    // int pur_num = 0;
    // for (auto& i : _today_purchased_hosts)
    // {
    //     pur_num += i.second;
    // }
    printf("(purchase, %d)\n", _today_purchased_hosts.size());
    fflush(stdout);
    for (auto i : _today_purchased_hosts)
    {
        printf("(%s, %d)\n", i.first.c_str(), i.second);
        fflush(stdout);
    }
        // cout << "(" << i.first << ", " << i.second << ")" << endl;
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
