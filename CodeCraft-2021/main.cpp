#include "iostream"
#include <string>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include "Scheduler.h"
#include "VirtualMachine.h"
#include "utils.h"
using namespace std;
#include <bits/stdc++.h>

unordered_map<string, HostInfo> hostInfos;
unordered_map<string, VMInfo> vmInfos;
// bool todayHasRequest
// (

// );


void makeNewHost(string &s){ // get all types of hosts, and stores the infos
    int id_h = 1; //"("
    int id_tail = s.find(',');
    string id = s.substr(id_h, id_tail - id_h); // id of the server
    // dcout << id << endl;
    int cpu_h = s.find(' ', id_tail);
    int cpu_tail = s.find(',', cpu_h);
    int cpu_num = stoi(s.substr(cpu_h + 1, cpu_tail - cpu_h - 1));
    int mem_h = s.find(' ', cpu_tail);
    int mem_t = s.find(',', mem_h);
    int mem_num = stoi(s.substr(mem_h + 1, mem_t - mem_h - 1));
    int hw_cost_h = s.find(' ', mem_t);
    int hw_cost_t = s.find(',', hw_cost_h);
    int hardware_cost = stoi(s.substr(hw_cost_h + 1, hw_cost_t - hw_cost_h - 1));
    int daily_cost_h = s.find(' ', hw_cost_t);
    int daily_cost_t = s.find(')', daily_cost_h);
    int daily_cost = stoi(s.substr(daily_cost_h + 1, daily_cost - daily_cost_h - 1));
    HostInfo h_info;
    h_info.type = id;
    h_info.cpu = cpu_num;
    h_info.mem = mem_num;
    h_info.basicCost = hardware_cost;
    h_info.dailyCost = daily_cost;
    pair<string, HostInfo> p(id, h_info);
    hostInfos.insert(p);
}

void makeNewVM(string & s){
    int id_h = 1; //"("
    int id_tail = s.find(',');
    string id = s.substr(id_h, id_tail - id_h); // id of the server
    int cpu_h = s.find(' ', id_tail);
    int cpu_tail = s.find(',', cpu_h);
    int cpu_num = stoi(s.substr(cpu_h + 1, cpu_tail - cpu_h - 1));
    int mem_h = s.find(' ', cpu_tail);
    int mem_t = s.find(',', mem_h);
    int mem_num = stoi(s.substr(mem_h + 1, mem_t - mem_h - 1));
    char cc = s[s.length() - 2];
    bool doubleNode = (cc == '1');
    VMInfo vmInfo;
    vmInfo.cpu = cpu_num;
    vmInfo.mem = mem_num;
    vmInfo.isDouble = doubleNode;
    vmInfos.insert(make_pair(id, vmInfo));
}

shared_ptr<VirtualMachine> addNewVM(string & s, vector<shared_ptr<VirtualMachine>> & oneDayVM){
    int id_h = s.find(' ');
    int id_t = s.find(',', id_h);
    string id = s.substr(id_h + 1, id_t - id_h - 1);
    int index_h = s.find(' ', id_t);
    int index_t = s.find(')', index_h);
    int index = stoi(s.substr(index_h + 1, index_t - index_h - 1));
    VMInfo itr = vmInfos.find(id)->second;
    shared_ptr<VirtualMachine> ptr(make_shared<VirtualMachine>(id, itr.cpu, itr.mem, itr.isDouble, index));
    oneDayVM.push_back(ptr);
    return ptr;
}


#include <unistd.h>
void readFile(const string &testName, Scheduler &scheduler)
{
    char cwd[50];
    getcwd(cwd, 50);
    string filename(cwd);
    filename.append("/");
    filename.append(testName);
    filename = testName;
    cout << filename << endl;
    ifstream infile(filename, ios::in);
    assert(infile.is_open());

    // auto &infile = cin;
    string s; //This variable stores the strings parsed every line
    getline(infile, s);
    fflush(stdin);
    int hostsTypeNum = stoi(s);
    for (int i = 0; i < hostsTypeNum; i++)
    {
        getline(infile, s);
        fflush(stdin);
        makeNewHost(s);
    }

    scheduler.setHostCandidates(hostInfos);

    getline(infile, s);
    fflush(stdin);
    int vmTypeNum = stoi(s);
    for (int i = 0; i < vmTypeNum; i++)
    {
        getline(infile, s);
        fflush(stdin);
        makeNewVM(s);
    }

    getline(infile, s);
    fflush(stdin);
    int days = stoi(s);

    for (int i = 0; i < days; i++)
    {
        /*
        * at the beginning of one day, first migrate
        */
        scheduler.declareANewDay();
        scheduler.oneDayMigration();
//        cout << "****** day:" << i << endl;
        cout <<  "(migration, " << scheduler.get_migrateVMNumPerDay() << ")" << endl;
        scheduler.printMigrateInfo();
        getline(infile, s);
        fflush(stdin);
        int reqNum = stoi(s);
        vector<shared_ptr<VirtualMachine>> oneDayAddVM;
        vector<int> oneDayDelVM;
        for (int j = 0; j < reqNum; j++)
        {
            getline(infile, s);
            fflush(stdin);
            if (s.substr(0, 4) == "(add")
            {
                auto newvm = addNewVM(s, oneDayAddVM);
                scheduler.addVM_bystep(newvm);
            }
            else if (s.substr(0, 4) == "(del")
            {
                // cout << "del" << endl;
                int id_h = s.find(' ');
                int id_t = s.find(',', id_h);
                int index = stoi(s.substr(id_h + 1, id_t - id_h - 1));
                oneDayDelVM.push_back(index);
                scheduler.deleteVM(index);
            }
        }

        // scheduler.deleteVM(oneDayDelVM);
        // scheduler.addVM(oneDayAddVM);
        // if (i % 100 == 0)
        // {
        //     // cout << i << endl;
        // auto today_purchased_result = scheduler.getNewPurchasedHosts();
        // scheduler.getTodayMigration();
        // scheduler.getTodayAddVMArrangment(oneDayAddVM);
        // }
        // exit(0);
    }
    // scheduler.checkVMS();
}

