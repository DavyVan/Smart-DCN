/*
* by David FAN Quan
**** Text below are written in Chinese, charset: UTF-8
* 本程序的作用是生成描述所有固定链路和可配置链路的拓扑全集的矩阵
* 输入的是样本数据的任意输出拓扑，输出为80*80的方阵
* 其中32个ToR，32个Aggressive交换机，16个Core交换机
*/

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define ToR_NUM 32
#define SIZE 80

int returnSwitchLevelNum(int index)
{
    if(0 <= index && index < ToR_NUM)
        return 0;   //stand for ToR
    else if(ToR_NUM <= index && index < ToR_NUM + 32)
        return 1;   //stand for 1st level of switch: Aggressive
    else if(ToR_NUM + 32 <= index && index < SIZE)
        return 2;   //stand for 2st level of switch: Core
    else
        return -1;  //Error
}

int returnOutputLayerNum(int a, int b)      //stand for a element's row index and column index
{
    return max(returnSwitchLevelNum(a), returnSwitchLevelNum(b)) + 1;
}

int main(int argc, char const *argv[]) {

    //Variables initialization
    int fullTopo[SIZE][SIZE] = {0};

    //read a topo from file called "input_topo.txt"
    ifstream input;
    input.open("input_topo.txt");

    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            input >> fullTopo[i][j];
    input.close();

    //set ToRs' configurable links
    for(int i = 0; i < ToR_NUM; i++)
        for(int j = 0; j < ToR_NUM; j++)
            if(i == j)
                fullTopo[i][j] = 0;
            else
                fullTopo[i][j] = 1;

    //compute the layer number
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            if(fullTopo[i][j] == 1)
                fullTopo[i][j] = returnOutputLayerNum(i, j);

    //output the fullTopo to file called "output_full_topo.txt"
    ofstream output;
    output.open("output_full_topo.txt");

    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            if(j == 0)
                output << fullTopo[i][j];
            else
                output << " " << fullTopo[i][j];
        }
        output << endl;
    }
    output.close();

    return 0;
}
