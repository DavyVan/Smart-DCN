#include "Topology.h"
#include<fstream>
#include <iostream>
using namespace std;

//there is no comments because it's so easy

Topology::Topology(int s):size(s)
{
	topo = new bool*[s];
	for(int i = 0;i < s;i ++)
		topo[i] = new bool[s];
}

Topology::Topology(int s, int** t):size(s)
{
	topo = new bool*[s];
	for(int i = 0;i < s;i ++)
		topo[i] = new bool[s];
	for(int i = 0;i < s;i ++)
		for(int j = 0;j < s;j ++)
			topo[i][j] = t[i][j];
}

Topology::Topology(char* filename)
{
	size = 0;
	topo = NULL;
	ifstream in(filename);
	if(!in)
	{
		cout << "Fail when create Topology from file" << endl;
		return;
	}
	in >> size;
	topo = new bool*[size];
	for(int i = 0;i < size;i ++)
		topo[i] = new bool[size];
	int c;
	for(int i = 0;i < size;i ++)
		for(int j = 0;j < size;j ++)
		{
			in >> c;
			if(c == 1) topo[i][j] = true;
			else topo[i][j] = false;
		}
	in.close();
}

Topology::~Topology()
{
	for(int i = 0;i < size;i ++)
		delete[] topo[i];
	delete[] topo;
}

int Topology::getSize()
{
	return size;
}

bool Topology::get(int i, int j)
{
	if(i < size && j < size)
		return topo[i][j];
	return false;
}
bool Topology::set(int i, int j, bool situation)
{
	if(i < size && j < size)
	{
		topo[i][j] = situation;
		return true;
	}
	return false;
}

void Topology::print()
{
	cout << "***************print Topology strart***************" << endl;
	cout << "size: " << size << endl;
	for(int i = 0;i < size;i ++)
	{
		for(int j = 0;j < size;j ++)
			cout << topo[i][j] << " ";
		cout << endl;
	}
	cout << "*************** print Topology end***************" << endl;
}