# imgproc
exercise-AEO3110

TO BUILD:
From the project directory, execute these commands:
mkdir build
cd build
cmake ..
make

To run:
ctest test1.png 135 60 0.01


Test cases:
A good example of all features can be seen by loading test4a.jpg
In the cli, run region with: region 275 100 0.1

perimeter and smooth can be run to create the perimeter and smoothed perimeter.


REFERENCES:
8-Way Recursive Method:
https://www.codeproject.com/Articles/6017/QuickFill-An-efficient-flood-fill-algorithm

RGB 2 HSV conversion:
https://www.rapidtables.com/convert/color/rgb-to-hsv.html

Distance between colors calculation:
https://stackoverflow.com/questions/35113979/calculate-distance-between-colors-in-hsv-space

Polyfit Least Squares Method:
http://www.bragitoff.com/2015/09/c-program-for-polynomial-fit-least-squares/

Improvements:
This does not properly handle all test cases. Given more time I would be able to work out
any issues for all test cases. I worked to have as much functionality in all the 
requirements as possible in the given time.
