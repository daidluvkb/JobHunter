#ifndef _HOST_H
#define _HOST_H
#include <iostream>
#include <string>
#include <unordered_map>
#include "VirtualMachine.h"
#include "math.h"
#include "utils.h"
#include <vector>
using namespace std;

class Host{
    private:
        string m_type;
        const int m_num_of_cpu;
        const int m_size_of_mem;
        const int m_cost_base;
        int m_index;
        unordered_map <int, shared_ptr <VirtualMachine>> _vms;
        int _left_cpu_A;
        int _left_cpu_B;
        int _left_mem_A;
        int _left_mem_B;
    public:
        const int m_cost_perday;
        double getNoLoadRatio() const;
        bool isAbleToAddVM(shared_ptr<VirtualMachine> &vm);
        int getVmNum() const;
        string getType() const { return m_type; }
        bool checkMyself() const;
        Host(const string &type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday, int index);
        Host(const HostInfo &hostinfo, const int index);
        int getSmallestCapacityAfterAdd(const shared_ptr<VirtualMachine> &vm) const;
        int getLargestCapacityAfterAdd(const shared_ptr<VirtualMachine> &vm) const;
        void getAbnormalCapcityAfterAdd(const shared_ptr<VirtualMachine> &vm, int &small, int &large) const;
        double getRemainCapacityAfterAdd(const shared_ptr<VirtualMachine> &vm, int &cpu, int &mem) const;
        void getAbnormalCapcity(int &small, int &large) const;
        
        void setIndex(const int index) { m_index = index; }
        int getNumOfCpu();
        int getSizeOfMem();
        int getCostBase();
        int getCostPerDay();
        unordered_map<int, shared_ptr<VirtualMachine>>& get_vms();
        void deleteVM(int id);
        int getIndex();
        bool addVMs(vector<shared_ptr<VirtualMachine>>& vms) ;
        bool addVM(shared_ptr<VirtualMachine>& vm);
        bool addVM_try(shared_ptr<VirtualMachine>& vm);
        bool addVM_try_migrate(shared_ptr<VirtualMachine>& vm);
        bool addVM_opt(shared_ptr<VirtualMachine>& vm, char& Node);
        //void addVM(int id, bool left); // A or B 
        int getAvailableCpu(bool isDouble);
        int getAvailableMem(bool isDouble);
        int getAvailableCpuA();
        int getAvailableCpuB();
        int getAvailableMemA();
        int getAvailableMemB();
        bool isFree();
        int getNumOfVM() const { return _vms.size(); }
        //int getLeftAfterAdd(const shared_ptr<VirtualMachine>& vm, char& Node);

    private:
        void print() const;
};

#endif