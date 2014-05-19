#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <ctype.h>

#include "opticalFlow.h"
//#include "turret.h"
#include "command.h"

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
}


void opticalFlow::clearPoints(){
	points[0].clear();
	points[1].clear();
}

float dist(Point a, Point b){
	return (float)(a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

const int turningThreshold = 20;

cv::Point getVIP(vector<Point2f> points){
	vector<int> weights(points.size());

	for (int i = 0; i < points.size(); i++)
	{
		int sum = 0;
		for (Point2f r : points)
		{
			sum += std::max((int)(100000 - dist(r, points[i])), 0);
		}
		weights[i] = sum;
	}

	return points[std::max_element(weights.begin(), weights.end()) - weights.begin()];
}

bool opticalFlow::run(Mat& image)
{
		cvtColor(image, gray, COLOR_BGR2GRAY);

		if (needToInit)
		{
			// automatic initialization
			goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
			cornerSubPix(gray, points[1], subPixWinSize, Size(-1, -1), termcrit);
			addRemovePt = false;
			needToInit = false;
		}
		if (!points[0].empty())
		{
			vector<uchar> status;
			vector<float> err;
			if (prevGray.empty())
				gray.copyTo(prevGray);
			calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
				3, termcrit, 0, 0.001);
			size_t i, k;
			for (i = k = 0; i < points[1].size(); i++)
			{
				if (dist(points[0][i], points[1][i]) < 0.1f){
					//cout << "Resetting"<<endl;
					//return false;
				}

				if (!Rect(50, 50, image.size().height - 100, image.size().width - 100).contains(points[1][i])){
					clearPoints();
					cv::swap(prevGray, gray);
					return false;
				}

				//circle(image, points[1][i], 3, Scalar(0, 255, 0), -1, 8);
			}
			auto target = getVIP(points[1]);
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

			circle(image, target, 6, Scalar(0, 255, 0), -1, 8);
		}
		else
		{
			clearPoints();
			cv::swap(prevGray, gray);
			return false;
		}

		circle(image, cv::Point2f(640 / 2, 480 / 2), 3, Scalar(0, 0, 255), -1, 8);

		std::swap(points[1], points[0]);
		cv::swap(prevGray, gray);

		return true;
}
