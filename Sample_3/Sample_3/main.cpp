#include<iostream>
#include <fstream>
#include<string>
#include <sstream>
#include <stdlib.h>
#include<time.h>

using namespace std;

#define BEAMWIDTH 5
#define K 8
#define ToRnum K * K / 2
#define Aggnum K * K / 2
#define corenum K * K / 4
#define nodenum ToRnum + Aggnum + corenum
#define SEARCHDEPTH 200
#define CHANGETIMES 100
#define SIZE ToRnum

void getDemand(int demand[][ToRnum])
{
	srand((unsigned)time(NULL));
	int size = SIZE;
	int i, j;
	for (i = 0; i < size;i++)
	for (j = 0; j < size; j++)
		demand[i][j] = rand() % 250;
	for (i = 0; i < size; i++)
		demand[i][i] = 0;
}


void dijkstra(int topo[][nodenum], int num,int res[nodenum])
{
	int node_num = SIZE;
	bool visited[SIZE];
	for (int i = 0; i < SIZE; i++)
		visited[i] = false;
	for (int i = 0; i < SIZE; i++)
		res[i] = topo[num][i];
	visited[num] = true;
	res[num] = 0;
	node_num--;
	while (node_num != 0)
	{
		int mini_dist = -1;
		int mini_node = -1;
		for (int i = 0; i < SIZE; i++)
		{
			if (!visited[i] && res[i] > 0 && (mini_dist < 0 || mini_dist > res[i]))
			{
				mini_dist = res[i];
				mini_node = i;
			}
		}
		if (mini_node < 0)
			break;
		visited[mini_node] = true;
		node_num--;
		for (int i = 0; i < SIZE; i++)
		if (!visited[i] && topo[mini_node][i] > 0 && (res[i] < 0 || res[i] > mini_dist + topo[mini_node][i]))
			res[i] = mini_dist + topo[mini_node][i];
	}
}

void getDist(int topo[][nodenum],int dist[][nodenum])
{
	for (int i = 0; i < SIZE; i++)
		dijkstra(topo, i,dist[i]);
}

/*
bool allLinked(int** topo)
{
	int** dist = getDist(topo);
	for (int i = 0; i < SIZE; i++)
	for (int j = 0; j < SIZE; j++)
	if (dist[i][j] < 0)
		return false;
	return true;
}*/

