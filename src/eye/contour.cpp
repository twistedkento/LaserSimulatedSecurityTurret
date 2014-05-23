#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\types_c.h>
#include "contour.h"

using namespace std;
using namespace cv;


typedef std::vector<cv::Vec4i> Hierarchy;

void contour::_getExternalContours(vector<vector<Point>> const& contours, vector<contour>* result, Hierarchy const& hierarchy, int const idx)
{
	//for every contour of the same hierarchy level
	for (int i = idx; i >= 0; i = hierarchy[i][0])
	{
		result->push_back(contours[i]);
		//for every of its internal contours
		for (int j = hierarchy[i][2]; j >= 0; j = hierarchy[j][0])
		{
			//recursively print the external contours of its children
			_getExternalContours(contours, result, hierarchy, hierarchy[j][2]);
		}
	}
}

vector<contour> contour::getExternalContours(Mat delta){
	vector<vector<Point>> contours;
	Mat contourMask;
	delta.copyTo(contourMask);
	std::vector<cv::Vec4i> hierarchy;
	findContours(contourMask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	vector<Rect> rects;
	vector<contour> externalcontours;
	if (!contours.empty())
		_getExternalContours(contours, &externalcontours, hierarchy, 0);
	return externalcontours;
}
#include <numeric>
#include <functional>

std::pair<Scalar, Scalar> contour::getColorRange(cv::Mat frame)
{
	Scalar min = Scalar(255,255,255);
	Scalar max = Scalar(0, 0, 0);
	vector<vector<double>> cols(3);
	
	Mat edges;
	Mat copy;
	/*boundary.x -= boundary.width;
	boundary.y -= boundary.height;
	boundary.width *= 1.5;
	boundary.height *= 1.5;
	if (boundary.x < 0) boundary.x = 0;
	if (boundary.y < 0) boundary.y = 0;
	*/

	/*Mat lab;
	for (int x = boundary.x; x < boundary.x + boundary.width;++x)
	{
		for (int y = boundary.y; y < boundary.y + boundary.height; ++y)
		{
			if (pointPolygonTest(realcontour, Point(x, y), false) > 0){
				Scalar color = frame.at<cv::Vec3b>(Point(x, y));
				for (int i = 0; i < 3; i++)
				{
					cols[i].push_back(color[i]);
					if (color[i] < min[i])
						min[i] = color[i];
					if (color[i] > max[i])
						max[i] = color[i];
				}
			}
		}
	}


	int index = 0;
	for (auto& color : cols)
	{
		double mean = std::accumulate(color.begin(), color.end(), 0) / color.size();
		std::vector<int> diff(color.size());
		std::transform(color.begin(), color.end(), diff.begin(),
			std::bind2nd(std::minus<double>(), mean));
		double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
		double stdev = std::sqrt(sq_sum / color.size());
		min[index] = mean - stdev*4.0;
		max[index] = mean + stdev*4.0;
		index++;
	}
	min[3] = 255;
	max[3] = 255;*/
	return make_pair(min, max);
}