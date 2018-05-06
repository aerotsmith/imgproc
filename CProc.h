#ifndef IMAGE_H_
#define IMAGE_H_
 
//Adding header files
#include <iostream>
#include <math.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
 
class CProc
{
 
public:
    void displayImage(const Mat &cvMat);
    void saveImage(Mat cvMat);
    void displayPixels(Mat *cvMat);
 
    //Member variables
protected:
 
};

#endif
