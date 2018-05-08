//-------------------------------------------------------------------------
// Developer: Tom Smith
// Date: 5/7/2018
// 
// Polynomial Fit
// Finds a smoothed curve of a list of points (pts) and returns polynomial
// coefficients for the curve of a given degree.
//-------------------------------------------------------------------------
#include "polyfit.h"

using namespace std;

void polyFit(int degree,int numPairs, vector<xy>::iterator *pts, double *a)
{
    int i,j,k;

    //  X will contain the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    double X[2*degree+1];

    for (i=0;i<2*degree+1;i++)
    {
        X[i]=0;
        for (j=0;j<numPairs;j++)
            X[i]=X[i]+pow((*pts)[j].x,i);
    }

    //B is the Normal matrix(augmented) that will store the equations
    double B[degree+1][degree+2];
    for (i=0;i<=degree;i++)
        for (j=0;j<=degree;j++)
            B[i][j]=X[i+j];

    // Y will contain the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    double Y[degree+1];
    for (i=0;i<degree+1;i++)
    {    
        Y[i]=0;
        for (j=0;j<numPairs;j++)
        Y[i]=Y[i]+pow((*pts)[j].x,i)*(*pts)[j].y;
    }

    for (i=0;i<=degree;i++)
        B[i][degree+1]=Y[i];

    degree=degree+1;

    //Gaussian Elimination starts to solve the set of linear equations (Pivotisation)
    for (i=0;i<degree;i++)
        for (k=i+1;k<degree;k++)
            if (B[i][i]<B[k][i])
                for (j=0;j<=degree;j++)
                {
                    double temp=B[i][j];
                    B[i][j]=B[k][j];
                    B[k][j]=temp;
                }
    
    //loop to perform the gauss elimination
    for (i=0;i<degree-1;i++)
        for (k=i+1;k<degree;k++)
            {
                double t=B[k][i]/B[i][i];
                for (j=0;j<=degree;j++)
                    B[k][j]=B[k][j]-t*B[i][j];
            }

    //back-substitution
    for (i=degree-1;i>=0;i--)
    {
        a[i]=B[i][degree];
        for (j=0;j<degree;j++)
            if (j!=i)
                a[i]=a[i]-B[i][j]*a[j];
        a[i]=a[i]/B[i][i];
    }
}
