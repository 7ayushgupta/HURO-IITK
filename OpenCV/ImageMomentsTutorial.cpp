//From OpenCV tutorials
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat src;
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
void thresh_callback(int, void*);

int main( int argc, char** argv )
{
  VideoCapture capture;
	capture.open(0);
  while(1){
  capture.read(src);
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  imshow( "source", src );
  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );
  char a = waitKey(50);
  if(a=='q')
    break;
  }
  return(0);
}
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
  vector<Moments> mu(contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }
  vector<Point2f> mc( contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( static_cast<float>(mu[i].m10/mu[i].m00) , static_cast<float>(mu[i].m01/mu[i].m00) ); }
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
  printf("\t Info: Area and Contour Length \n");
  for( size_t i = 0; i< contours.size(); i++ )
     {
       printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", (int)i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }
}
