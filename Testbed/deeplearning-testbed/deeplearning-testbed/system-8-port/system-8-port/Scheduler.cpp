#include "Scheduler.h"
#include "Demand.h"
#include "Flow.h"
#include "basic.h"
#include "Dijkstra.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

Scheduler::Scheduler(){}
Scheduler::~Scheduler(){}
int Scheduler::schedule(Topology* topo, Demand* demand, Flow* f, bool show = false)
{
	//get size of matrix
	int size = topo->getSize();
	//total flow rate of all link
	double** flow_rate = new double*[size];
	//some useless variable, used to collect some statistics
	int total_time = 0;
	int total_flow = 0;
	vector<int> finish_time;
	/*int route_cond[320][320];
	for(int i = 0;i < 320;i ++)
		for(int j = 0;j < 320;j ++)
			route_cond[i][j] = 0;*/
	for(int i = 0;i < size;i ++)
	{
		flow_rate[i] = new double[size];
	}
	for(int i = 0;i < size;i ++)
		for(int j = 0;j < size;j ++)
		{
			flow_rate[i][j] = 0;
		}
	int tick = 0;
	Flow* cur = f;
	//traverse linked list of flows to sum flow rate of all links

	while(cur != NULL)
	{
		total_flow ++;
		int hops = cur->getLen();
		int* chart = cur->getRoute();
		//chart is RouteChart of this flow
		for(int i = 0;i < hops;i ++)
			flow_rate[chart[i]][chart[i+1]] += cur->rate;
		cur = cur->next;
	}
	//cout << "Scheduling......" << endl;

	string path_fct = "..\\..\\Sample_Data\\flowCompletionTime.txt";
	ofstream out_fct(path_fct.c_str());
	while(f!=NULL)
	{
		//cout << "at tick: " << tick << endl;
		//check if collision
		/*
		for(int i = 0;i < size;i ++)
		{
			for(int j = 0;j < size;j ++)
				cout << flow_rate[i][j] << " ";
			cout << endl;
		}
		*/
		//check if congestion happened
		while(!check(flow_rate, size))
		{
			//traverse list and change flow rate
			cur = f;
			while(cur != NULL)
			{
				int len = cur->getLen();
				int* chart = cur->getRoute();
				for(int i = 0;i < len;i ++)
				{
					if(flow_rate[chart[i]][chart[i+1]] > MAX_RATE)
					{
						cur->rate /= 2;
						break;
					}
				}
				cur = cur->next;
			}
			
			//flow_rate stores link bandwidth utilization 
			//traverse again to build flow matrix
			for(int i = 0;i < size;i ++)
				for(int j = 0;j < size;j ++)
					flow_rate[i][j] = 0;
			cur = f;
			while(cur != NULL)
			{
				int hops = cur->getLen();
				int* chart = cur->getRoute();
				for(int i = 0;i < hops;i ++)
					flow_rate[chart[i]][chart[i+1]] += cur->rate;//flow rate on each second
				cur = cur->next;
			}
		}
		//YANG: the flow_rate checke finished. output flow_rate here to get link utilization
		//YANG: output flow.rate here
		//YANG: add outfile here
		
		if (show)
		{
			stringstream ss;
			ss << tick;
			string s = ss.str();
			string fpath = "..\\..\\Sample_Data\\link_utilization\\lu_" + s + ".txt";//store temprory topology
			ofstream out_lu(fpath.c_str());
	
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (flow_rate[i][j] != 0.0)
					{
						double total_rate = flow_rate[i][j] + flow_rate[j][i];
						out_lu << i << " " << j << " " << total_rate << " " << total_rate / MAX_RATE << endl;
					}
				}
			}
			out_lu.close();
		}

		//Flow::printlist(f);
		//transmit all flows
		//update flow state

		cur = f;
		Flow* last_flow = NULL;
		while(cur != NULL)
		{	
			cur->remain -= cur->rate;
			//transmit finish
			if(cur->remain <= 0)
			{
				//output flow completion time
				//output order: src, dest, origin size, fct
				if (show)
				{
					out_fct << cur->getSrc() << " " << cur->getDest() << " " << cur->origin << " " << tick << endl;
				}
					

				finish_time.push_back(tick);
				total_time += tick;
				if(cur == f)
				{
					f = cur->next;
					delete cur;
					cur = f;
				}
				else
				{
					cur = cur->next;
					delete last_flow->next;
					last_flow->next = cur;
				}
				
				continue;
			}
			cur->rate += START_RATE;
			last_flow = cur;
			cur = cur->next;
		}

		//out_fct.close();

		for(int i = 0;i < size;i ++)
			for(int j = 0;j < size;j ++)
				flow_rate[i][j] = 0;
		cur = f;
		while(cur != NULL)
		{
			int hops = cur->getLen();
			int* chart = cur->getRoute();
			for(int i = 0;i < hops;i ++)
				flow_rate[chart[i]][chart[i+1]] += cur->rate;
			cur = cur->next;
		}
		tick++;
		//system("pause");
	}
	if(0)
	{
		/**/
		for(int i = 0;i < finish_time.size();i ++)
			cout << finish_time[i] << " ";
		cout << endl;
	}
	for(int i = 0;i < size;i ++)
		delete[] flow_rate[i];
	delete[] flow_rate;
	//cout << "***************total time " << tick << " ***************" << endl;
	return tick;
}
//create flows based on topology and demand
Flow* Scheduler::addFlows(Topology* t, Demand* d, Flow* f, int time, int way)
{

	string routePath = "..\\..\\Sample_Data\\routePath.txt";
	ofstream out_rp(routePath.c_str());

	if(t->getSize() != d->getSize()) 
	{
		cout << "Topology and demand have different size!";
		return f;
	}
	double** matrix = NULL;
	double** dist = NULL;
	int const size = t->getSize();

	matrix = new double*[size];
	dist = new double*[size];
	for(int i = 0;i < size;i ++)
	{
		matrix[i] = new double[size];
		dist[i] = new double[size];
	}
	//Dijkstra require input of topo is 1 for connected -1 for disconnected
	for(int i = 0;i < size;i ++)
		for(int j = 0;j < size;j ++)
		{
			bool connected = t->get(i,j);
			matrix[i][j] = connected?1.0:-1.0;
		}
	Flow* node = f;
	
	//useless variable to collect some statistics
	//int routeCount[size][size];
	//use dijkastra to find shortest length and paths
	for(int i = 0;i < size;i ++)
	{
		vector<int> prev[320];
		vector<vector<int> > route[320];
		//for each node i, find shortest way to other nodes and their precursor node
		Dijkstra(matrix, dist[i], i, size, prev);
		//create route chart
		findAllRoute(i, size, prev, route);//route stores all shortest paths between i and j
		for(int j = 0;j < size;j ++)
		{
			//create flow
			double demand = d->get(i,j);
			/*if(route[j].size()!=0) routeCount[i][j] = route[j][0].size();
			else routeCount[i][j] = 0;
			routeCond[i][j] = route[j].size();*/
			if(demand != 0)
			{
				if(i == j) continue;
				int ways = route[j].size();
				
				//cout << "from " << i << " to " << j << " has " << ways << " route at length of " << route[j][0].size() << endl;
				for(int k = 0;k < ways;k ++)
				{
					vector<int> chart = route[j][k];
					chart.push_back(j);//chart stores complete path nodes
					Flow* newflow = new Flow(demand/ways,i,j,time);//flow starting from i to j is existing; time = 0.
					newflow->setRoute(chart,(int)dist[i][j]);
					
					//YANG: output routing detail
					out_rp << i << " to " << j << " : " << demand / ways << endl;
					for (int i = 0; i < chart.size(); i++)
					{
						out_rp << chart[i] << " ";
					}
					out_rp << endl;


					if(!node)
					{
						f = node = newflow;
					}
					else
					{
						node -> next = newflow;
						node = node -> next;
					}
				}
			}
		}
	}
	
	for(int i = 0;i < size;i ++)
		delete[] matrix[i];
	delete[] matrix;
	for(int i = 0;i < size;i ++)
		delete[] dist[i];
	delete[] dist;
	return f;
}

bool Scheduler::check(double** matrix, int size)
{
	for(int i = 0;i < size;i ++)
		for(int j = 0;j < i;j ++)
		{
			if(matrix[i][j] > MAX_RATE)
				return false;
		}
	return true;
}