#include <opencv2\opencv.hpp>
#include <array>
#include "opticalFlow.h"

using namespace cv;
using namespace std;

typedef std::vector<std::vector<cv::Point> > Contours;
typedef std::vector<cv::Vec4i> Hierarchy;

Mat Delta(Mat back,Mat frame){
	Mat diff;
	absdiff(back, frame, diff);
	//imshow("diff", diff);
	//moveWindow("diff", 640, 0);
	cv::inRange(diff, cv::Scalar(0, 0, 0), cv::Scalar(50, 50, 50), diff);
	return diff < 253;
}

float dist(Rect a, Rect b){
	int x1 = a.x + a.width / 2;
	int y1 = a.y + a.height / 2;
	int x2 = b.x + b.width / 2;
	int y2 = b.y + b.height / 2;

	return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

cv::Rect getVIP(vector<Rect> rects){
	//vector<int> weights(rects.size());
	return *std::max_element(rects.begin(), rects.end(), [](Rect a,Rect b){
		return a.area() < b.area();
	});

	/*for (int i = 0; i < rects.size(); i++)
	{
		int sum = 0;
		for (Rect r : rects)
		{
			sum += std::max((int)(100000 - dist(r, rects[i])),0) * rects[i].area();
		}
		weights[i] = sum;
	}

	return rects[std::max_element(weights.begin(), weights.end()) - weights.begin()];*/
}

void getExternalContours(Contours const& contours, vector<Rect>* retRects, Hierarchy const& hierarchy, int const idx)
{
	//for every contour of the same hierarchy level
	for (int i = idx; i >= 0; i = hierarchy[i][0])
	{
		retRects->push_back(cv::boundingRect(contours[i]));

		//for every of its internal contours
		for (int j = hierarchy[i][2]; j >= 0; j = hierarchy[j][0])
		{
			//recursively print the external contours of its children
			getExternalContours(contours, retRects, hierarchy, hierarchy[j][2]);
		}
	}
}


void DetectMotion(){
	VideoCapture cap(0);
	int counts = 0;
	Mat back;
	cap >> back;
	bool started = false;
	bool locating = true;
	bool adding = false;
	opticalFlow flow;

	while (true)
	{
		Mat frame;
		cap >> frame;
		Mat delta = Delta(back, frame);
		if (!flow.run(frame))
			locating = true;

			if (!started)
			{
				if (cv::countNonZero(delta) == 0 && counts++ > 4)
				{
					counts = 0;
					started = true;
					cout << cv::countNonZero(delta) << endl;
				}
				else
					frame.copyTo(back);
			}
			else{
				vector<vector<Point>> contours;

				if (locating)
				{
					cv::dilate(delta, delta, Mat(), Point(-1, -1), 1);
					cv::erode(delta, delta, Mat(), Point(-1, -1), 1);
					cv::dilate(delta, delta, Mat(), Point(-1, -1), 1);
					cv::erode(delta, delta, Mat(), Point(-1, -1), 1);

					Mat contourMask;
					delta.copyTo(contourMask);
					std::vector<cv::Vec4i> hierarchy;
					cv::findContours(contourMask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

					vector<Rect> rects;
					if (!contours.empty())
						getExternalContours(contours, &rects, hierarchy, 0);
					cv::imshow("New", contourMask);

					int size = rects.size();
					for (int i = 0; i < size; i++)
					{
						rects.push_back(Rect(rects[i]));
					}

					groupRectangles(rects, 1, 0.2);

					for (Rect r : rects)
					{
						cv::rectangle(frame, r, cv::Scalar(0, 0, 255), 1);
					}


					if (!rects.empty())
					{
						Rect vip = getVIP(rects);
						if (vip.area() > 100 && Rect(100, 100, 640 - 200, 480 - 200).contains(Point(vip.x + vip.width / 2, vip.y + vip.height / 2)))
						{
							cv::rectangle(frame, vip, cv::Scalar(0, 255, 255), 4);
							//for (int x = -10; x < 10; x += 2)
							{
								//for (int y = -10; y < 10; y += 2)
								{
									if (locating && adding)
									{
										flow.addPoint(vip.x + vip.width / 2
											//+x
											, vip.y + vip.height / 2
											//+y
											);
									}
								}
							}

							locating = false;
						}
					}
				}
			}

			int c = waitKey(10);
			if (c == 's')
				adding = true;
			if (c == 'o')
				return;
			if (c == 'k')
			{
				started = false;
				locating = true;
			}
			if (started){
				//imshow("back", back);
				imshow("delta", delta);
				cv::moveWindow("back", 0, 480);
				cv::moveWindow("delta", 640, 0);
			}

			imshow("main", frame);
			//cv::moveWindow("main", 0, 0);
	}
}