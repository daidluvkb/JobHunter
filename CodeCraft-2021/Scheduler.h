#ifndef _SCHEDULER_
#define _SCHEDULER_
#include <unordered_map>
#include "VirtualMachine.h"
#include "Host.h"
class Scheduler
{
using namespace std;
private:
    /* data */
    unordered_map <int, VirtualMachine &> _vms;
    vector <shared_ptr<Host>> _free_host;
    vector <shared_ptr<Host>> _busy_host;
    void deleteVM(const int id);
    void addVM(const int id);
public:
    Scheduler(/* args */);
    ~Scheduler();
    void deletVM(vector<shared_ptr<VirtualMachine>> &ptr);
    void addVM(vector<shared_ptr<VirtualMachine>> &ptr);
    void shutHost(shared_ptr<Host>);
    shared_ptr <Host> haveFreeHost();
    void buyHosts(int cpu, int mem);
    void buyAHost(int cpu, int mem);
    void sumRequest(int& cpu, int& mem, const vector<shared_ptr<VirtualMachine>> &request);
    unordered_map <string, int> getNewPurchasedHosts(); // get purchased hosts every day  
    // void migration();
};



#endif