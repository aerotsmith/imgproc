//Polynomial Fit
#ifndef _POLYFIT_
#define _POLYFIT_
#include <cmath>
#include "ImgUtils.h"

using namespace std;

void polyFit(int degree,int numPairs, vector<xy>::iterator *pts, double *a);
#endif
