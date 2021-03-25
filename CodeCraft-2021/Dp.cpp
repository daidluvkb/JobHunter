#include "Dp.h"

void DpMemo::setKey(const int index, bool included)
{
    unsigned long long tmp = 1;
    if (included)
        _key |= (tmp << index);
    else
        _key &= ~(tmp << index);
    // printf("%llx", _key);
}

unsigned int DpMemo::getCost(const vector<int> &indexlist) 
{
    resetKey();
    for (size_t i = 0; i < indexlist.size(); i++)
    {
        setKey(indexlist[i], true);
    }
    return costdict[_key];
}

void DpMemo::setCost(const vector<int> &indexlist, const int cost) 
{
    resetKey();
    for (size_t i = 0; i < indexlist.size(); i++)
    {
        setKey(indexlist[i], true);
    }
    costdict[_key] = cost;
}
