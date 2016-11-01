#ifndef SCHEDULER
#define SCHEDULER
#include "Topology.h"
#include "Demand.h"
#include "Flow.h"
#include "basic.h"

class Scheduler
{
public:
	Scheduler();
	~Scheduler();
	int schedule(Topology*, Demand*, Flow*, bool);
	static Flow* addFlows(Topology*, Demand*, Flow*, int = 0, int = APPENDLIST);
private:
	int time;
	bool check(double**, int);
};
#endif