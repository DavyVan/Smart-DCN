#ifndef TOPO
#define TOPO
#include "basic.h"
/*
Topology is a class used to describe a graph
topo is used to save connection of nodes, topo[i][j] means if node i and j is connected
size is the number of nodes, alse size of topo
get is used to get if node i and j are connected
set is used to set connection between i and j. return true if set successfully, if i or j is out of index, return false.
*/
class Topology
{
public:
	Topology(int);
	Topology(int, int**);
	Topology(char*);
	~Topology();
	bool get(int, int);
	bool set(int, int, bool);
	int getSize();
	void print();
private:
	bool **topo;
	int size;
};

#endif