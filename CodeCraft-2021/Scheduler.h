#ifndef _SCHEDULER_
#define _SCHEDULER_
#include <unordered_map>
#include "VirtualMachine.h"
#include "Host.h"
#include <vector>
#include "utils.h"

extern unordered_map<string, HostInfo> hostInfos;

using namespace std;
class Scheduler
{
private:
    /* data */
    vector<HostInfo> _host_candidates;
    unordered_map<int, shared_ptr<VirtualMachine>> _vms;
    vector<shared_ptr<Host>> _free_host;
    vector<shared_ptr<Host>> _busy_host;
    int _host_cheapest;
private:
    /* method */
    void deleteVM(const int id);
    void addVM(shared_ptr<VirtualMachine>& vm);
    void sumRequest(int &cpu, int &mem, const vector<shared_ptr<VirtualMachine>> &request);

private:
    /**temporary*/
    void findCheapestHost();
    
public:
    Scheduler(/* args */);
    ~Scheduler();
    void deletVM(vector<int> &ptr);
    void addVM(vector<shared_ptr<VirtualMachine>> &ptr);
    void shutHost(shared_ptr<Host> &host);
    shared_ptr<Host> haveFreeHost();
    void buyHosts(int cpu, int mem);
    void buyAHost(int cpu, int mem);

    unordered_map<string, int> getNewPurchasedHosts(); // get purchased hosts every day
    // void migration();
};

#endif