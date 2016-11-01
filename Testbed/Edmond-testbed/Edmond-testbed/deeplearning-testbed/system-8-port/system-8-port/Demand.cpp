#include "Demand.h"
#include <fstream>
#include <iostream>
using namespace std;

Demand::Demand(int s):size(s)
{
	demand = new double*[s];
	for(int i = 0;i < s;i ++)
		demand[i] = new double[s];
	for(int i = 0;i < s;i ++)
		for(int j = 0;j < s;j ++)
			demand[i][j] = 0;

}

Demand::Demand(int s, int** d):size(s)
{
	demand = new double*[s];
	for(int i = 0;i < s;i ++)
		demand[i] = new double[s];
	for(int i = 0;i < s;i ++)
		for(int j = 0;j < s;j ++)
			demand[i][j] = d[i][j];
}

Demand::Demand(char* filename)
{
	size = 0;
	demand = NULL;
	ifstream in(filename);
	if(!in)
	{
		cout << "Fail when create Demand from file" << endl;
		return;
	}
	in >> size;
	demand = new double*[size];
	for(int i = 0;i < size;i ++)
		demand[i] = new double[size];
	for(int i = 0;i < size;i ++)
		for(int j = 0;j < size;j ++)
			in >> demand[i][j];
	in.close();
}

Demand::~Demand()
{
	for(int i = 0;i < size;i ++)
		delete[] demand[i];
	delete[] demand;
}

double Demand::get(int i, int j)
{
	if(i < size && j < size)
		return demand[i][j];
	return 0;
}
bool Demand::set(int i, int j, double d)
{
	if(i < size && j < size)
	{
		demand[i][j] = d;
		return true;
	}
	return false;
}

int Demand::getSize()
{
	return size;
}

void Demand::print()
{
	cout << "***************print Demand strart***************" << endl;
	cout << "size: " << size << endl;
	for(int i = 0;i < size;i ++)
	{
		for(int j = 0;j < size;j ++)
			cout << demand[i][j] << " ";
		cout << endl;
	}
	cout << "*************** print Demand end***************" << endl;
}