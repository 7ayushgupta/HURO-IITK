//coding ShapeDetection.cpp from scratch
//importing various libraries
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <fstream>
#include <iostream>
#include "math.h"
#include "string.h"

//using all the namespaces to avoid writing cv::etc
using namespace cv;
using namespace std;

//writing main function
int main(int argc, char** argv)
{
  VideoCapture cap(0);
  while(1){
  Mat src;
  Mat dst;
  Mat threshold;
  int vertices;
  String str;
  double epsilon, x, y, ratio;
  int side1, side2;
  Moments moment;
  cap >> src;
  cvtColor(src,dst, COLOR_BGR2GRAY);
  blur(dst,dst, Size(3, 3), Point(-1,-1));
  inRange(dst,20,255,threshold);
  vector<vector<Point> > contours;
  vector<vector<Point> > approximation;
  vector<Vec4i> hierarchy;
  findContours(threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
  findContours(threshold, approximation, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
  if(contours.size()>0)
  {
   	Scalar color = Scalar(0,255,0);
	  for(int i = 0; i < contours.size(); i++)
	  {
      double epsilon = 0.006*arcLength(contours[i], true);
      approxPolyDP(contours[i], approximation[i], epsilon, true);
      Rect boundRect = boundingRect(approximation[i]);
      drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
      moment = moments(contours[i]);
      x = moment.m10 / moment.m00; //center of polygon
      y = moment.m01 / moment.m00;
      vertices = approximation[i].size();
      if(vertices == 3) str = "Triangle";
      if(vertices == 4)
      {     //check for square
        ratio = (boundRect.tl().x-boundRect.br().x)/(boundRect.tl().y-boundRect.br().y);
        if(ratio > 0.95 && ratio < 1.05)
            str = "Square";
        else
            str = "Rectangle";
        }
      if(vertices == 5) str = "Pentagon";
      if(vertices == 6) str = "Hexagon";
      if(vertices == 7) str = "Septagon";
      if(vertices == 8) str = "Octagon";
      if(vertices == 9) str = "Nonagon";
      if(vertices > 9) str = "Circle";
      putText(src, str, Point(x,y), 1, 2, Scalar(255,0,0),2);
    }
  }
  imshow("Output", src);
  waitKey(10);
  }
}
