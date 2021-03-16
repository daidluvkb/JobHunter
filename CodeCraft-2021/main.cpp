#include "iostream"
#include <string>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include "Scheduler.h"
#include "VirtualMachine.h"
bool todayHasRequest
(

);

int main()
{
	readFile();
	Scheduler scheduler;
	while(true){
		scheduler.deletVM(vector<shared_pt<VirtualMachine>ptr);
		scheduler.add(vector);
	}
	return 0;
}
int old_main()
{
	using namespace std;
	string testdataname = "../../training-data/training-1.txt";
	ifstream infile(testdataname, ios::in);

    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行
	unordered_map<string, pair<int, int>> vms;
	unsigned long long cpu_req = 0, mem_req = 0;
	string s;
	int cnt = 0;
	while (getline(infile, s))
	{
		if(s[0]!='('){
			cnt++;
			if (cnt % 10 == 0)
				cout << "--day " << cnt << "-- cpu req: " << cpu_req << " mem_req: " << mem_req << endl;
		}else if(s.substr(0, 4) == "(add")
		{
			int id_h = s.find(' ');
			int id_t = s.find(',', id_h);
			string id = s.substr(id_h + 1, id_t - id_h - 1);
			auto itr = vms.find(id);
			if (itr != vms.end())
			{
				cpu_req += itr->second.first;
				mem_req += itr->second.second;
			}
		}
		else if (s.substr(0, 4) == "(del")
		{
			int id_h = s.find(' ');
			int id_t = s.find(',', id_h);
			string id = s.substr(id_h + 1, id_t - id_h - 1);
			auto itr = vms.find(id);
			if (itr != vms.end())
			{
				cpu_req -= itr->second.first;
				mem_req -= itr->second.second;
			}
		}
		else if(s.substr(0, 3) == "(vm")
		{
			int head = s.find('v');
			int tail = s.find(',');

			string id = s.substr(head, tail - head);
			int cpu_h = s.find(' ');
			int cpu_t = s.find(',', cpu_h);
			int cpu = stoi(s.substr(cpu_h + 1, cpu_t - cpu_h - 1));
			int mem_h = s.find(' ', cpu_t);
			int mem_t = s.find(',', mem_h);
			int mem = stoi(s.substr(mem_h + 1, mem_t - mem_h - 1));
			pair<int, int> value(cpu, mem);
			cout << id << '\t' << cpu << '\t' << mem << endl;
			vms.insert(make_pair(id, value));
		}
	}
	// TODO:read standard input
	// TODO:process
	// TODO:write standard output
	// TODO:fflush(stdout);


	// while()
	return 0;
}
