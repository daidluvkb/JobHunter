#ifndef _SCHEDULER_
#define _SCHEDULER_
#include <unordered_map>
#include "VirtualMachine.h"
#include "Host.h"
#include <vector>
#include "utils.h"

#include <sstream>
#include "Dp.h"
using namespace std;
class Scheduler
{
    unsigned int countFreeVolumn() const;

public:
    void histVmProbability(const unordered_map<string, VMInfo> &vms);
    void clearVmBuffer();
private:

    int low_pro_sup, low_pro_floor;
    vector<shared_ptr<VirtualMachine>> _vm_buffer;
    
    vector<vector<int>> dp_recurse(vector<int> index_vec, const vector<shared_ptr<VirtualMachine>> &vms);
    void buyHostsFor_dp(vector<shared_ptr<VirtualMachine>> &vms);
    shared_ptr<const HostInfo> chooseAHost(const vector<shared_ptr<VirtualMachine>> &vms);
    unordered_map<int, vector<int>> buyHostsfor(vector<shared_ptr<VirtualMachine>> vms);

public:

    void batchAddDoubleVmFinished();

private:

    unsigned long long cost;
    int _host_num_lastday;
    int getTodayDailyCost();
    void recordPurchasedHost(const HostInfo& host);
private:
    /* data */

    stringstream _today_add_arrangement;
    unordered_map<string, int> _today_purchased_hosts;//host type/name and cnt;
    vector<string> _today_purchased_hosts_vec;//host ;
private:
    vector<HostInfo> _host_candidates;
    unordered_map<int, shared_ptr<VirtualMachine>> _vms;

    vector<shared_ptr<VirtualMachine>> _vms_vec;//记录vm顺序，便于满足oj顺序处理要求

    vector<shared_ptr<Host>> _free_host;
    vector<shared_ptr<Host>> _busy_host;
    vector<shared_ptr<Host>> _hosts;
    int _host_cheapest;
    int _today_add_arrangement_num;
    double _probability;
private:
    /* method */
    void addVM(shared_ptr<VirtualMachine>& vm);
    void addVM_dp(shared_ptr<VirtualMachine>& vm);
    void addVM_opt(shared_ptr<VirtualMachine>& vm);
    void sumRequest(int &cpu, int &mem, const vector<shared_ptr<VirtualMachine>> &request);

private:
    /**temporary*/
    void findCheapestHost();
    void _buyAHost(const HostInfo& host);
    shared_ptr<Host> _buyAHost_immedidate(const HostInfo& host);
    void _buyAHost_opt(const HostInfo& host);
    void _addHostToFree(shared_ptr<Host>& host);
    
public:
    Scheduler(/* args */);
    ~Scheduler();
    void setHostCandidates(unordered_map<string, HostInfo> &hostInfos);
    void deleteVM(vector<int> &ptr);
    void deleteVM(const int id);
    void deleteVM_diff(const int id, bool is_double);
    void addVM(vector<shared_ptr<VirtualMachine>> &ptr);
    void addVM_bystep(shared_ptr<VirtualMachine> &vm);
    void addVM_bystep_dp(shared_ptr<VirtualMachine> &vm);
    void addVM_opt(vector<shared_ptr<VirtualMachine>> &vms);
   
    void shutHost(shared_ptr<Host> &host);
    void buyHosts(int cpu, int mem);
    void declareANewDay();
    int getTodayAddVMArrangment(vector<shared_ptr<VirtualMachine>>&vms);
    vector<shared_ptr<const HostInfo>> getNewPurchasedHosts(); // get purchased hosts every day

    shared_ptr<const HostInfo> chooseAHost(const int cpu, const int mem);
    shared_ptr<const HostInfo> chooseAHost_(const int cpu, const int mem);
    shared_ptr<const HostInfo> chooseAHost_opt2(const int cpu, const int mem);
    unsigned long long getCost();
    void checkVMS();

    /**
    *migration oprations
    */
private:
//    void migrateVM(shared_ptr<VirtualMachine>& vm, shared_ptr<Host>& targetHost);
    bool chooseAHostToFree(shared_ptr<Host>&);
    bool migrateVM(shared_ptr<VirtualMachine>& vm, shared_ptr<Host>& targetHost);
    vector<shared_ptr<Host>> _migrate_list;
    int _migrateVMNumPerDay;
    stringstream _todayMigrationInfo;
public:
    void getTodayMigration();
    void freeHost(shared_ptr<Host>& host);
    int get_migrateVMNumPerDay() const;
    static bool cmp(shared_ptr<Host>&, shared_ptr<Host> &);
    void oneDayMigration();
    void printMigrateInfo();
};

#endif