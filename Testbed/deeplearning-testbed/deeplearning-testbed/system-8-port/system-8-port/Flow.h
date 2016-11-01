#ifndef FLOW
#define FLOW
#include "basic.h"
/*
Flow is a class used to describe a flow
src is source node, dest is destination node
createTime is the time this flow is created
hop is the length of this flow's route
routeChart is a linked list describe how this flow routes
*/
#include <vector>

using namespace std;
class Flow
{
public:
	Flow(double, int, int, int);
	~Flow();
	Flow* next;
	int getSrc();
	int getDest();
	int* getRoute();
	int getLen();
	void setRoute(int*, int);
	void setRoute(vector<int>, int);
	void print();
	static void printlist(Flow* f);
	double rate;
	double remain;
	double origin;
	bool contain(int, int);

private:
	int src;
	int dest;
	int createTime;
	int hop;
	int* routeChart;

};
#endif