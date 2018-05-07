#include "CProc.h"

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
                              vector<vector<xy>> *perimeters)
{
    // Create perimeter bitmap
    uchar **perimeter = new uchar*[cols];
    for (int i=0; i<cols; i++)
       perimeter[i]=(uchar *)calloc(rows,sizeof(uchar));

    vector<xy> pv;
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
                pv.push_back(pos);
             }
          }
       }

    // sort perimeter coords

    // Loop for each perimeter until pv is empty
    while (pv.size() > 0)
    {
       // Begin new perimeter

       // Get first point
       vector<xy> perim;
       xy pos;
       int vsize=perim.size();
       pos.x=pv[0].x;
       pos.y=pv[0].y;
       perim.push_back(pos);
       pv.erase(pv.begin()); 

       bool found;
       bool keepgoing=true;
       while(keepgoing)
       {
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x  ==pv[i].x) &&
                  (perim.back().y-1==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x+1==pv[i].x) &&
                  (perim.back().y-1==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x+1==pv[i].x) &&
                  (perim.back().y  ==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x+1==pv[i].x) &&
                  (perim.back().y+1==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x  ==pv[i].x) &&
                  (perim.back().y+1==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x-1==pv[i].x) &&
                  (perim.back().y+1==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x-1==pv[i].x) &&
                  (perim.back().y  ==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          found=false;
          for (int i=0; i<pv.size(); i++)
          {
              // If next position found, add to perim
              if ((perim.back().x-1==pv[i].x) &&
                  (perim.back().y-1==pv[i].y))
              {
                  pos.x=pv[i].x;
                  pos.y=pv[i].y;
                  perim.push_back(pos);
                  pv.erase(pv.begin()+i); 
                  found=true;
                  break;
              }
          }
   
          if (found)
              continue;
   
          keepgoing=false;
       }
 
       perimeters->push_back(perim);
    }

    return perimeter;
}
