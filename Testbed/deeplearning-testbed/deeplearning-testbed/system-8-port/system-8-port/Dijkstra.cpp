#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include "Dijkstra.h"
using namespace std;
//#define N 5

/*
Dijkstra algorithm is used to calculate shortest length for a graph, the input is a weight matrix.
This algorithm requires all weights are positive number, so if there is no direct link from i to j,let matrix[i][j] < 0 
*/

//calculate shortest distance for a node
void Dijkstra(double** matrix, double* res, int node, int nodeNum)
{
	//cout << "node " << node << endl;
	int remain_node = nodeNum;
	bool visited[nodenum];//YANG: replace nodeNum with nodenum
	for(int i = 0;i < nodeNum;i ++)
	{
		visited[i] = false;
		res[i] = matrix[node][i];
	}
	visited[node] = true;
	res[node] = 0;
	remain_node --;
	while(remain_node != 0)
	{
		int mini_dist = -1;
		int mini_node = -1;
		//find an unvisted node with shortest distance
		for(int i = 0;i < nodeNum;i ++)
		{
			if(!visited[i] && res[i] > 0 && (mini_dist < 0 || mini_dist > res[i]))
			{
				mini_dist = res[i];
				mini_node = i;
			}
		}
		//no node to update, the graph is not connected
		if(mini_node < 0) break;
		visited[mini_node] = true;
		//update distance
		for(int i = 0;i < nodeNum;i ++)
		{
			if(!visited[i] && matrix[mini_node][i] > 0 && (res[i] < 0 || res[i] > mini_dist + matrix[mini_node][i]))
				res[i] = mini_dist + matrix[mini_node][i];
		}
		remain_node --;
	}
}


//matrix is the matrix of weight, res is used for record result, N is number of nodes
void Dijkstra(double** matrix, double** res, const int nodeNum)
{
	//calculate shortest distance for each node
	for(int i = 0;i < nodeNum;i ++)
		Dijkstra(matrix, res[i], i, nodeNum);
}

int* findway(double** matrix, double** dist, const int size, int x, int y)
{
	//cout << "find way in " << x << " and " << y << endl;
	/*
	for(int i = 0;i < size;i ++)
	{
		for(int j = 0;j< size;j ++)
			cout << dist[i][j] << " ";
		cout << endl;
	}
	*/
	static bool flag = false;
	if(!flag)
	{
		srand((int)time(NULL));
		flag = true;
	}
	int hop = (int)dist[x][y];
	int* res = new int[hop+1];
    int node = x;
	res[0] = node;
	for(int i = 1;i < hop + 1;i ++)
	{
		int post_num = 0;
		for(int j = 0;j < size;j ++)
		{
			if(node == j) continue;
			if(matrix[node][j] > 0 && matrix[node][j] + dist[j][y] == dist[node][y])
				post_num ++;
		}
		if(!post_num)
		{
			cout << "error when find routing" << endl;
			return NULL;
		}
		int post_pos = rand() % post_num;
		post_num = 0;
		for(int j = 0;j < size;j ++)
		{
			if(node == j) continue;
			if(matrix[node][j] > 0 && matrix[node][j] + dist[j][y] == dist[node][y])
			{
				
				if(post_num == post_pos)
				{
					res[i] = j;
					node = j;
					//cout << "find " << j << endl;
					break;
				}
				post_num ++;
			}
		}
	}
	//int distance = (int)dist[x][y];
	return res;
}

void Dijkstra(double** matrix, double* res, int node, const int nodeNum, vector<int> *prev)
{
	//cout << "start" << endl;
	bool visited[nodenum];//YANG: replace nodeNum with nodenum
	int remain_node = nodeNum;
	for(int i = 0;i < nodeNum;i ++) 
	{
		visited[i] = false;
		res[i] = matrix[node][i];
		if(res[i] > 0) prev[i].push_back(node);
	}
	res[node] = 0;
	visited[node] = true;
	remain_node --;
	while(remain_node != 0)
	{
		int mini_dist = -1;
		int mini_node = -1;
		//find an unvisted node with shortest distance
		for(int i = 0;i < nodeNum;i ++)
		{
			if(!visited[i] && res[i] > 0 && (mini_dist < 0 || mini_dist > res[i]))
			{
				mini_dist = res[i];
				mini_node = i;
			}
		}
		//no node to update, the graph is not connected
		if(mini_node < 0) break;
		visited[mini_node] = true;
		//update distance
		for(int i = 0;i < nodeNum;i ++)
		{
			if(!visited[i] && matrix[mini_node][i] > 0 && (res[i] < 0 || res[i] >= mini_dist + matrix[mini_node][i]))
			{
				if(res[i] < 0 || res[i] > mini_dist + matrix[mini_node][i])
				{
					res[i] = mini_dist + matrix[mini_node][i];
					prev[i].clear();
				}
				prev[i].push_back(mini_node);
				
			}
		}
		remain_node --;
	}
}

void findRoute(int node, int dest, const int nodeNum, vector<int> *prev, vector<vector<int> > *route)
{
	if(node == dest) return;
	if(route[dest].size() != 0) return;
	for(vector<int>::iterator it = prev[dest].begin();it != prev[dest].end();it ++)
	{
		int p = *it;
		if(p == node) 
		{
			vector<int> v;
			v.push_back(p);
			route[dest].push_back(v);
		}
		else
		{
			findRoute(node, p, nodeNum, prev, route);
			for(vector<vector<int> >::iterator iter = route[p].begin();iter != route[p].end();iter ++)
			{
				vector<int> new_chart(*iter);
				new_chart.push_back(p);
				route[dest].push_back(new_chart);
			}
		}

	}
}


void findAllRoute(int node, const int nodeNum, vector<int> *prev, vector<vector<int> > *route)
{
	for(int i = 0;i < nodeNum;i ++)
	{
		if(node == nodeNum) continue;
		findRoute(node, i, nodeNum ,prev, route);
	}
}

