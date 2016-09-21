#include "basic.h"
#include "Topology.h"
#include "Scheduler.h"
#include "Demand.h"
#include "Flow.h"
#include "basic.h"
#include "Dijkstra.h"

//create a random demand matrix
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

//make score of input demand and topo, para show decided if show finish time of all flows on cmd
int accurate_score(int demand[][SIZE], int topo[][nodenum], bool show = false)
{
	clock_t begin = clock();
	//init topology and demand
	Topology *t = new Topology(nodenum);
	Demand *d = new Demand(nodenum);
	for(int i = 0;i < SIZE;i ++)
		for(int j = 0;j < SIZE;j ++)
		{
			d->set(i,j,demand[i][j]);
		}
	for(int i = 0;i < nodenum;i ++)
		for(int j = 0;j <nodenum;j ++)
		{
			t->set(i,j,topo[i][j]);
		}
	Scheduler* s = new Scheduler();
	//create flows, f is a linked list
	Flow* f = NULL;
	f = Scheduler::addFlows(t,d,f);
	//if there are more than one shortest paths, the flow will be divided into several subflows.
	//start to schedule and return finish time
	int ts =  s->schedule(t,d,f,show);
	//cout << "finish time" << ts << endl;
	delete s;
	delete t;
	delete d;
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	//printf("One running time: %f sec\n", elapsed_secs);
	return ts;
}

void copy_topo(int topo[][nodenum],int newTopo[][nodenum])
{
	for (int i = 0; i < nodenum; i++)
		for (int j = 0; j < nodenum; j++)
			newTopo[i][j] = topo[i][j];
}

//ntopo stores new topo
void topo_change(int topo[][nodenum],int ntopo[][nodenum], int width = 1)
{
	srand((unsigned)time(NULL));
	copy_topo(topo,ntopo);
	int num = ToRnum ;
	for(int i = 0;i < width; i ++)
	{
		int start1 = rand() % num + ToRnum;
		int end1 = rand() % num + ToRnum;
		//cout << end1 << " " ;
		while (end1 == start1 || ntopo[start1][end1] != 1)
			end1 = (end1 + 1) % num + ToRnum;
		int start2 = rand() % num + ToRnum;
		while (start2 == start1 || start2 == end1 || ntopo[start2][end1] == 1)
			start2 = (start2 + 1) % num + ToRnum;
		int end2 = rand() % num + ToRnum;
		while (end2 == start1 || end2 == end1 || end2 == start2 || ntopo[start1][end2] == 1 || ntopo[start2][end2] != 1)
			end2 = (end2 + 1) % num + ToRnum;
		// Fan Quan: skip fixed link
		if (start1 - end1 == 1 || end1 - start1 == 1)
			continue;
		if (start2 - end2 == 1 || end2 - start2 == 1)
			continue;
		ntopo[start1][end1] = 0;
		ntopo[end1][start1] = 0;
		ntopo[start2][end2] = 0;
		ntopo[end2][start2] = 0;
		ntopo[start1][end2] = 1;
		ntopo[start2][end1] = 1;
		ntopo[end1][start2] = 1;
		ntopo[end2][start1] = 1;
	}
}

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


void getnewtopo(int topo[][nodenum])
{
	for (int i = 0; i < 100; i++)
		topo_change(topo, topo);//what it means?
}

