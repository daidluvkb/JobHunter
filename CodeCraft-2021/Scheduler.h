#ifndef _SCHEDULER_
#define _SCHEDULER_
#include <unordered_map>
#include "VirtualMachine.h"
#include "Host.h"
#include <vector>
#include "utils.h"
#include <queue>

#include <sstream>

using namespace std;
class Scheduler
{
private:
    unsigned long long cost;
    int _host_num_lastday;
    int getTodayDailyCost();
    void recordPurchasedHost(const HostInfo& host);
    queue<string> _purchase_info_eachday;
    queue<string> _migration_info_eachday;
    queue<string> _arrangement_info_eachday;
private:
    /* data */

    bool isMigrationOver5_1000() const;

    stringstream _today_add_arrangement;
    unordered_map<string, int> _today_purchased_hosts;//host type/name and cnt;
    vector<string> _today_purchased_hosts_vec;//host ;
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
    void addVM(shared_ptr<VirtualMachine>& vm);
    void addVM_opt(shared_ptr<VirtualMachine>& vm);
    
    void sumRequest(int &cpu, int &mem, const vector<shared_ptr<VirtualMachine>> &request);

private:
    /**temporary*/
    void findCheapestHost();
    void _buyAHost(const HostInfo& host);
    shared_ptr<Host> _buyAHost_immedidate(const HostInfo& host);
    void _buyAHost_opt(const HostInfo& host);
    void _addHostToFree(shared_ptr<Host>& host);
    vector<shared_ptr<VirtualMachine>> _vm_buffer;
    
public:
    double getVmProbability(const int cpu,  const int mem) const;   //lst

    Scheduler(/* args */);
    ~Scheduler(); 
    void printADayInfo();
    void printRemainInfo();
    void setHostCandidates(unordered_map<string, HostInfo> &hostInfos);
    void deleteVM(vector<int> &ptr);
    void deleteVM(const int id);
    void addVM(vector<shared_ptr<VirtualMachine>> &ptr);
    void addVM_bystep(shared_ptr<VirtualMachine> &vm);
    void addVM_bystep_dp(shared_ptr<VirtualMachine> &vm);
    void addVM_dp(shared_ptr<VirtualMachine>& vm);
    void clearVmBuffer();
    shared_ptr<vector<vector<int>>> buyHostsfor_itr(vector<shared_ptr<VirtualMachine>> vms);
    void addVM_opt(vector<shared_ptr<VirtualMachine>> &vms);

    void shutHost(shared_ptr<Host> &host);
    void buyHosts(int cpu, int mem);
    void declareANewDay();
    int getTodayAddVMArrangment(vector<shared_ptr<VirtualMachine>>&vms);
    vector<shared_ptr<const HostInfo>> getNewPurchasedHosts(); // get purchased hosts every day

    shared_ptr<const HostInfo> chooseAHost(const vector<shared_ptr<VirtualMachine>> &vms);
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
    bool freeHost(shared_ptr<Host>& host);
    int get_migrateVMNumPerDay() const;
    static bool cmp(shared_ptr<Host>&, shared_ptr<Host> &);
    void oneDayMigration();
    void printMigrateInfo();
    //第一步求今天可以迁移的vm最大数量
    //2：找到几个空载率最高的host，vm总和小于第一步
    //3；在freelist里面n个freelist 和m 个vm 求daily最小；
    //  [[1,2,3],[4,5,6],[7,8],[9],[10]][对应的host]
    //4： shared_ptr<vector<vector<int>>> chooseHostsforMigrationVms_dp(vector<shared_ptr<VirtualMachine>> vms);
    //统计迁移：//czy

};

#endif