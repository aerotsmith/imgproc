#include "ImgUtils.h"

// Displays a Mat image
void ImgUtils::displayImage(string name, const Mat &cvMat)
{
    namedWindow(name.c_str(), WINDOW_AUTOSIZE);
    imshow(name.c_str(), cvMat);
    waitKey(0);
}

// Displays a bitmap from findRegion or findPerimeter
void ImgUtils::displayPixels(string name, uchar **pixels, int rows, int cols)
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

    displayImage(name, image);
    return;
}

// save bitmap to image file
void ImgUtils::savePixels(uchar **pixels, int rows, int cols, string filename)
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
