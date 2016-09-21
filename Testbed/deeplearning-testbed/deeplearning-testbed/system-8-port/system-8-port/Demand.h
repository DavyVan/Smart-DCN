#ifndef DEMAND
#define DEMAND

class Demand
{
public:
	Demand(int);
	Demand(int, int**);
	Demand(char*);
	~Demand();
	int getSize();
	double get(int, int);
	bool set(int, int, double);
	void print();
private:
	double** demand;
	int size;
};

#endif