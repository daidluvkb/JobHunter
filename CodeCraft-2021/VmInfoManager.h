#ifndef VM_INFO_MANAGER
#define VM_INFO_MANAGER
#include "utils.h"
class VmInfoManager
{
    const unordered_map<string, VMInfo> &vmInfos;
    vector<shared_ptr<VMInfo>> _vminfo_vec;
    // vector<vector<int>> vmPD;
    int f; //= 10 概率密度分割尺度
    vector<vector<int>> pd;
    double vm_d_cpu;
    double vm_d_mem;
    int vm_num;
    int min_cpu, min_mem, max_cpu, max_mem;

public:
    VmInfoManager(const unordered_map<string, VMInfo> &setvmInfos)
        : vmInfos(setvmInfos), f(20),
          pd(f, vector<int>(f, 0)),
          vm_d_cpu(0.0), vm_d_mem(0.0), vm_num(0)
    {
        _vminfo_vec.reserve(vmInfos.size());
        for (auto iter = vmInfos.begin(); iter != vmInfos.end(); ++iter)
        {
            _vminfo_vec.emplace_back(make_shared<VMInfo>(iter->second));
        }
        vmProbabilityDensity();
    }

private:
    static bool cmpVmCpu(shared_ptr<VMInfo> &vm1, shared_ptr<VMInfo> &vm2)
    {
        int cpu1 = vm1->cpu, cpu2 = vm2->cpu;
        if(vm1->isDouble)
            cpu1 /= 2;
        if(vm2->isDouble)
            cpu2 /= 2;
        return cpu1 < cpu2;
    }
    static bool cmpVmMem(shared_ptr<VMInfo> &vm1, shared_ptr<VMInfo> &vm2) 
    {
        int mem1 = vm1->mem, mem2 = vm2->mem;
        if (vm1->isDouble)
            mem1 /= 2;
        if (vm2->isDouble)
            mem2 /= 2;
        return mem1 < mem2;
    }
    void vmProbabilityDensity()
    { //记录每个区间内的vm种类数量
    
        // vector<vector<int>> pd(f, vector<int>(f, 0));
        //记录范围最大值    //ljm add 以及最小值
     
        {
            sort(_vminfo_vec.begin(), _vminfo_vec.end(), cmpVmCpu);
            shared_ptr<VMInfo> &mincpu_vm = *(_vminfo_vec.begin());
            shared_ptr<VMInfo> &maxcpu_vm = *(_vminfo_vec.rbegin());
            min_cpu = mincpu_vm->cpu;
            max_cpu = maxcpu_vm->cpu;
            if (mincpu_vm->isDouble)
                min_cpu /= 2;
            if (maxcpu_vm->isDouble)
                max_cpu /= 2;
        }//统计cpu
        {
            sort(_vminfo_vec.begin(), _vminfo_vec.end(), cmpVmMem);
            shared_ptr<VMInfo> &minmem_vm = *(_vminfo_vec.begin());
            shared_ptr<VMInfo> &maxmem_vm = *(_vminfo_vec.rbegin());
            min_mem = minmem_vm->mem;
            max_mem = maxmem_vm->mem;
            if (minmem_vm->isDouble)
                min_mem /= 2;
            if (maxmem_vm->isDouble)
                max_mem /= 2;
        }//统计mem

        vm_num = vmInfos.size();

        // for (auto iter = vmInfos.begin(); iter != vmInfos.end(); ++iter)
        // {
        //     if (iter->second.cpu > max_cpu)
        //         max_cpu = iter->second.cpu;
        //     if (iter->second.mem > max_mem)
        //         max_mem = iter->second.mem;
        //     vm_num++;
        // }

        vm_d_cpu = (double)(max_cpu - min_cpu) / f;
        vm_d_mem = (double)(max_mem - min_mem) / f;
        for (auto iter = vmInfos.begin(); iter != vmInfos.end(); ++iter)
        {
            for (int i = 0; i < f; i++)
            {
                if (iter->second.cpu > (i + 1) * vm_d_cpu + min_cpu)
                    continue;
                for (int j = 0; j < f; j++)
                {
                    if (iter->second.mem > (j + 1) * vm_d_mem + min_mem)
                        continue;
                    ++pd[i][j];
                }
            }
        }

        // for (int i = f-1; i >=0 ; i++)
        //     pd[0][i] += pd[0][i - 1];
        // for (int i = 1; i < f; i++)
        //     pd[i][0] += pd[i - 1][0];
        // for (int i = 1; i < f; i++)
        // {
        //     for (int j = 1; j < f; j++)
        //     {
        //         pd[i][j] += pd[i - 1][j] + pd[i][j - 1] - pd[i - 1][j - 1];
        //     }
        // }

        // copy(pd.begin(), pd.end(), vmPD.begin());
    }
public:
    double getVmProbability(const int cpu, const int mem) const
    {
        int i = (cpu - min_cpu) / vm_d_cpu -1;
        int j = (mem - min_mem) / vm_d_mem-1;
        if(i == -1 || j == -1)
            return 0.0;
        // i = i * vm_d_cpu == cpu ? i - 1 : i;
        // j = j * vm_d_mem == mem ? j - 1 : j;
        i = min(i, f - 1);
        j = min(j, f - 1);
        double vpd = (double)pd[i][j] / vm_num;
        return vpd;
    }
};
#endif//VM_INFO_MANAGER