#include "Server.h"
Server::Server(const string& type, int num_of_cpu, int size_of_mem, int cost_base, int cost_perday) 
:_type(type), _num_of_cpu(num_of_cpu), _size_of_mem(size_of_mem), _cost_base(cost_base), _cost_perday(cost_perday)
{
    
}
