#ifndef PARAMS_H
#define PARAMS_H
#include <vector>
#include <string>
#include "point.h"

using namespace std;

struct Instance{
    vector<Point> points;
    string instanceName;
    int n;
};

#endif // PARAMS_H
