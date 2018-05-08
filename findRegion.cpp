//-------------------------------------------------------------------------
// Developer: Tom Smith
// Date: 5/7/2018
// 
// findRegion() method
// Finds the region of a flood fill in a given image at x,y. Tolerance is
// calculated using color distance between hsv values.
// Returns bitmap of fill region
//-------------------------------------------------------------------------
#include "ImgUtils.h"

// returns the maximum r g or b value
float max(float r, float g, float b)
{
    if ((r>g) && (r>b))
        return r;
    if ((g>r) && (g>b))
        return g;
    return b;
}

// returns the minimum r g or b value
float min(float r, float g, float b)
{
    if ((r<g) && (r<b))
        return r;
    if ((g<r) && (g<b))
        return g;
    return b;
}

// Converts normalized rgb values to hsv
void rgb2hsv(float r, float g, float b, float *h, float *s, float *v)
{
    float Cmax = max(r,g,b); 
    float Cmin = min(r,g,b); 

    float delta = Cmax-Cmin;

    *v = Cmax;

    if (delta < 0.00001)
    {
        *s = 0.0;
        *h = 0.0;
        return;
    }

    if (Cmax == 0.0)
        *s = 0.0f;
    else
        *s = delta/Cmax;

    if (Cmax==r)
        *h = (g-b)/delta;
    else if (Cmax==g)
        *h = 2.0+(b-r)/delta;
    else 
        *h = 4.0+(r-g)/delta;

    return; 
}

// Converts normalized rgb values to hsv
void rgb2hsv(float r, float g, float b, hsv *color)
{
    rgb2hsv(r,g,b,&(color->h),&(color->s),&(color->v));
}

// Recursively traverses image to flood fill region
void SeedFill(Mat *cvMat, uchar **region, int x, int y,
              hsv seedColor, float tolerance, vector<xy> *rv)
{
    // If out of bounds, return
    if ((x<0) || (x>cvMat->cols-1) ||
        (y<0) || (y>cvMat->rows-1))
      return;

    Mat_<Vec3b> _cvMat = *cvMat;

    // If already filled, return
    if (region[x][y]==1)
      return;

    Vec3b current;
    float r,g,b;
    float h,s,v;

    // Get current color and convert to hsv
    r=_cvMat(y,x)[2]/255.0;
    g=_cvMat(y,x)[1]/255.0;
    b=_cvMat(y,x)[0]/255.0;

    rgb2hsv(r,g,b,&h,&s,&v);

    // Find color distance between seed color and current color
    float dh = min(abs(seedColor.h-h), 360-abs(seedColor.h-h)) / 180.0;
    float ds = abs(seedColor.s-s);
    float dv = abs(seedColor.v-v) / 255.0;
    float distance = sqrt(dh*dh+ds*ds+dv*dv);

    // If doesn't match seedColor, return
    if (distance > tolerance)
      return;

    region[x][y] = 1;
    xy pos;
    pos.x=x;
    pos.y=y;
    rv->push_back(pos);

    // Recursively scan neighboring pixels
    SeedFill(cvMat, region, x-1,y, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x+1,y, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x,y-1, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x,y+1, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x-1,y+1, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x+1,y-1, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x+1,y+1, seedColor, tolerance,rv);
    SeedFill(cvMat, region, x-1,y-1, seedColor, tolerance,rv);

    return;
}

bool compareByXY(const xy &a, const xy &b)
{
    return (a.y < b.y) || ((a.y == b.y) && (a.x < b.x));
}

// Creates a region bitmap from a Mat cvMat image. The region is a flood fill region
// flooding at x,y
uchar **ImgUtils::findRegion(Mat *cvMat, int x, int y, float tolerance,
                          vector<xy> *rv)
{
    int nRows = cvMat->rows;
    int nCols = cvMat->cols;
    hsv seedColor;
    Mat_<Vec3b> _cvMat = *cvMat;

    // Create region bitmap
    uchar **region = new uchar*[cvMat->cols];
    for (int i=0; i<cvMat->cols; i++)
       region[i]=(uchar *)calloc(cvMat->rows,sizeof(uchar));

    float r,g,b;
    r=_cvMat(y,x)[2]/255.0;
    g=_cvMat(y,x)[1]/255.0;
    b=_cvMat(y,x)[0]/255.0;

    // Get color at x,y
    rgb2hsv(r,g,b,&seedColor);

    // Recursively traverse bitmap and fill
    SeedFill(cvMat, region, x,y, seedColor, tolerance, rv);

    sort(rv->begin(), rv->end(), compareByXY);
    return region;
}
