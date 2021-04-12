# 2021华为 Huawei CodeCraft比赛现场复赛版
## Requirement
* g++
* cmake above 2.8 
## Build
    git clone 
    bash build.sh
## 类和对象
### 服务器Server
### 虚拟机VirtualMachine
### 调度器Scheduler
### Request
* create
* delete

## TODO
1. 配置开发环境（recommend：win10 linux子系统）
2. 实现输入输出
    - [ ] InputStringParser
    - [ ] OutputEncoder
2. 实现对象
    - [ ] Server
    - [ ] Scheduler
    - [ ] VirtualMachine
    - ...
3. 实现任务书描述的调度子策略
    - [ ] 服务器空间不够，买新的
    - [ ] 服务器空间够，移动虚拟机
    - [ ] 关掉cost_perday大的服务器
4. 优化
    - [ ] 选择性价比高的服务器
    - [ ] 最大负荷的一天需要的服务器量