//beam search to find a best topology
//look for help from SHENGHUI
void getBestTopo(int demand[][SIZE],int topo[][nodenum], int beamTopo[BEAMWIDTH][nodenum][nodenum], int beamScore[BEAMWIDTH],int bestTopo[][nodenum],int dist[][nodenum], int ntopo[][nodenum])
{
	int count = 0;
	int curBest = INT_MAX;
	int curScore = accurate_score(demand, topo);
	for (int i = 0; i < BEAMWIDTH; i++)
		copy_topo(topo, beamTopo[i]);
	while (count < SEARCHDEPTH)
	{
		clock_t begin = clock();
		bool flag = false;
		for (int i = 0; i < CHANGETIMES; i++)
		{
			//cout << i << endl;
			int width = rand() % 2 + 1;
			topo_change(topo, ntopo);

			int s = accurate_score(demand, ntopo);
			//if(count == 0) cout << s << endl;
			if (s < curScore)
			{
				curScore = s;
				//k = -1;
				flag = true;
				copy_topo(ntopo, topo);
				updateBeam(ntopo, s, beamTopo, beamScore);
			}
		}
		//cout << curScore << endl;
		if (!flag) {
			if (!(rand() % 1))
			{
				getnewtopo(topo);
			}
			else
			{
				int tmpScore = INT_MAX;
				int tmpIndex = -1;
				for (int k = 0; k < BEAMWIDTH; k++) {
					//if(memcmp(topo, beamTopo[k], sizeof(topo)) == 0) continue;
					bool f = true;
					for (int i = 0; i < nodenum; i++)
					{
						for (int j = 0; j < nodenum; j++)
						{
							if (topo[i][j] != beamTopo[k][i][j])
							{
								f = false;
								break;
							}
						}
						if (!f) break;
					}
					if (f) continue;
					if (beamScore[k]<tmpScore) {
						tmpScore = beamScore[k];
						tmpIndex = k;
					}
				}
				if (tmpIndex != -1)
					copy_topo(beamTopo[tmpIndex], topo);
			}
			//cout << "************************" << endl;
		}

		//up?
		if (curScore < curBest)
		{
			curBest = curScore;
			copy_topo(topo, bestTopo);
		}
		//YANG: 8-29
		stringstream ss;
		ss << count;
		string s = ss.str();
		string fpath = "E:\\VS_Project\\deeplearning\\Sample_Data\\tmp2\\topo" + s + ".txt";//store temprory topology
		//ofstream outfile(fpath.c_str());
		/*for (int i = 0; i < nodenum; i++)
		{
			for (int j = 0; j < nodenum; j++)
			{
				outfile << topo[i][j] << " ";
			}
			outfile << endl;
		}*/
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		//accurate_score(demand, topo, true); //this topo stores the best topo of current depth
		//printf("One running time: %f sec\n", elapsed_secs);
		count++;
		cout << "curScore:" << curScore << endl;
		//outfile << curScore << endl;
		//outfile.close();
		/*
		for(int i = 0;i < SIZE;i ++)
		{
		for(int j = 0;j < SIZE;j ++)
		{
		cout << topo[i][j] << " ";
		}
		cout << endl;
		}
		*/
	}
	//cout << curBest << endl;
}

//initial OSC links between ToRs, each ToR has 4 wireless links 
void initialToRLink(int topo[][nodenum])
{
	int num = ToRnum;
	for (int i = 0; i < num / 2; i++)		// Fan Quan: 0-5, 1-6...beside the ring base, 1 connection
	{
		topo[i + ToRnum][(i + 5) % num + ToRnum] = 1;
		topo[(i + 5) % num + ToRnum][i + ToRnum] = 1;
		//topo[i+ToRnum][(i + num / 2 - 1) % num+ToRnum] = 1;
		//topo[(i + num / 2 - 1) % num+ToRnum][i+ToRnum] = 1;
	}
}

void initialFixedLink(int topo[][nodenum])
{
	//YANG£ºeach server connects to a toR switch
	for (int i = 0; i < ToRnum; i++)
	{
		topo[i][ToRnum + i] = 1;
		topo[ToRnum + i][i] = 1;

		// Fan Quan: ring
		topo[i + ToRnum][(i + 1) % ToRnum + ToRnum] = 1;
		topo[(i + 1) % ToRnum + ToRnum][i + ToRnum] = 1;
	}
}

void writeTopoArray2File(string path,int array[][nodenum],int size)
{
	cout << "write Topo to: " << path << endl;
	ofstream f(path.c_str());
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
	cout << "complete writing topo operation" << endl;
	
}

