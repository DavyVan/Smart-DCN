/*
* 输入文件需要预处理！多跳情况中的原地踏步要去掉！
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <cmath>
#include <cstdlib>

using namespace std;

#define INPUT_FILE_NAME "path.txt"
#define ROUTE_CMD_FILE_NAME "route_cmd.txt"
#define RUN_CMD_FILE_NAME "run_cmd.txt"
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

int next_port[10] = { 0 };
char const* filenames[5] = { "piac8-1-br0.txt", "pica8-1-br1.txt", "pica8-2-br0.txt", "pica8-2-br1.txt", "pica8-2-br2.txt" };
char const* cmd_brs[5] = { "br0", "br1", "br0", "br1", "br2" };
int server_in_port[10] = { 23, 24, 15, 16, 17, 19, 20, 5, 6, 7 };
int server_base_port[10] = { 33, 41, 25, 33, 41, 37, 45, 29, 37, 45 };
char const* IPs[10] = {
	"192.168.110.13",
	"192.168.110.14",
	"192.168.110.15",
	"192.168.110.16",
	"192.168.110.17",
	"192.168.110.5",
	"192.168.110.6",
	"192.168.110.3",
	"192.168.110.7",
	"192.168.110.8"
};


int main()
{
	int multipath_count = 1;		// 多路径组数，用于分配端口，5x01,5x02...
	for (int i = 0; i < 10; i++)
		next_port[i] = 5001;

    // 读文件
    ifstream fin(INPUT_FILE_NAME);
    if(!fin.is_open())
        cout << "Open file failed!" << endl;

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
        //cout << src << " " << dst << " " << demand << endl;

		// 读第二行
		getline(fin, line);
		ssin.clear();
		ssin.str(line);
		while (ssin >> t)
		{
			// cout << t << endl;
			if (t >= 10)
			{
				records[rec_count].hops[records[rec_count].hops_count++] = t % 10;
				//cout << hops[i - 1] << endl;
			}
		}
		rec_count++;
    }
	fin.close();

	// 打开文件
	ofstream fouts[5];
	for (int i = 0; i < 5; i++)
		fouts[i].open(filenames[i]);
	ofstream serverside[10];
	ofstream clientside[10];
	for (int i = 0; i < 10; i++)
	{
		char filename[50] = { 0 };
		//sscanf(filename, "%d-server-side.txt", i);
		serverside[i].open(to_string(i) + string("-server-side.txt"));
		//memset(filename, 0, sizeof(char) * 50);
		//sscanf(filename, "%d-client-side.txt", i);
		clientside[i].open(to_string(i) + string("-client-side.txt"));
	}

	// 针对每一个路由记录进行处理
	for (int i = 0; i < rec_count; i++)
	{
		// 分配端口
		if (records[i].port == 0)		// 如果没有分配过端口，因为在遇到multipath的时候会向后遍历更新端口，所以有可能已经分配了端口号
		{
			// 看下面一组，判断是否为multipath
			if (i < rec_count - 1 && records[i].src == records[i + 1].src && records[i].dst == records[i + 1].dst)
			{
				// 是multipath，开始向后遍历
				int index = 1;
				for (int j = i; j < rec_count; j++)
				{
					if (records[j].src == records[i].src && records[j].dst == records[i].dst)
					{
						records[j].port_flag = 1;
						records[j].port = 5000 + multipath_count * 100 + index;
						index++;
					}
					else
						break;
				}
				index = 1;
				for (int j = i; j < rec_count; j++)
				{
					if (records[j].src == records[i].dst && records[j].dst == records[i].src)
					{
						records[j].port_flag = 1;
						records[j].port = 5000 + multipath_count * 100 + index;
						index++;
					}
				}
				multipath_count++;
			}
			else
			{
				// 不是multipath，每个server按照顺序分配
				records[i].port = next_port[records[i].dst];
				next_port[records[i].dst]++;
			}
		}

		// 路由命令，正向
		int offset = 0;
		for (int j = 0; j < records[i].hops_count - 1; j++)
		{
			int a = records[i].hops[j];
			int b = records[i].hops[j + 1];
			int in_port = (j == 0 ? server_in_port[a] : server_base_port[a % 5] + records[i].hops[j - 1] / 5 * 4 + offset - 1);
			int out_port;
			if (a % 5 < b % 5)
			{
				offset = b % 5 - a % 5;
				out_port = server_base_port[a] + offset - 1;
			}
			else if (a % 5 == b % 5 && records[i].hops_count == 2)		// 本地直达
			{
				out_port = server_in_port[b];
			}
			if (a % 5 > b % 5)
			{
				offset = b % 5 + 5 - a % 5;
				out_port = server_base_port[a] + offset - 1;
			}
			if (records[i].port_flag == 0)
				fouts[a % 5] << "ovs-ofctl add-flow " << cmd_brs[a % 5] << " in_port=" << in_port << ",dl_type=0x0800,nw_src=" << IPs[records[i].src] << ",nw_dst=" << IPs[records[i].dst] << ",actions=output:" << (out_port != in_port ? to_string(out_port) : "in_port") << endl;
			else
			{
				fouts[a % 5] << "ovs-ofctl add-flow " << cmd_brs[a % 5] << " in_port=" << in_port << ",dl_type=0x0800,nw_src=" << IPs[records[i].src] << ",nw_dst=" << IPs[records[i].dst] << ",nw_proto=6,tp_dst=" << records[i].port << ",actions=output:" << (out_port != in_port ? to_string(out_port) : "in_port") << endl;
				fouts[a % 5] << "ovs-ofctl add-flow " << cmd_brs[a % 5] << " in_port=" << in_port << ",dl_type=0x0800,nw_src=" << IPs[records[i].src] << ",nw_dst=" << IPs[records[i].dst] << ",nw_proto=6,tp_src=" << records[i].port << ",actions=output:" << (out_port != in_port ? to_string(out_port) : "in_port") << endl;
			}
		}
		// 反向
		for (int j = records[i].hops_count - 1; j > 0; j--)
		{
			int a = records[i].hops[j];
			int b = records[i].hops[j - 1];
			int in_port = (j == records[i].hops_count - 1 ? server_in_port[a] : server_base_port[a % 5] + records[i].hops[j + 1] / 5 * 4 + offset - 1);
			int out_port;
			if (a % 5 < b % 5)
			{
				offset = b % 5 - a % 5;
				out_port = server_base_port[a] + offset - 1;
			}
			else if (a % 5 == b % 5 && records[i].hops_count == 2)		// 本地直达
			{
				out_port = server_in_port[b];
			}
			if (a % 5 > b % 5)
			{
				offset = b % 5 + 5 - a % 5;
				out_port = server_base_port[a] + offset - 1;
			}
			if (records[i].port_flag == 0)
				fouts[a % 5] << "ovs-ofctl add-flow " << cmd_brs[a % 5] << " in_port=" << in_port << ",dl_type=0x0800,nw_src=" << IPs[records[i].dst] << ",nw_dst=" << IPs[records[i].src] << ",actions=output:" << (out_port != in_port ? to_string(out_port) : "in_port") << endl;
			else
			{
				fouts[a % 5] << "ovs-ofctl add-flow " << cmd_brs[a % 5] << " in_port=" << in_port << ",dl_type=0x0800,nw_src=" << IPs[records[i].dst] << ",nw_dst=" << IPs[records[i].src] << ",nw_proto=6,tp_dst=" << records[i].port << ",actions=output:" << (out_port != in_port ? to_string(out_port) : "in_port") << endl;
				fouts[a % 5] << "ovs-ofctl add-flow " << cmd_brs[a % 5] << " in_port=" << in_port << ",dl_type=0x0800,nw_src=" << IPs[records[i].dst] << ",nw_dst=" << IPs[records[i].src] << ",nw_proto=6,tp_src=" << records[i].port << ",actions=output:" << (out_port != in_port ? to_string(out_port) : "in_port") << endl;
			}
		}

		// 运行命令
		// server side
		serverside[records[i].dst] << "iperf3 -p " << records[i].port << " -s -D -I ~/pid_" << records[i].port << ".txt" << endl;
		// client side
		clientside[records[i].src] << "iperf3 -p " << records[i].port << " -f m -i 0 -c " << IPs[records[i].dst] << " -V -n " << (int)round(records[i].demand * RATE_MULTIPLIER) << "M --logfile ~/result-" << records[i].src << "-" << records[i].dst << "-" << records[i].port << ".txt" << endl;


		//system("pause");
		//return 0;
	}

	//关闭文件
	for (int i = 0; i < 5; i++)
		fouts[i].close();
	for (int i = 0; i < 10; i++)
	{
		serverside[i].close();
		clientside[i].close();
	}
	//system("pause");
}
