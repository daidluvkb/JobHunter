#include <iostream>
#include <string>

using namespace std;

class Server{
    private:
        string m_type;
        int m_num_of_cpu;
        int m_size_of_mem;
        const int m_cost_base;
        const int m_cost_perday;
    public:
        Server();
        Server(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday);
        
        int getNumOfCpu();
        int getSizeOfMem();
        int getCostBase();
        int getCostPerDay();
};