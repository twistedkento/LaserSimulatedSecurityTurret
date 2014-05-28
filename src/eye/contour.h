#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
typedef std::vector<cv::Point> cvcontour;
struct contour{
	contour(){
	}
	contour(cvcontour contour){
		realcontour = contour;
		boundary = cv::boundingRect(contour);
		center.x = boundary.x + boundary.width / 2;
		center.y = boundary.y + boundary.height/ 2;
		moment = cv::moments(realcontour);
	}
	cv::Point2f center;
	typedef std::vector<cv::Vec4i> Hierarchy;
	
	static std::vector<contour> getExternalContours(cv::Mat delta);
	cvcontour realcontour;
	cv::Rect boundary;
	cv::Moments moment;
	std::pair<cv::Scalar, cv::Scalar> getColorRange(cv::Mat frame);
	void draw(cv::Mat frame, cv::Scalar col = cv::Scalar(0, 0, 255))
	{
		for (auto p : realcontour)
		{
			cv::circle(frame, p, 2, col);
		}
	}

	bool operator < (contour b){
		return boundary.area() < b.boundary.area();
	}
private:
	static void _getExternalContours(std::vector<std::vector<cv::Point>> const& contours, std::vector<contour>* result, Hierarchy const& hierarchy, int const idx);
};

