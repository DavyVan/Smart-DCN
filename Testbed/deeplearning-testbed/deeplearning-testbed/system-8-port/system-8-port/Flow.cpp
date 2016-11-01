#include "Flow.h"
#include "basic.h"
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
Flow::Flow(double r, int s, int d, int time):remain(r),src(s),dest(d),createTime(time)
{
	origin = r;
	hop = 0;
	rate = START_RATE;
	next = NULL;
	routeChart = NULL;
}
Flow::~Flow()
{
	if(routeChart)
		delete[] routeChart;
}
int Flow::getSrc(){return src;}
int Flow::getDest(){return dest;}
int* Flow::getRoute(){return routeChart;}
int Flow::getLen(){return hop;}
void Flow::setRoute(int* chart, int h)
{
	routeChart = chart;
	hop = h;
}
void Flow::setRoute(vector<int> chart, int h)
{
	routeChart = new int[h+1];
	for(int i = 0;i < h+1;i ++)
		routeChart[i] = chart[i];
	hop = h;
}
void Flow::print()
{
	cout << "Flow: " << src << " to " << dest <<" create at " << createTime << endl;
	cout << "remain " << remain << " at rate " << rate << endl;
	cout << "ruote chart:";
	if(routeChart != NULL)
	{
		for(int i = 0;i <= hop;i ++)
			cout << routeChart[i] << " ";
	}
	else
	{
		cout << "empty yet" << endl;
	}
	cout << endl;
}
void Flow::printlist(Flow* f)
{
	cout << "***************print flowlist start***************" << endl;
	Flow* cur = f;
	while(cur != NULL)
	{
		cur->print();
		cur = cur->next;
	} 
	cout << "***************print flowlist end***************" << endl;
}

bool Flow::contain(int x, int y)
{
	for(int i = 0;i < hop;i ++)
	{
		if(routeChart[i] == x && routeChart[i+1] == y)
			return true;
	}
	return false;
}