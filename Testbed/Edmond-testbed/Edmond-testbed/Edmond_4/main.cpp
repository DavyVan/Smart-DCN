#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include <chrono>
#include <thread>
#include <utility>
#include "EdmondsMatching.hpp"
using namespace std;

#define BEAMWIDTH 5
#define K 8
#define ToRnum 10
#define Aggnum 10
#define corenum 0
#define nodenum ToRnum + Aggnum + corenum
#define SEARCHDEPTH 30
#define CHANGETIMES 20
#define SIZE ToRnum

void initialFixedLink(int topo[][nodenum])
{
	//YANG: each server connects to a toR switch
	for (int i = 0; i < ToRnum; i++)
	{
		topo[i][ToRnum + i] = 1;
		topo[ToRnum + i][i] = 1;
	}
}

void writeTopoArray2File(string path, int array[][nodenum], int size)
{
	ofstream f(path);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			f << array[i][j];
			f << " ";
		}
		f << endl;
	}
	f.close();
	cout << path << endl;
}
void writeDemandArray2File(string path, int array[][SIZE], int size)
{
	ofstream f(path);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			f << array[i][j];
			f << " ";
		}
		f << endl;
	}
	f.close();
	//cout << path << endl;
}


int main(int argc, char *argv[])
{

	int demandMatrix[ToRnum][ToRnum];
	int finalTopo[nodenum][nodenum];
	for (int count = 0; count < 1; count++)
	{
		int time = 0;
		for (int i = 0; i < nodenum; i++)
		for (int j = 0; j < nodenum; j++)
			finalTopo[i][j] = 0;
		string topoPath = "..\\..\\Sample_data\\edmond_4\\topo\\topo";

		for (time = 0; time < 1; time++)		// Fan Quan: 1 ToR can touch only 1 other ToR now.
		{
			//string path1 = "F:\\Learning\\demand0\\demand0.txt";
			string path = "..\\..\\Sample_data\\edmond_4\\demand";
			string outDemandPath = "..\\..\\Sample_data\\edmond_4\\demand";
			for (int i = 0; i < ToRnum;i++)
				for (int j = 0; j < ToRnum; j++)
					demandMatrix[i][j] = 0;
			stringstream ss;
			ss << count;
			string s = ss.str();

			stringstream timess;
			timess << time;
			string times = timess.str();

			stringstream timess2;
			timess2 << (time + 1);
			string times2 = timess2.str();

			path += times + "\\demand" + s + ".txt";
			cout << path << endl;

			// Fan Quan: What this means???? Generate topoPath when time==0
			if (time == 0)
				topoPath += s + ".txt";
			outDemandPath += times2 + "\\demand" + s + ".txt";
			ifstream input(path, ios::in);
			EdmondsMatching matching(ToRnum);
			map<pair<int, int>, int> edges;
			int round = (time== 0 ? ToRnum:ToRnum);
			for (int i = 0; i < round; i++)
			{
				for (int j = 0; j < round; j++)

				{
					int v1, v2, w;
					v1 = i;
					v2 = j;
					input >> w;
					if(w >= 0) w = -w;
					
					if(i < ToRnum && j < ToRnum)
					{
					demandMatrix[i][j] = w;
					//if (v1 > v2)
					//	swap(v1, v2);
					if (v1 != v2 && v1 - v2 != 1 && v2 - v1 != 1)		// Fan Quan: We don't add ring-base into edges
					{
						edges[make_pair(v1, v2)] = w;
						matching.addEdge(v1, v2, w);
					}
					}
				}
			}
			for (int i = 0; i < ToRnum; i++)
			{
				for (int j = 0; j < ToRnum; j++)
				{
					cout << demandMatrix[i][j] << " ";
				}
				cout << endl;
			}
			bool success = matching.findMinimumWeightMatching();
			auto finalEdges = matching.getMatchedEdges();
			for (auto e : finalEdges)
			{
				int i = e.first;
				int j = e.second;
				finalTopo[i+ToRnum][j+ToRnum] = 1;
				finalTopo[j+ToRnum][i+ToRnum] = 1;
				demandMatrix[i][j] = 0;
				demandMatrix[j][i] = 0;
				cout << i << " " << j << endl;
			}
			writeDemandArray2File(outDemandPath, demandMatrix, ToRnum);
				
		}
		// Fan Quan: Add fixed ring base into finalTopo
		for (int i = 0; i < ToRnum; i++)
		{
			finalTopo[i + ToRnum][(i + 1) % ToRnum + ToRnum] = 1;
			finalTopo[(i + 1) % ToRnum + ToRnum][i + ToRnum] = 1;
		}
		initialFixedLink(finalTopo);
		writeTopoArray2File(topoPath, finalTopo, nodenum);
	}
	system("pause");
	return 0;
}
