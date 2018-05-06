#include "CProc.h"

void CProc::displayImage(const Mat &cvMat)
{
    namedWindow("Input Image", WINDOW_AUTOSIZE);
    imshow("Input Image", cvMat);
    waitKey(0);
}
