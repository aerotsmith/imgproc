#include "CProc.h"

// Displays a Mat image
void CProc::displayImage(const Mat &cvMat)
{
    namedWindow("Input Image", WINDOW_AUTOSIZE);
    imshow("Input Image", cvMat);
    waitKey(0);
}

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
uchar **CProc::findRegion(Mat *cvMat, int x, int y, float tolerance,
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

// Returns true if any neighbor pixels are empty
bool hasEmptyNeighbor(uchar **region, int rows, int cols, int x, int y)
{
    return ( ((x>0)      && (y>0)      && (region[x-1][y-1]==0)) ||
             (              (y>0)      && (region[x  ][y-1]==0)) ||
             ((x<cols-1) && (y>0)      && (region[x+1][y-1]==0)) ||
             ((x>0)                    && (region[x-1][y  ]==0)) ||
             ((x<cols-1)               && (region[x+1][y  ]==0)) ||
             ((x>0)      && (y<rows-1) && (region[x-1][y+1]==0)) ||
             (              (y<rows-1) && (region[x  ][y+1]==0)) ||
             ((x<cols-1) && (y<rows-1) && (region[x+1][y+1]==0)));
}


// Creates a perimeter bitmap from a region bitmap
uchar ** CProc::findPerimeter(uchar **region, int rows, int cols,
                              vector<xy> *pv)
{
    // Create perimeter bitmap
    uchar **perimeter = new uchar*[cols];
    for (int i=0; i<cols; i++)
       perimeter[i]=(uchar *)calloc(rows,sizeof(uchar));

    for (int y=0; y<rows; y++)
       for (int x=0; x<cols; x++)
       {
          if (region[x][y]==0)
             perimeter[x][y]=0;
          else
          {
             if (!hasEmptyNeighbor(region,rows,cols, x,y))
                perimeter[x][y]=0;
             else
             {
                perimeter[x][y]=1;

                xy pos;
                pos.x=x;
                pos.y=y;
                pv->push_back(pos);
             }
          }
       }

    return perimeter;
}

// Displays a bitmap from findRegion or findPerimeter
void CProc::displayPixels(uchar **pixels, int rows, int cols)
{
    Mat image(rows, cols , CV_8UC3, Scalar(255,255,255));
    Mat_<Vec3b> _image = image;

    for (int x=0; x<cols; x++)
      for (int y=0; y<rows; y++)
      {
         if (pixels[x][y])
         {
            _image(y,x)[0]=0;
            _image(y,x)[1]=0;
            _image(y,x)[2]=0;
         }
      }

    displayImage(image);
    return;
}

// save bitmap to image file
void CProc::savePixels(uchar **pixels, int rows, int cols, string filename)
{
    Mat image(rows, cols , CV_8UC3, Scalar(255,255,255));
    Mat_<Vec3b> _image = image;

    for (int x=0; x<cols; x++)
      for (int y=0; y<rows; y++)
      {
         if (pixels[x][y])
         {
            _image(y,x)[0]=0;
            _image(y,x)[1]=0;
            _image(y,x)[2]=0;
         }
      }

    imwrite(filename, image);
    return;
}
