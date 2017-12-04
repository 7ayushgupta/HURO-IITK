//Object Detection (HURO Team)
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <fstream>
#include <iostream>
#include "math.h"
#include "string.h"

using namespace cv;
using namespace std;
//initial min and max HSV filter values.c
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 37;
int S_MIN = 77;
int S_MAX = 256;
int V_MIN = 144;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";


void on_trackbar(int, void*)
{//This function gets called whenever a
 // trackbar position is changed
}

string intToString(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars() {
	//create window for trackbars
	namedWindow(trackbarWindowName, 0);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}

void drawObject(int x, int y, Mat &frame, Point2f center, float radius) {

	circle(frame, center, radius, Scalar(0, 255, 2), 2);
	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);

}

void morphOps(Mat &thresh) {
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);
	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed) {
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	int index, maxindex;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0)
	{
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects < MAX_NUM_OBJECTS) {
			for (index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
					maxindex = index;
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;

				//index++;

			}
			//let user know you found an object
			if (objectFound == true) {
				// printing values for different positions in grid
				if (x < cameraFeed.cols / 3 && y < cameraFeed.rows / 3 && x>0 && y>0)
					cout << "1\n";
				else if (x < cameraFeed.cols * 2 / 3 && x>cameraFeed.cols / 3 && y > 0 && y < cameraFeed.rows / 3)
					cout << "2\n";
				else if (x<cameraFeed.cols && x>cameraFeed.cols * 2 / 3 && y > 0 && y < cameraFeed.rows / 3)
					cout << "3\n";
				else if (x < cameraFeed.cols / 3 && x>0 && y > cameraFeed.rows / 3 && y < cameraFeed.rows * 2 / 3)
					cout << "4\n";
				else if (x > cameraFeed.cols / 3 && x<cameraFeed.cols * 2 / 3 && y>cameraFeed.rows / 3 && y < cameraFeed.rows * 2 / 3)
					cout << "5\n";
				else if (x > cameraFeed.cols * 2 / 3 && x < cameraFeed.cols && y < cameraFeed.rows * 2 / 3 && y > cameraFeed.rows / 3)
					cout << "6\n";
				else if (x > 0 && x<cameraFeed.cols / 3 && y>cameraFeed.rows * 2 / 3 && y < cameraFeed.rows)
					cout << "7\n";
				else if (x > cameraFeed.cols / 3 && x<cameraFeed.cols * 2 / 3 && y>cameraFeed.rows * 2 / 3 && y < cameraFeed.rows)
					cout << "8\n";
				else if (x > cameraFeed.cols * 2 / 3 && x<cameraFeed.cols && y>cameraFeed.rows * 2 / 3 && y < cameraFeed.rows)
					cout << "9\n";
				Point2f center;
				float radius;
				putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				int flag = 1;
				if (flag == 1)
				{
					minEnclosingCircle(contours[maxindex], center, radius);
					float ball_area = (3.14159*radius*radius);     // minEnclosingCircle(contours[maxindex], center, radius).area();
					float distance = sqrt(925 * 925 * (16.5*15.0) / ball_area);
					cout << distance << "\n";
					drawObject(x, y, cameraFeed, center, radius);
				}
				else if (flag == 0)
				{
					boundingRect(contours[maxindex]);
					float basket_area = boundingRect(contours[maxindex]).area();
					float distance = sqrt(925 * 925 * (16.5*15.0) / basket_area);
					cout << distance << "\n";
					rectangle(cameraFeed, boundingRect(contours[maxindex]).tl(), boundingRect(contours[maxindex]).br(), Scalar(0, 255, 0), 2, 8, 0);
					//drawObject(x, y, cameraFeed, center, radius);
				}
			}



		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
	else
		cout << "Object not found\n";
}
void drawingcontours(Mat &img)
{

}
int main(int argc, char* argv[])
{
	//some boolean variables for different functionality within this
	//program
	bool trackObjects = true;
	bool useMorphOps = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x = 0, y = 0;
	//create slider bars for HSV filtering

	createTrackbars();

	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (waitKey(10) != 'q') {
		//store image to matrix
		capture.read(cameraFeed);
		Point p1, p2, p3, p4, p5, p6, p7, p8;
		p1.x = 0; p1.y = cameraFeed.rows / 3;
		p2.x = cameraFeed.cols; p2.y = cameraFeed.rows / 3;
		p3.x = 0; p3.y = cameraFeed.rows * 2 / 3;
		p4.x = cameraFeed.cols; p4.y = cameraFeed.rows * 2 / 3;
		p5.x = cameraFeed.cols / 3; p5.y = 0;
		p6.x = cameraFeed.cols * 2 / 3; p6.y = 0;
		p7.x = cameraFeed.cols / 3; p7.y = cameraFeed.rows;
		p8.x = cameraFeed.cols * 2 / 3; p8.y = cameraFeed.rows;
		line(cameraFeed, p1, p2, 0, 2, 8, 0);
		line(cameraFeed, p3, p4, 0, 2, 8, 0);
		line(cameraFeed, p5, p7, 0, 2, 8, 0);
		line(cameraFeed, p6, p8, 0, 2, 8, 0);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if (useMorphOps)
			morphOps(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if (trackObjects)
			trackFilteredObject(x, y, threshold, cameraFeed);

		//drawingContours(src);
		//show frames
		imshow(windowName2, HSV);
		imshow(windowName3, threshold);
		imshow(windowName, cameraFeed);
		//imshow(windowName1,HSV);
		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}
	return 0;
}
