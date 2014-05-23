#include <opencv2/opencv.hpp>
#include <array>
#include "opticalFlow.h"
#include "contour.h"

using namespace cv;
using namespace std;

typedef std::vector<std::vector<cv::Point> > Contours;

Mat Delta(Mat back,Mat frame){
	Mat diff;
	absdiff(back, frame, diff);
	cv::inRange(diff, cv::Scalar(0, 0, 0), cv::Scalar(50, 50, 50), diff);
	return diff < 253;
}

cv::Rect getVIP(vector<Rect> rects){
	return *std::max_element(rects.begin(), rects.end(), [](Rect a,Rect b){
		return a.area() < b.area();
	});
}

void DetectMotion(){
	VideoCapture cap;
    cap.set(CV_CAP_PROP_BRIGHTNESS, 0.65);
    cap.set(CV_CAP_PROP_CONTRAST, 0.35);
    cap.set(CV_CAP_PROP_EXPOSURE, 0.99);
	int counts = 0;
	bool started = false;
	bool locating = true;
	bool adding = false;
	bool test = true;
	int changeDiscardThreshold = 100;
	opticalFlow flow;
	Mat back;
	while (back.empty())
	{
		if (test) cap.open(0);
		else cap.open("udp://@0.0.0.0:5001");
		cap >> back;
		std::cout << "Reopening";
	}

	while (true)
	{
		Mat frame;
		cap >> frame;
		for (int i = 0; i < 3; i++)
			cap.grab();

		Mat delta = Delta(back, frame);
		int change = cv::countNonZero(delta);
		if (!flow.run(frame))
			locating = true;

			if (!started)
			{
				if (change == 0 && counts++ > 4)
				{
					counts = 0;
					started = true;
				}
				else
					frame.copyTo(back);
			}
			else{
				if(change > changeDiscardThreshold )
					continue;

				vector<vector<Point>> contours;

				if (locating)
				{
					cv::blur(delta, delta, Size(32, 32));					

					Mat contourMask;
					delta.copyTo(contourMask);
					std::vector<cv::Vec4i> hierarchy;
					//FIND EXTERNAL CONTOURS
					auto contours = contour::getExternalContours(delta);

					if (!contours.empty())
					{
						auto target = std::max_element(contours.begin(), contours.end());
						Rect vip = target->boundary;

						if (vip.area() > 100 && Rect(padding, padding, frame.size().width - padding * 2, frame.size().height - padding * 2).contains(Point(vip.x + vip.width / 2, vip.y + vip.height / 2)))
						{
							//cv::rectangle(frame, vip, cv::Scalar(0, 255, 255), 4);

							if (locating && adding)
							{
								auto moment = cv::moments(target->realcontour);
								int centerx = moment.m10 / moment.m00;
								int centery = moment.m01 / moment.m00;
								Mat cpy;
								frame.copyTo(cpy);
								target->draw(cpy);
								imshow("contours", cpy);
								flow.addPoint(centerx, centery);
								locating = false;
							}
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

			imshow("main", frame);
	}
}
