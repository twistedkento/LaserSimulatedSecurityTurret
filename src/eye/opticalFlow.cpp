#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <ctype.h>

#include "opticalFlow.h"
#include "command.h"
//#include <Windows.h>
#include "contour.h"

using namespace cv;
using namespace std;

Point2f point;
bool addRemovePt = false;

static void doFilter(Mat img) {
    const int filterValue = 8;
    for (int i = 0; i < (filterValue/2); ++i) {
        dilate(img,img,Mat());
    }
    for (int i = 0; i < filterValue; ++i) {
        erode(img,img,Mat());
    }
    for (int i = 0; i < (filterValue/2); ++i) {
        dilate(img,img,Mat());
    }
}

Mat test2;
static void getStrangeDiff(const Mat &src_img) {
    Mat new_img;
    vector<Mat> channels; 
    cvtColor( src_img, new_img, CV_BGR2YCrCb );
    split(new_img ,channels);
    equalizeHist( channels[0], channels[0] );
    absdiff(channels[1], channels[2], test2);
    inRange(test2, Scalar(20,20,20), Scalar(140,140,140),test2);
    blur(test2, test2, Size(3,3));
    doFilter(test2);
    doFilter(test2);
    //imshow("test2_2", test2);
}

void opticalFlow::addPoint(int x, int y)
{
	vector<Point2f> tmp;
	tmp.push_back(Point(x,y));
	//cornerSubPix(gray, tmp, winSize, cvSize(-1, -1), termcrit);
	points[1].push_back(tmp[0]);
	points[0].push_back(tmp[0]);
}

opticalFlow::opticalFlow(){
	subPixWinSize = Size(10, 10);
	winSize = Size(16, 16);
	needToInit = false;
	namedWindow("frame");
	createTrackbar("eigentresh", "frame", &eigen, 20);
	createTrackbar("epsilon", "frame", &epsilon, 20);
}


void opticalFlow::clearPoints(){
	points[0].clear();
	points[1].clear();
}

float dist(Point a, Point b){
	return (float)(a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

const int turningThreshold = 20;

bool opticalFlow::run(Mat image)
{
    //getStrangeDiff(image);
		image.copyTo(frame);
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		if (!points[0].empty())
		{
			Mat edges;
			Mat copy = frame.clone();

			int thresh = 50;
			
			vector<uchar> status;
			vector<float> err;
			if (!prevGray.empty())
			{
				//circle(image, points[0][0], 3, Scalar(0, 255, 0), -1, 8);
				
				vector<Mat> prevPyr;// = prevGray.clone();
				vector<Mat> nextPyr;// = gray.clone();
				const cv::TermCriteria termcrit = cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, double(epsilon) / 100.0);
				calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 1000, termcrit, 0, double(eigen) / 1000.0);
				size_t i, k;
				
				if (!Rect(0, 0, frame.size().width, frame.size().height).contains(points[1][0])){
						std::cout << "Outside \n";
						clearPoints();
						cv::swap(prevGray, gray);
						return false;
					}

				circle(frame, points[1][0], 6, Scalar(0, 255, 0), -1, 8);
				auto target = points[1][0];
            Command com;
			if (target.x > frame.size().width / 2 + turningThreshold) com.setServoX(Command::CommandState::SERVO_DECREASE);
			if (target.y < frame.size().height / 2 - turningThreshold) com.setServoY(Command::CommandState::SERVO_INCREASE);
			if (target.x < frame.size().width / 2 - turningThreshold) com.setServoX(Command::CommandState::SERVO_INCREASE);
			if (target.y > frame.size().height / 2 + turningThreshold) com.setServoY(Command::CommandState::SERVO_DECREASE);
            turret.send(com);
            }

		}
		else
		{
			/*std::cout << "Empty \n";
			clearPoints();
			cv::swap(prevGray, gray);
            */
			//return false;
		}

		circle(frame, cv::Point2f(frame.size().width / 2, frame.size().height / 2), 3, Scalar(0, 0, 255), -1, 8);

		std::swap(points[1], points[0]);
		cv::swap(prevGray, gray);

        imshow("frame", frame);


		return true;
}
