//
// Created by czy on 2021/3/16.
//

#ifndef CODECRAFT_2021_UTILS_H
#define CODECRAFT_2021_UTILS_H

typedef struct HostInfo{
    int cpu;
    int mem;
    int basicCost;
    int dailyCost;
}HostInfo;

typedef struct VMInfo{
    int cpu;
    int mem;
    bool isDouble;
}VMInfo;


#endif //CODECRAFT_2021_UTILS_H
