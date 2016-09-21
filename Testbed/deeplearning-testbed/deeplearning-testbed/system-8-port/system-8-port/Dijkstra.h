#ifndef DIJKSTRA
#define DIJKSTRA
#include <vector>
#include "basic.h"
using namespace std;

void Dijkstra(double** matrix, double* res, int node, const int nodeNum);
void Dijkstra(double** matrix, double** res, const int nodeNum);
void Dijkstra(double** matrix, double* res, int node, const int nodeNum, vector<int> *prev);
int* findway(double**, double**, int, int, int);
void findAllRoute(int, const int, vector<int> *, vector<vector<int> > *);
#endif