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

	// extrapolation
	double      px   = -1;
	double      py   = -1;
	int         step =  0;

public:
	static const int XTRAPLTN_RATE = 3;

	tracker() {};
	tracker(cv::Mat&, cv::Mat&, cv::Rect&);


	virtual ~tracker();

	void        next_step () { step++; };
	bool        is_extrapolated()        { return step>0; };

	double      A();//        { return area; };
	double      X();//        { return 100*(x + (x-px)* step /tracker::XTRAPLTN_RATE)/imgIn.cols; };
	double      Y();//        { return 100*(y + (y-py)*(step++)/tracker::XTRAPLTN_RATE)/imgIn.rows; };
	std::string get_color(){ return color; };

	bool
	track();

private:
	void
	guessColor();
};

#endif /* SRC_TRACKER_HPP_ */
