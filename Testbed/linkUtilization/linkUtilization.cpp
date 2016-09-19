// 思路：对于每个边，计算所有经过该边的流的总流量，然后根据最大流完成时间计算出最大流量，其比例就是该条边的利用率，最后算平均即可
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <cmath>
#include <cstdlib>

using namespace std;

#define INPUT_PATH_FILE_NAME "path.txt"
#define INPUT_TIME_FILE_NAME "time.txt"     // Don't forget to pre-process it
#define OUTPUT_FILE_NAME "utilization_rate.txt"
#define RATE_MULTIPLIER 15

struct route_rec
{
	int src, dst;
	double demand;
	int hops[10];
	int hops_count = 0;
	int port_flag = 0;		// 用于指示是不是multipath
	int port = 0;
};
route_rec records[1000];
int rec_count = 0;
double max_time = 0.0;
double util_rate[10][10] = {0};

int main(int argc, char const *argv[]) {
    // read path file
    ifstream fin(INPUT_PATH_FILE_NAME);
    if(!fin.is_open())
        cout << "Open path file failed!" << endl;

    // 循环直至文件读完
	string line;
    while(getline(fin, line))
    {
        string tmp;
		int i = 0;
		int t;

		// 读第一行
		istringstream ssin(line);
        ssin >> records[rec_count].src >> tmp >> records[rec_count].dst >> tmp >> records[rec_count].demand;

		// 读第二行
		getline(fin, line);
		ssin.clear();
		ssin.str(line);
		while (ssin >> t)
		{
			if (t >= 10)
				records[rec_count].hops[records[rec_count].hops_count++] = t % 10;
		}
		rec_count++;
    }
	fin.close();

	// 读取时间，并获取最大完成时间
	fin.open(INPUT_TIME_FILE_NAME);
	if(!fin.is_open())
		cout << "Open time file failed!" << endl;
	double t = 0;
	for(int i = 0; i < rec_count; i++)
	{
        fin >> t;
        if(t > max_time)
            max_time = t;
    }
	fin.close();

    // 针对每个路由记录，遍历其经过的边，统计流量
    // 只统计单向，反向以ACK报文为主，忽略不计
    for(int i = 0; i < rec_count; i++)
    {
        for (int j = 0; j < records[i].hops_count - 1; j++)
            util_rate[records[i].hops[j]][records[i].hops[j + 1]] += records[i].demand;
    }

    // 计算比率
    double max_throughput = max_time * 9414;        // Mbps
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            util_rate[i][j] = util_rate[i][j] * RATE_MULTIPLIER * 8 / max_throughput;

    // 输出到文件，输出成可以直接粘贴到matlab中的矩阵格式
    ofstream fout(OUTPUT_FILE_NAME);
    fout << "[" << endl;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
            fout << util_rate[i][j] << " ";
        fout << endl;
    }
    fout << "]";

    return 0;
}
