#include <iostream>
#include "CProc.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    uchar **region;
    uchar **perimeter;
    int x,y;
    float tolerance;
    vector<xy> rv;
    vector<vector<xy>> perimeters;

    if (argc != 5)
    {
        cerr << "usage: ctest <Image_Path> <x-coord> <y-coord> <tolerance>\n";
        return -1;
    }

    // Collect parameters
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    tolerance = (float)atof(argv[4]);

    Mat image;
    image = imread(argv[1], 1);

    if (!image.data)
    {
        cout << "No image data (warning: OpenCV recognize files by extensions)\n";
        return -1;
    }

    CProc cProc;

    cProc.displayImage(argv[1],image);

    // find the region at x,y and transform region bitmap into a perimeter bitmap
    region = cProc.findRegion(&image,x,y,tolerance, &rv);
    perimeter = cProc.findPerimeter(region, image.rows, image.cols, &perimeters);

/*
    vector<vector<xy>>::iterator it;
    vector<xy>::iterator it2;
    for (it = perimeters.begin(); it != perimeters.end(); it++)
    {
        cout << "New perimeter\n";
        for (it2=it->begin(); it2!=it->end(); it2++)
        {
            cout << it2->x << ", "<<it2->y<<endl;
        }
    }
*/

    // Display region and perimeter bitmaps
    cProc.displayPixels("Region", region, image.rows, image.cols);
    cProc.displayPixels("Perimeter", perimeter, image.rows, image.cols);

    cProc.savePixels(region, image.rows, image.cols, "regionimg.png");
    cProc.savePixels(perimeter, image.rows, image.cols, "perimeterimg.png");

    Mat smoothPerimeters;
    smoothPerimeters = cProc.findSmoothPerimeter(&perimeters, image.rows, image.cols);

    cProc.displayImage("Smooth Perimeters", smoothPerimeters);

    // Clean up region
    for (int i=0; i<image.rows; i++)
      free(region[i]);
    delete [] region;

    // Clean up perimeter
    for (int i=0; i<image.rows; i++)
      free(perimeter[i]);
    delete [] perimeter;

    return 0;
}
