#ifndef _SCHEDULER_
#define _SCHEDULER_
#include <unordered_map>
#include "VirtualMachine.h"
#include "Host.h"
#include <vector>
#include "utils.h"

#include <sstream>

using namespace std;
class Scheduler
{
private:
    unsigned long long cost;
    int getTodayDailyCost();
private:
    /* data */
    stringstream _today_add_arrangement;
    unordered_map<string, int> _today_purchased_hosts;//host type/name and cnt;
private:
    vector<HostInfo> _host_candidates;
    unordered_map<int, shared_ptr<VirtualMachine>> _vms;
    vector<shared_ptr<Host>> _free_host;
    vector<shared_ptr<Host>> _busy_host;
    vector<shared_ptr<Host>> _hosts;
    int _host_cheapest;
    int _today_add_arrangement_num;
private:
    /* method */
    void deleteVM(const int id);
    void addVM(shared_ptr<VirtualMachine>& vm);
    void addVM_opt(shared_ptr<VirtualMachine>& vm);
    void sumRequest(int &cpu, int &mem, const vector<shared_ptr<VirtualMachine>> &request);

private:
    /**temporary*/
    void findCheapestHost();
    void _buyAHost(const HostInfo& host);
    shared_ptr<Host> _buyAHost_immedidate(const HostInfo& host);
    void _buyAHost_opt(const HostInfo& host);
    
public:
    Scheduler(/* args */);
    ~Scheduler();
    void setHostCandidates(unordered_map<string, HostInfo> &hostInfos);
    void deleteVM(vector<int> &ptr);
    void addVM(vector<shared_ptr<VirtualMachine>> &ptr);
    void addVM_opt(vector<shared_ptr<VirtualMachine>> &vms);
   
    void shutHost(shared_ptr<Host> &host);
    void buyHosts(int cpu, int mem);
    void declareANewDay();
    int getTodayAddVMArrangment();
    vector<shared_ptr<const HostInfo>> getNewPurchasedHosts(); // get purchased hosts every day

    shared_ptr<const HostInfo> chooseAHost(const int cpu, const int mem);
    unsigned long long getCost();
    void checkVMS();

    /**
    *migration oprations
    */
private:
    void migrateVM(shared_ptr<VirtualMachine>& vm, shared_ptr<Host>& targethost);
    shared_ptr<Host> chooseAHostToFree();

public:
    void getTodayMigration();
    bool freeHost(shared_ptr<Host>& host);
};

#endif