int main()
{
    ios::sync_with_stdio(false);
    clock_t start, end;
    //定义clock_t变量
    start = clock();
//    string testDataName = "/home/czy/MyWorkSpace/JobHunter/training-data/training-1.txt";
    string testDataName = "../training-data/training-1.txt";
    string testDataName_2 = "../training-data/training-2.txt";
    //dcout << "Begin to schdule!" << endl;
    //    ifstream infile(testDataName, ios::in);
    // return 0;
    Scheduler scheduler;
    readFile(testDataName, scheduler);
    Scheduler scheduler_2;
    readFile(testDataName_2, scheduler_2);
    // readOj(scheduler);

    //	while(true){
    //		scheduler.deletVM(vector<shared_pt<VirtualMachine>ptr);
    //		scheduler.add(vector);
    //	}

    end = clock();   
    unsigned long long  cost1 = scheduler.getCost();
    unsigned long long  cost2 = scheduler_2.getCost();
    cout << "cost1: " << cost1 << endl;     
    cout << "cost2: " << cost2 << endl;
    cout << "sum cost: " << cost1 + cost2 << endl;

    // cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl; //输出时间（单位：ｓ）

    return 0;
}
int old_main()
{
	// using namespace std;
	// string testdataname = "../../training-data/training-1.txt";
	// ifstream infile(testdataname, ios::in);
    
    // assert(infile.is_open());   //若失�则输出错误消�并终止程序运�	unordered_map<string, pair<int, int>> vms;
	// unsigned long long cpu_req = 0, mem_req = 0;
	// string s;
	// int cnt = 0;
	// while (getline(infile, s))
	// {
	// 	if(s[0]!='('){
	// 		cnt++;
	// 		if (cnt % 10 == 0)
	// 			cout << "--day " << cnt << "-- cpu req: " << cpu_req << " mem_req: " << mem_req << endl;
	// 	}else if(s.substr(0, 4) == "(add")
	// 	{
	// 		int id_h = s.find(' ');
	// 		int id_t = s.find(',', id_h);
	// 		string id = s.substr(id_h + 1, id_t - id_h - 1);
	// 		auto itr = vms.find(id);
	// 		if (itr != vms.end())
	// 		{
	// 			cpu_req += itr->second.first;
	// 			mem_req += itr->second.second;
	// 		}
	// 	}
	// 	else if (s.substr(0, 4) == "(del")
	// 	{
	// 		int id_h = s.find(' ');
	// 		int id_t = s.find(',', id_h);
	// 		string id = s.substr(id_h + 1, id_t - id_h - 1);
	// 		auto itr = vms.find(id);
	// 		if (itr != vms.end())
	// 		{
	// 			cpu_req -= itr->second.first;
	// 			mem_req -= itr->second.second;
	// 		}
	// 	}
	// 	else if(s.substr(0, 3) == "(vm")
	// 	{
	// 		int head = s.find('v');
	// 		int tail = s.find(',');

	// 		string id = s.substr(head, tail - head);
	// 		int cpu_h = s.find(' ');
	// 		int cpu_t = s.find(',', cpu_h);
	// 		int cpu = stoi(s.substr(cpu_h + 1, cpu_t - cpu_h - 1));
	// 		int mem_h = s.find(' ', cpu_t);
	// 		int mem_t = s.find(',', mem_h);
	// 		int mem = stoi(s.substr(mem_h + 1, mem_t - mem_h - 1));
	// 		pair<int, int> value(cpu, mem);
	// 		cout << id << '\t' << cpu << '\t' << mem << endl;
	// 		vms.insert(make_pair(id, value));
	// 	}
	// }
	// TODO:read standard input
	// TODO:process
	// TODO:write standard output
	// TODO:fflush(stdout);


	// while()
	return 0;
}
