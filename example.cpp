#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <fstream>
#include <iostream>
#include "math.h"
#include "string.h"


using namespace cv;
using namespace std;

int H_MIN,H_MAX,S_MIN,S_MAX,V_MIN,V_MAX;

void createTrackbars(){
	//create window for trackbars

	char trackbarWindowName[50] = "trackbars";
    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window

    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, 255);
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, 255);
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, 255);
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, 255);
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, 255);
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, 255);


}


int main(int argc, char** argv)
{
	Mat src;
  Mat dst;
  Mat threshold;
  Mat blur;
  Mat erode;
  Mat contours;
  createTrackbars();
	VideoCapture capture;
	capture.open(0);
	while(1)
	{
		capture.read(src);
    imshow("input",src);

    cvtColor(src,dst,COLOR_BGR2HSV);

    inRange(dst,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
    imshow("threshold",threshold);
    //GaussianBlur( threshold, blur, Size( 3,3), 0, 0 );
    //erode(threshold,erode,Mat(),Point(-1,-1));
    //imshow("erode",erode);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point>approx;

    findContours( threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    if(contours.size()>0)
    {
      Scalar color = Scalar(0,255,0);
	  	for( int i = 0; i< contours.size(); i++ )
	     	{
		    		drawContours( src, contours, i, color, 2, 8, hierarchy, 0, Point() );
      	}
    }
    if(contours.size()>0)
    {
    for(int i = 0; i< contours.size(); i++ )
	     	{
		    	approxPolyDP(Mat(contours[i]), approx,3,true);
    }
    }

    char a = waitKey(50);
		if(a=='q')
			break;
  }
  return 0;
}
