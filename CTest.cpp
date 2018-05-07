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

    cProc.displayImage(image);

    // find the region at x,y and transform region bitmap into a perimeter bitmap
    region = cProc.findRegion(&image,x,y,tolerance);
    perimeter = cProc.findPerimeter(region, image.rows, image.cols);

    // Display region and perimeter bitmaps
    cProc.displayPixels(region, image.rows, image.cols);
    cProc.displayPixels(perimeter, image.rows, image.cols);

    cProc.savePixels(region, image.rows, image.cols, "regionimg.png");
    cProc.savePixels(perimeter, image.rows, image.cols, "perimeterimg.png");

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
