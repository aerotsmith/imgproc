#include "CProc.h"

void CProc::displayImage(const Mat &cvMat)
{
    namedWindow("Input Image", WINDOW_AUTOSIZE);
    imshow("Input Image", cvMat);
    waitKey(0);
}

void SeedFill(Mat &cvMat, vector<xy>* v, int x, int y, Vec3b color, Vec3b fill_color)
{
    // If out of bounds, return
    if ((x<0) || (x>cvMat.rows-1) ||
        (y<0) || (y>cvMat.cols-1))
      return;

    Mat_<Vec3b> _I = cvMat;

    // If match color, return
    if ((color[0]>_I(x,y)[0]-10) && (color[0]<_I(x,y)[0]+10) &&
        (color[0]>_I(x,y)[1]-10) && (color[0]<_I(x,y)[1]+10) &&
        (color[0]>_I(x,y)[2]-10) && (color[0]<_I(x,y)[2]+10))
      return;

    xy pos;

    pos.x = x;
    pos.y = y;

    _I(x,y)[0] = fill_color[0];
    _I(x,y)[1] = fill_color[1];
    _I(x,y)[2] = fill_color[2];

    v->push_back(pos);

    SeedFill(cvMat, v, x,y, color, fill_color);
    SeedFill(cvMat, v, x-1,y, color, fill_color);
    SeedFill(cvMat, v, x+1,y, color, fill_color);
    SeedFill(cvMat, v, x,y-1, color, fill_color);
    SeedFill(cvMat, v, x,y+1, color, fill_color);
    SeedFill(cvMat, v, x-1,y+1, color, fill_color);
    SeedFill(cvMat, v, x+1,y-1, color, fill_color);
    SeedFill(cvMat, v, x+1,y+1, color, fill_color);
    SeedFill(cvMat, v, x-1,y-1, color, fill_color);

    return;
}

vector<xy>* CProc::findRegion(Mat &cvMat, int x, int y)
{
    int nRows = cvMat.rows;
    int nCols = cvMat.cols;
    vector<xy> v;
    Vec3b color, fill_color;
    Mat_<Vec3b> _I = cvMat;

    // Set color at x,y
    color[0] = _I(x,y)[0];
    color[1] = _I(x,y)[1];
    color[2] = _I(x,y)[2];

    // Default fill color to black
    fill_color[0]=0;
    fill_color[1]=0;
    fill_color[2]=0;


    SeedFill(cvMat, &v, x,y, color, fill_color);

    return &v;
}
