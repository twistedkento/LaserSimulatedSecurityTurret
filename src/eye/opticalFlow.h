#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include "turret.h"
struct opticalFlow{
    Turret turret;
	const cv::TermCriteria termcrit = cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	
	cv::Size subPixWinSize,winSize; 

	const int MAX_COUNT = 500;
	bool needToInit = false;
	bool nightMode = false;
	bool hasFace = false;

	int eigen = 15;
	int epsilon = 15;
	
	cv::Mat frame;
	cv::Mat gray;
	cv::Mat prevGray;

	std::vector<cv::Point2f> points[2];

	void clearPoints();
	opticalFlow();
	bool run(cv::Mat frame);
	void addPoint(int x, int y);
};
