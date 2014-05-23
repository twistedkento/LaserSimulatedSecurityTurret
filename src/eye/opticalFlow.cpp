#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <ctype.h>

#include "opticalFlow.h"
#include "command.h"
#include <Windows.h>
#include "contour.h"

using namespace cv;
using namespace std;

Point2f point;
bool addRemovePt = false;

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
	winSize = Size(31, 31);
	needToInit = false;
	namedWindow("main");
	createTrackbar("eigentresh", "main", &eigen, 20);
	createTrackbar("epsilon", "main", &epsilon, 20);
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
		frame = image;
		cvtColor(image, gray, COLOR_BGR2GRAY);

		if (!points[0].empty())
		{
			Mat edges;
			Mat copy = image.clone();

			int thresh = 50;
			
			vector<uchar> status;
			vector<float> err;
			if (!prevGray.empty())
			{
				circle(image, points[0][0], 3, Scalar(0, 255, 0), -1, 8);
				
				vector<Mat> prevPyr;// = prevGray.clone();
				vector<Mat> nextPyr;// = gray.clone();
				const cv::TermCriteria termcrit = cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, double(epsilon) / 100.0);
				calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 50, termcrit, 0, double(eigen) / 1000.0);
				size_t i, k;
				
				if (!Rect(-20, -20, image.size().height, image.size().width + 40).contains(points[1][0])){
						std::cout << "Outside \n";
						clearPoints();
						cv::swap(prevGray, gray);
						return false;
					}

				circle(image, points[1][0], 3, Scalar(0, 255, 0), -1, 8);
				auto target = points[1][0];
            Command com;
			if (target.x > image.size().width / 2 + turningThreshold)
                com.setServoX(Command::CommandState::SERVO_DECREASE);
			if (target.y < image.size().height / 2 - turningThreshold)
                com.setServoY(Command::CommandState::SERVO_DECREASE);
			if (target.x < image.size().width / 2 - turningThreshold)
                com.setServoX(Command::CommandState::SERVO_INCREASE);
			if (target.y > image.size().height / 2 + turningThreshold)
                com.setServoY(Command::CommandState::SERVO_INCREASE);
            turret.send(com);

		}
		else
		{
			std::cout << "Empty \n";
			clearPoints();
			cv::swap(prevGray, gray);
			return false;
		}

		circle(image, cv::Point2f(640 / 2, 480 / 2), 3, Scalar(0, 0, 255), -1, 8);

		std::swap(points[1], points[0]);
		cv::swap(prevGray, gray);

		return true;
}
