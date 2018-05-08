//-------------------------------------------------------------------------
// Developer: Tom Smith
// Date: 5/7/2018
// 
// findSmoothPerimeter() method
// Used the Polyfit Least Squares method to smooth perimeter curves
// Returns Mat image file of smoothed curves
//-------------------------------------------------------------------------
#include "ImgUtils.h"
#include "polyfit.h"

Mat ImgUtils::findSmoothPerimeter(vector<vector<xy>> *perimeters, int rows, int cols)
{
    Mat image(rows, cols , CV_8UC3, Scalar(255,255,255));

    xy pos1,pos2;
    xy lpos,pos;
    bool lineFinished=true;
    bool verticalLine=false;

    vector<vector<xy>>::iterator it;
    vector<xy>::iterator it2;
    for (it = perimeters->begin(); it != perimeters->end(); it++)
    {
        for (it2=it->begin(); it2!=it->end(); it2++)
        {
            double *a;
            int degree=4;
            a = (double *)calloc(degree,sizeof(double));

            if (it2+20 < it->end())
            {
                int x1=it2->x;
                polyFit(degree,20, &it2, a);
                it2+=20;
                int x2=it2->x;

                if (x1<x2)
                    for (double x=x1; x<x2; x++)
                    {
                        Mat_<Vec3b> _image = image;
                        double y=0.0;

                        for (int d=0; d<=degree; d++)
                            y+=a[d]*pow(x,d);

                        _image(y,(int)x)[0]=0;
                        _image(y,(int)x)[1]=0;
                        _image(y,(int)x)[2]=0;
                    }
                else
                    for (double x=x2; x<x1; x++)
                    {
                        Mat_<Vec3b> _image = image;
                        double y=0.0;

                        for (int d=0; d<=degree; d++)
                            y+=a[d]*pow(x,d);

                        _image(y,(int)x)[0]=0;
                        _image(y,(int)x)[1]=0;
                        _image(y,(int)x)[2]=0;
                    }
            }
        }
    }

    return image;
} 
