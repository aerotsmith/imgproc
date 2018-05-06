#include <iostream>
#include "CProc.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "usage: ctest <Image_Path> <Output_Path\n";
        return -1;
    }

    Mat image;
    image = imread(argv[1], 1);

    if (!image.data)
    {
        cout << "No image data (warning: OpenCV recognize files by extensions)\n";
        return -1;
    }

    CProc cProc;

    cProc.displayImage(image);
    return 0;
}