void printTopo(int topo[][nodenum])
{
	for (int i = 0; i < nodenum; i++)
	{
		for (int j = 0; j < nodenum; j++)
		{
			cout << topo[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

int score(int demand[][SIZE], int topo[][nodenum],int dist[][nodenum])
{
	int score = 0;
	int num = ToRnum;
	getDist(topo,dist);
	for (int i = 0; i < num; i++)
	for (int j = 0; j < num; j++)
		score += demand[i][j] * dist[i][j];
	return score;
}


void copy_topo(int topo[][nodenum],int newTopo[][nodenum])
{
	for (int i = 0; i < nodenum; i++)
	for (int j = 0; j < nodenum; j++)
		newTopo[i][j] = topo[i][j];
}

void topo_change(int topo[][nodenum],int ntopo[][nodenum])
{
	copy_topo(topo,ntopo);
	int num = ToRnum;
	int start1 = rand() % num;
	int end1 = rand() % num;
	while (end1 == start1 || topo[start1][end1] != 1)
		end1 = (end1 + 1) % num;
	int start2 = rand() % num;
	while (start2 == start1 || start2 == end1 || topo[start2][end1] == 1)
		start2 = (start2 + 1) % num;
	int end2 = rand() % num;
	while (end2 == start1 || end2 == end1 || end2 == start2 || topo[start1][end2] == 1 || topo[start2][end2] != 1)
		end2 = (end2 + 1) % num;
	ntopo[start1][end1] = 0;
	ntopo[end1][start1] = 0;
	ntopo[start2][end2] = 0;
	ntopo[end2][start2] = 0;
	ntopo[start1][end2] = 1;
	ntopo[start2][end1] = 1;
	ntopo[end1][start2] = 1;
	ntopo[end2][start1] = 1;
}
/**
int** getBestTopo(int** demand)
{
int count = 0;
int** curBestTopo = new int*[SIZE];
for (int i = 0; i < SIZE; i++)
curBestTopo[i] = new int[SIZE];
int curBest = INT_MAX;
while (count < 10)
{
stringstream ss;
ss << count;
string s = ss.str();
int** topo = getTopo(s);
int curScore = score(demand, topo);
int** curTopo = copy_topo(topo);
int flag = 0;
while (true)
{
flag = 0;
topo = copy_topo(curTopo);
for (int i = 0; i < SIZE; i++)
for (int j = 0; j < SIZE; j++)
{
if (topo_change(topo, i, j))
{
int s = score(demand, topo);
if (s < curScore)
{
curScore = s;
flag = 1;
curTopo = copy_topo(topo);
topo_change(topo, i, j);
}
}
}
if (flag == 0)
break;
}
if (curScore < curBest)
{
curBest = curScore;
curBestTopo = copy_topo(curTopo);
}
count++;
cout << curScore << endl;
}
cout << curBest << endl;
return curBestTopo;
}

**/

void updateBeam(int topo[][nodenum], int score, int beamTopo[BEAMWIDTH][nodenum][nodenum], int beamScore[BEAMWIDTH])
{
	for (int i = 0; i < BEAMWIDTH; i++)
	if (beamScore[i] > score)
	{
		beamScore[i] = score;
		copy_topo(topo, beamTopo[i]);
		break;
	}
}

void getBestTopo(int demand[][SIZE],int topo[][nodenum], int beamTopo[BEAMWIDTH][nodenum][nodenum], int beamScore[BEAMWIDTH],int bestTopo[][nodenum],int dist[][nodenum], int ntopo[][nodenum])
{
	int count = 0;
	int curBest = INT_MAX;
	while (count < SEARCHDEPTH)
	{
		int curScore = score(demand,topo,dist);
		for (int i = 0; i < BEAMWIDTH; i++)
			copy_topo(topo,beamTopo[i]);
		
		bool flag=false;
		for (int i = 0; i < CHANGETIMES; i++)
		{
			topo_change(topo,ntopo);
			
			int s = score(demand, ntopo,dist);
			if (s < curScore)
			{
				curScore = s;
				//k = -1;
				flag=true;
			
				copy_topo(ntopo,topo);
				updateBeam(ntopo, s, beamTopo, beamScore);
			}
		}
		if(!flag){
			int tmpScore=INT_MAX;
			int tmpIndex=-1;
			for(int k=0; k < BEAMWIDTH; k++){
				if(memcmp(topo, beamTopo[k], sizeof(topo)) == 0) continue;
				if(beamScore[k]<tmpScore){
					tmpScore = beamScore[k];
					tmpIndex = k;
				}
			}
			if(tmpIndex!=-1) 
				copy_topo(beamTopo[tmpIndex],topo);
			cout << "************************" << endl;
		}
		
		
		if (curScore < curBest)
		{
			curBest = curScore;
			copy_topo(topo, bestTopo);
		}
		count++;
		cout << curScore << endl;
	}
	cout << curBest << endl;
}

//initial OSC links between ToRs
void initialToRLink(int topo[][nodenum])
{
	int num = ToRnum;
	for (int i = 0; i < num; i++)
	{
		topo[i][(i + 2) % num] = 1;
		topo[(i + 2) % num][i] = 1;
		topo[i][(i + num / 2 - 1) % num] = 1;
		topo[(i + num / 2 - 1) % num][i] = 1;
	}
}

//initial fixed links of fat tree
void initialFixedLink(int topo[][nodenum])
{
	int num = Aggnum;
	for (int i = num; i < num * 2; i++)
	{
		if (i % 2 == 0)
		{
			topo[i][i - num] = 1;
			topo[i][i - num + 1] = 1;
			topo[i - num][i] = 1;
			topo[i - num + 1][i] = 1;
		}
		else
		{
			topo[i][i - num] = 1;
			topo[i][i - num - 1] = 1;
			topo[i - num][i] = 1;
			topo[i - num - 1][i] = 1;
		}
	}

	int core = corenum;
	for (int i = num * 2; i < num * 2 + core / 2; i++)
	{
		for (int j = num; j < num * 2; j = j + 2)
		{
			topo[i][j] = 1;
			topo[j][i] = 1;
		}
	}
	for (int i = num * 2 + core / 2; i < num * 2 + core; i++)
	{
		for (int j = num + 1; j < num * 2; j = j + 2)
		{
			topo[i][j] = 1;
			topo[j][i] = 1;
		}
	}
}

void writeTopoArray2File(string path,int array[][nodenum],int size)
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
	cout << path << endl;
}

int demandMatrix[ToRnum][ToRnum];
int beamTopo[BEAMWIDTH][nodenum][nodenum];
int beamScore[BEAMWIDTH];
int topo[nodenum][nodenum];
int bestTopo[nodenum][nodenum];
int dist[nodenum][nodenum];
int ntopo[nodenum][nodenum];

int main()
{
	int count = 0;

	while (count < 10000)
	{
	clock_t start, finish;
	double duration;
	start = clock();
	getDemand(demandMatrix);//get demand

	//initial beam
	
	for (int i = 0; i < BEAMWIDTH; i++)
		beamScore[i] = INT_MAX;

	//initial topo
	memset(topo, 0, sizeof(int)*nodenum*nodenum);
	for (int i = 0; i < nodenum;i++)
	for (int j = 0; j < nodenum; j++)
		topo[i][j] = 0;
	initialToRLink(topo);
	initialFixedLink(topo);
	

	getBestTopo(demandMatrix,topo, beamTopo, beamScore,bestTopo,dist,ntopo);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	string demandPath = "F:\\Learning\\demand8\\demand";
	string topoPath = "F:\\Learning\\topo8\\topo";
	string timePath = "F:\\Learning\\time8\\time";
	stringstream ss;
	ss << count;
	string s = ss.str();
	demandPath += s + ".txt";
	topoPath += s + ".txt";
	timePath += s + ".txt";
	//cout << demandPath << endl;
	//cout << topoPath << endl;
	
	ofstream f(timePath);
	f << duration;
	f.close();
	writeDemandArray2File(demandPath, demandMatrix, ToRnum);
	writeTopoArray2File(topoPath, bestTopo, nodenum);
	//printTopo(bestTopo);
	cout << endl;
	count++;
	}
	return 0;
}