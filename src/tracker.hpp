/*
 * tracker.hpp
 *
 *  Created on: 1 февр. 2016 г.
 *      Author: pavelzelenov
 */

#ifndef SRC_TRACKER_HPP_
#define SRC_TRACKER_HPP_

#include <map>
#include <string>
//#include <utility>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"

class tracker {
private:
	cv::Mat     ROI;
	cv::Mat     imgIn;
	cv::Mat     imgOver;
	std::string color;
	cv::Size    sz;
	cv::Point   pt;

	double      area;
	double      x;
	double      y;

public:
	tracker() {};
	tracker(cv::Mat&, cv::Mat&, cv::Rect&);


	virtual ~tracker();
//	double      x()        { return pt.x + 0*sz.width/2; };
//	double      y()        { return pt.y + 0*sz.height/2; };
	double      A()        { return area; };
	double      X()        { return 100*x/imgIn.cols; };
	double      Y()        { return 100*y/imgIn.rows; };
	std::string get_color(){ return color; };

	bool
	track();

private:
	void
	guessColor();
};

#endif /* SRC_TRACKER_HPP_ */
