#include "opencv2/opencv.hpp"
#include <array>
#include "opticalFlow.h"
#include "contour.h"

using namespace cv;
using namespace std;

const int padding = 50;

typedef std::vector<std::vector<cv::Point> > Contours;

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
Mat Delta(Mat back,Mat frame){
	Mat diff;
    absdiff(back, frame, diff);
    //imshow("diff delta",diff);
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
	//VideoCapture cap("udp://@0.0.0.0:5001");
    cap.set(CV_CAP_PROP_BRIGHTNESS, 0.65);
    cap.set(CV_CAP_PROP_CONTRAST, 0.35);
    cap.set(CV_CAP_PROP_EXPOSURE, 0.99);
	int counts = 0;
    int changecount = 0;
	bool started = false;
	bool locating = true;
	bool adding = true;
	bool test = false;
	int changeDiscardThreshold = 50000;
	opticalFlow flow;
	Mat back;
	while (back.empty())
	{
		if (test) cap.open(0);
		else cap.open("udp://@0.0.0.0:5001");
		cap >> back;
		std::cout << "Reopening";
	}

    Mat frame;
	while (true)
	{
		cap >> frame;
      vector<Mat> channels; 
        cvtColor( frame, frame, CV_BGR2YCrCb );
        split(frame,channels);
        equalizeHist( channels[0], channels[0] );
        merge(channels,frame);
        cvtColor(frame, frame, CV_YCrCb2BGR);
        if (changeDiscardThreshold == 0) {
            changeDiscardThreshold = frame.size().width * frame.size().height;
        }
        
        if (false) {
		    for (int i = 0; i < 3; i++)
			    cap.grab();
        } else {
		    for (int i = 0; i < 1; i++)
			    cap.grab();
        }

		Mat delta = Delta(back, frame);
        doFilter(delta);
        //imshow("lol",delta);
		int change = cv::countNonZero(delta);
		if (!flow.run(frame)) {
			locating = true;
            started = false;
        }
            

            //circle(frame,Point(10,10), 3,Scalar(100,100,100),-1,50);
			if (!started)
			{
				if (change <= (changeDiscardThreshold/100) && counts++ > 4)
				{
					counts = 0;
					started = true;
				}
				else {
					frame.copyTo(back);
                }
			}
			else{
				if(change > (changeDiscardThreshold / 2) )
                {
                    if (changecount >= 10) {
                        started = false;
                        adding = true;
                        changecount = 0;
                    } else {
                        ++changecount;
                    }
					continue;
                } else {
                    changecount = 0;
                }

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

						if (vip.area() > 10000 && Rect(padding, padding, frame.size().width - padding * 2, frame.size().height - padding * 2).contains(Point(vip.x + vip.width / 2, vip.y + vip.height / 2)))
						{
							//cv::rectangle(frame, vip, cv::Scalar(0, 255, 255), 4);

							if (locating && adding)
							{
								//auto moment = cv::moments(target->realcontour);
								int centerx = vip.x + (vip.width / 2);
								int centery = vip.y + (vip.height / 2);
								//Mat cpy;
								//frame.copyTo(cpy);
								//target->draw(cpy);
								//imshow("contours", cpy);
								flow.addPoint(centerx, centery);
								locating = false;
							}
						}
					}
				}
			}

			int c = waitKey(15);
			if (c == 's')
				adding = true;
			if (c == 'o')
				return;
			if (c == 'k')
			{
				started = false;
				locating = true;
			}

			//imshow("back", back);
			//imshow("main", frame);
	}
}
