//
// Created by czy on 2021/3/16.
//

#ifndef CODECRAFT_2021_UTILS_H
#define CODECRAFT_2021_UTILS_H
#include <string>
#include <algorithm>
#define dcout 0 && cout//或者NULL && cout
// #define dcout cout


typedef struct HostInfo{
    int cpu;
    int mem;
    int basicCost;
    int dailyCost;
    std::string type;
}HostInfo;

typedef struct VMInfo{
    int cpu;
    int mem;
    bool isDouble;
}VMInfo;


#endif //CODECRAFT_2021_UTILS_H
