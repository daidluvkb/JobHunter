#include <unordered_map>
#include "Host.h"
#include "VirtualMachine.h"
#include <vector>
class DpMemo
{

    unsigned long long _key; 
public:
    std::unordered_map<unsigned long long, unsigned int> costdict; //key, cost
    DpMemo() : _key(0) {}
    void resetKey() { _key = 0; }
    void setKey(const int index, bool included);

    unsigned long long makeKey() { return _key; }
    unsigned int getCost(const vector<int> &indexlist);
    void setCost(const vector<int> &indexlist, const int cost);
};