void writeDemandArray2File(string path, int array[][SIZE], int size)
{
	ofstream f(path.c_str());
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


//read demand matrix from a file, file path defined in function, I fell very sorry I do not use para to pass it. This implement is too stupid!
void readDemand(int demand[][ToRnum], int k)
{
	//string demandPath = "E:\\VS_Project\\deeplearning\\Sample_Data\\Demand\\demand";
	string demandPath = "..\\..\\Sample_Data\\Demand\\demand";
	stringstream ss;
	ss << k;
	string s = ss.str();
	demandPath += s + ".txt";
	cout << "read file from: " << demandPath << endl;
	int d;
	ifstream in(demandPath.c_str());
	
	for(int i = 0;i < ToRnum;i ++)
	{
		for(int j = 0;j < ToRnum;j ++)
			in >> demand[i][j];
	}
	in.close();
}
void readTopo(int topo[][nodenum], int k)
{
	string demandPath = "..\\..\\..\\..\\Edmond-testbed\\Edmond-testbed\\Sample_data\\edmond_4\\topo\\topo0.txt";
	/*stringstream ss;
	ss << k;
	string s = ss.str();
	demandPath += s + ".txt";*/
	ifstream in(demandPath.c_str());
	for (int i = 0; i < nodenum; i++)
	for (int j = 0; j < nodenum; j++)
		in >> topo[i][j];
	in.close();
}


int main()
{
	//srand((unsigned)time(NULL));
	//only test 1 sample

	//delete directory
	/*string dir = "..\\..\\Sample_Data\\link_utilization";
	string command ="del " + dir;
	system(command.c_str());*/

	for(int count = 0;count < 1 ;count ++)
	{
		clock_t start, finish;
		double duration;
		start = clock();
		//YANG: generate and write demand to file
		//getDemand(demandMatrix);//get demand
		//string demandPath = "..\\..\\Sample_Data\\Demand\\demand0.txt";
		//writeDemandArray2File(demandPath, demandMatrix, SIZE);


		//initial beam
		for (int i = 0; i < BEAMWIDTH; i++)
			beamScore[i] = INT_MAX;
		//set path
		//string demandPath = "E:\\VS Project\\deeplearning\\Sample_Data\\demand_16/new_demand";
		string topoPath = "..\\..\\Sample_Data\\topo\\topo";
		//string timePath = "E:\\VS Project\\deeplearning\\Sample_Data\\time_0\\new_time";
		stringstream ss;
		ss << count;
		string s = ss.str();
		//demandPath += s + ".txt";
		topoPath += s + ".txt";
		//timePath += s + ".txt";
		//read demand matrix from file, if you want to generate demand matrix, use getDemand in line 513, and writeDemandArray2File in line 530
		readDemand(demandMatrix, count);

		//writeDemandArray2File(demandPath, demandMatrix, ToRnum);

		//initial topo
		//2016.8.3: No.i data is related with No.i-1, so except for the No.0 is initialized with zero the others are initialized with the No.i-1 topo.
		if (count == 0)
		{
			// Fan Quan: read topo from edmond, and no need to initialize ToR link and fixed link
			//readTopo(topo, count);
			memset(topo, 0, sizeof(int) *nodenum*nodenum);
			for (int i = 0; i < nodenum; i++)
				for (int j = 0; j < nodenum; j++)
					topo[i][j] = 0;

			initialToRLink(topo);
			initialFixedLink(topo);
		}
		else
		{
			//copy bestTopo into topo to initialize topo
			memcpy(topo, bestTopo, sizeof(int) * nodenum * nodenum);
		}


		getBestTopo(demandMatrix, topo, beamTopo, beamScore, bestTopo, dist, ntopo);

		accurate_score(demandMatrix, bestTopo, true);

		finish = clock();
		duration = (double) (finish - start) / CLOCKS_PER_SEC;

		//cout << demandPath << endl;
		//cout << topoPath << endl;

		/*ofstream f(timePath.c_str());
		f << duration;
		f.close();*/
		//
		writeTopoArray2File(topoPath, bestTopo, nodenum);
		//printTopo(bestTopo);
		cout << endl;
	}
	system("pause");
	return 0;
}