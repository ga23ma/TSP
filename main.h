#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <float.h>
#include "point.h"
#include "instance.h"
#include "solver.h"

using namespace std;

struct Params{
    string algorithm;
    string inputFile;
    bool showGraph;
    int timelimit;
};

void readParams(Params &params, int argc, char *argv[]);
void readInstance(string filename, Instance &instance);
void printSolution(Instance &instance, vector<int> &solution);
void createPDF(Instance &instance, vector<int> &solution);
#endif // MAIN_H
