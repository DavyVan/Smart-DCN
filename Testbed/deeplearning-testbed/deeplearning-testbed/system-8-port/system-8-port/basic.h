#ifndef BASIC
#define BASIC

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <memory>
#include <cstring>

#define BEAMWIDTH 25
#define K 5 //port number
#define ToRnum 10
#define Aggnum 10
#define corenum 0
#define nodenum ToRnum + Aggnum + corenum
#define SEARCHDEPTH 30
#define CHANGETIMES 50 //number of neighbour
#define SIZE ToRnum

#define NEWLIST 0
#define APPENDLIST 1
#define START_RATE 1
#define MAX_RATE 80

using namespace std;
#endif