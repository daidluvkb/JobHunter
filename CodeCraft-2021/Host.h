#include <iostream>
#include <string>
#include <unordered_map>
#include "VirtualMachine.h"
using namespace std;

class Host{
    private:
        string m_type;
        const int m_num_of_cpu;
        const int m_size_of_mem;
        const int m_cost_base;
        const int m_cost_perday;
        unordered_map <int, shared_ptr <VirtualMachine>> _vms;
        int _left_cpu_A;
        int _left_cpu_B;
        int _left_mem_A;
        int _left_mem_B;
    public:
        Host();
        Host(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday);
        
        int getNumOfCpu();
        int getSizeOfMem();
        int getCostBase();
        int getCostPerDay();
        void deleteVM(int id);
        void addVM(int id, bool left); // A or B 
        int getAvailableCpu(bool isDouble);
        int getAvailableMem(bool isDouble);
        bool isFree();
        
};