#ifndef IMAGE_H_
#define IMAGE_H_
 
//Adding header files
#include <iostream>
#include <math.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

struct xy
{ 
  int x;
  int y;
};

struct hsv
{ 
  float h;
  float s;
  float v;
};
 
class CProc
{
 
public:
    void displayImage(string name, const Mat &cvMat);
    void saveImage(Mat cvMat);
    void displayPixels(string name, uchar **pixels, int rows, int cols);
    void savePixels(uchar **pixels, int rows, int cols, string filename);
    uchar **findRegion(Mat *cvMat, int x, int y, float tolerance, vector<xy> *rv);
    uchar **findPerimeter(uchar **region, int rows, int cols, vector<vector<xy>> *perimeters);
    Mat findSmoothPerimeter(vector<vector<xy>> *perimeters, int rows, int cols); 
};

#endif
