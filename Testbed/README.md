# Testbed
## cmd
各种实验所需要的配置命令  
- 光交换机配置截图
- 电交换机路由配置命令  

详情见其中README文件
## configen
configuration generator  
用于生成电交换机路由的配置命令
详情见其中README文件
## deeplearning-testbed
根据demand生成搜索算法拓扑和路由  
该版本程序根据testbed特殊要求修改过，不能直接适用于适用其他情景  
### 使用方法
将demand文件命名为demand0.txt放在Sample_Data/Demand中即可  
运行程序时应打开Release模式
## Edmond-testbed
根据demand利用最大权重匹配算法（Edmond）生成拓扑和路由，其中：
- deeplearning-testbed：这部分是经过改造的，仅仅利用了其生成路由的功能，并不会启动搜索算法
- Edmond_4：这里实现了Edmond算法，只会生成拓扑

### 使用方法
将demand文件命名为demand0.txt放在Sample_data/edmond_4/demand[0-4]中，以及deeplearning-testbed/Sample_Data/Demand中  
依次运行demand和deeplearning程序即可  
## linkUtilization
用于计算链路利用率，计算的范围是所有链路，包括server和交换机之间的链路
## Equipments.txt
记录了位于HKUST的testbed的硬件配置和参考价格
## demandGenerator.m
生成demand，服从均匀随机分布，可以设置密度
生成的demand会输出到deeplearning-testbed和Edmond-testbed中，该文件应和上述两个工程文件夹放在一起，否则不能正常输出
## MegaSwitchTestbed.docx
这是HKUST曾高雄同学给我的参考资料，里面描述了MegaSwitch拓扑的配置方式
## testbed-structure.vsdx
testbed结构图
