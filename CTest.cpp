//-----------------------------------------
// Developer: Tom Smith
// Date: 5/7/2018
// 
// cli test program for ImgUtils class
//-----------------------------------------
#include <iostream>
#include "ImgUtils.h"
#include "opencv2/opencv.hpp"
#include <dirent.h>
#include <errno.h>
#include <iostream>

using namespace std;
using namespace cv;

int getdir (string dir)
{
    DIR *dp;
    struct dirent *dirp;
    if ((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0]!='.')
            cout << dirp->d_name << "  ";
    }
    cout << endl;

    closedir(dp);
    return 0;
}

void getParams(string str, int *x, int *y, float *tol)
{
    stringstream ss;    
 
    ss << str;
 
    string temp;

    ss >> temp;
    stringstream(temp) >> *x;
    ss >> temp;
    stringstream(temp) >> *x;
    ss >> temp;
    stringstream(temp) >> *y;
    ss >> temp;
    stringstream(temp) >> *tol;

    return;
}

int main(int argc, char **argv)
{
    uchar **region;
    uchar **perimeter;
    int x,y;
    float tolerance;
    vector<xy> rv;
    vector<vector<xy>> perimeters;
    string path="../Images/";
    string filename;

    ImgUtils imgUtils;
    Mat image;
    Mat smoothPerimeters;

    bool imgLoaded=false;
    bool regionCreated=false;
    bool perimeterCreated=false;
    bool smoothCreated=false;


    string cmd="";

    while (cmd!="quit")
    {
        cout << "\n\nCommands:\n";
        cout << "list - lists image files\n";
        cout << "load <image_name> - load an image\n";
        if (imgLoaded)
           cout << "region <x> <y> <tolerance> - fills region in the loaded image at x,y and tolerance (0.1 is good)\n";
        if (regionCreated)
        {
           cout << "save region    - save region as image and text file\n";
           cout << "perimeter      - finds the perimeter of the region\n";
        }
        if (perimeterCreated)
        {
           cout << "save perimeter - save perimeter as image and text file\n";
           cout << "smooth         - finds the smooth perimeter of the region\n";
        }
        cout << "type quit to exit\n\n";

        getline(cin,cmd);

        if (cmd=="list")
           getdir(path);
        else if ((cmd.compare(0,5,"load "))==0)
        {
            filename=cmd.substr (5,cmd.length()-5);
            cout << "Loading "<<filename<<"\n";
            cout << "Click image and hit key to continue...\n";
            image = imread(path.append(filename), 1);
            if (!image.data)
            {
                cout << "No image data (warning: OpenCV recognize files by extensions)\n";
                return -1;
            }

            imgUtils.displayImage(path.append(filename),image);

            imgLoaded=true;
        }
        else if ((imgLoaded) && ((cmd.compare(0,7,"region "))==0))
        {
            getParams(cmd, &x,&y, &tolerance);
            
            cout << "Flooding image at "<<x<<", "<<y<<endl;
            region = imgUtils.findRegion(&image,x,y,tolerance, &rv);
            cout << "Click image and hit key to continue...\n";
            imgUtils.displayPixels("Region", region, image.rows, image.cols);

            regionCreated=true;
        }
        else if ((regionCreated) && ((cmd.compare(0,9,"perimeter"))==0))
        {
            cout << "Finding perimeter\n";
            perimeter = imgUtils.findPerimeter(region, image.rows, image.cols, &perimeters);
            cout << "Click image and hit key to continue...\n";
            imgUtils.displayPixels("Perimeter", perimeter, image.rows, image.cols);

            perimeterCreated=true;
        }
        else if ((regionCreated) && ((cmd.compare(0,11,"save region"))==0))
        {
            cout << "Writing regionimg.png and region.txt\n";
            imgUtils.savePixels(region, image.rows, image.cols, "regionimg.png");

            ofstream file;
            file.open ("region.txt");

            vector<xy>::iterator it;
            for (it=rv.begin(); it!=rv.end(); it++)
            {
                file << it->x << ", "<<it->y<<endl;
            }
            file.close();
        }
        else if ((perimeterCreated) && ((cmd.compare(0,6,"smooth"))==0))
        {
            cout << "Finding smooth perimeter\n";
            smoothPerimeters = imgUtils.findSmoothPerimeter(&perimeters, image.rows, image.cols);
            cout << "Click image and hit key to continue...\n";
            imgUtils.displayImage("Smooth Perimeters", smoothPerimeters);

            smoothCreated = true;
        }
        else if ((perimeterCreated) && ((cmd.compare(0,14,"save perimeter"))==0))
        {
            imgUtils.savePixels(perimeter, image.rows, image.cols, "perimeterimg.png");
            cout << "Writing perimeterimg.png and perimeter.txt\n";

            ofstream file;
            file.open ("perimeter.txt");

            vector<vector<xy>>::iterator it;
            vector<xy>::iterator it2;
            for (it = perimeters.begin(); it != perimeters.end(); it++)
            {
                file << "New perimeter\n";
                for (it2=it->begin(); it2!=it->end(); it2++)
                {
                    file << it2->x << ", "<<it2->y<<endl;
                }
            }
            file.close();
        }
        
    }

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
