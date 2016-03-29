/*
 * tracker.cpp
 *
 *  Created on: 1 февр. 2016 г.
 *      Author: pavelzelenov
 */

#include <string>
#include <map>

#include <iostream>

#include "tracker.hpp"

typedef
struct {
	int minH; int maxH;
} hueInterval;

std::multimap< std::string, hueInterval > hues = {
		{ "red",     {0, 30} },
		{ "yellow",  {30,  40} },
		{ "green",   {40,  90} },
		{ "cyan",    {90,  110} },
		{ "blue",    {110, 140} },
		{ "magenta", {140, 180} },
		{ "red",     {180, 255} }
//		{ "red", 0 },
//		{ "yellow", 42 },
//		{ "green", 70 },
//		{ "cyan", 128 },
//		{ "blue", 170 },
//		{ "magenta", 213 }
};

tracker::tracker(cv::Mat &in, cv::Mat &over, cv::Rect &b)
{
	imgIn = in;
	imgOver = over;
	ROI = imgIn(b);
	ROI.adjustROI(2*b.height, 2*b.height, 2*b.width, 2*b.width);
	ROI.locateROI(sz,pt);
	guessColor();

	track();
};
double      tracker::A()        { return area; };
double      tracker::X()        {
	float delta = (x-px)* step /tracker::XTRAPLTN_RATE;
//	if (delta<4) delta=0;
	return 100*(x+delta)/imgIn.cols;
};
double      tracker::Y()        {
	float delta = (y-py)* step /tracker::XTRAPLTN_RATE;
//	if (delta<4) delta=0;
	return 100*(y+delta)/imgIn.rows;
};

void
tracker::guessColor()
{
	std::map<std::string,double> colors;

	for ( auto &hue: hues) {
		cv::Mat imgHSV;
		cv::Mat imgTmp;

		cv::cvtColor( ROI, imgHSV, cv::COLOR_BGR2HSV);

		cv::inRange(imgHSV, cv::Scalar(hue.second.minH, 200, 150),
				cv::Scalar(hue.second.maxH, 255, 255), imgTmp);

		cv::erode(imgTmp, imgTmp,
				cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
		cv::dilate(imgTmp, imgTmp,
				cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
		cv::dilate(imgTmp, imgTmp,
				cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
		cv::erode(imgTmp, imgTmp,
				cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(imgTmp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		if (hierarchy.size() > 0) {
			int idx = 0;
			for (; idx >= 0; idx = hierarchy[idx][0]) {
				colors[hue.first] = contourArea(contours[idx]);
				std::cout << hue.first << "(" << contourArea(contours[idx]) << ") ";
				if (!color.empty() || colors[color] < colors[hue.first])
					color = hue.first;
			}
		}
	}
	std::cout << "---" << color << "\n";
};

bool
tracker::track()
{
	cv::Mat roiHSV;
	cv::Mat roiTres = cv::Mat::zeros(cv::Size(ROI.cols, ROI.rows), CV_8U);
	cv::cvtColor(ROI, roiHSV, cv::COLOR_BGR2HSV);

	step = 0;
	px = x;
	py = y;

	for(auto h = hues.begin(); h != hues.end(); h++){
		if ( h->first != color) continue;
		cv::Mat imgTmp;
		cv::inRange(roiHSV, cv::Scalar(h->second.minH, 100, 100),
				cv::Scalar(h->second.maxH, 255, 255), imgTmp);
		cv::bitwise_or(roiTres, imgTmp, roiTres);
	}

//	int tune_size = 10;
//	cv::erode(imgTres, imgTres,
//			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(tune_size,tune_size)));
//	cv::dilate(imgTres, imgTres,
//			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(tune_size,tune_size)));
//	cv::dilate(imgTres, imgTres,
//			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(tune_size,tune_size)));
//	cv::erode(imgTres, imgTres,
//			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(tune_size,tune_size)));

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(roiTres, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, pt);

	if (contours.size() > 0) {
		int idx = 0;

		double area_max = 0;
		int    cidx_max = 0;

		for (; idx >= 0; idx = hierarchy[idx][0]) {
			if( contourArea(contours[idx]) > area_max ) { area_max=contourArea(contours[idx]); cidx_max=idx; };
		}

		cv::Moments mu = moments( contours[cidx_max], false );
		cv::Point2f c = cv::Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );


		cv::Rect b = cv::boundingRect(contours[cidx_max]);
		cv::drawContours(imgOver, contours, cidx_max, cv::Scalar(255-rand()%128,255-rand()%128,255-rand()%128), 1, 8);

//		cv::Point2f c;
//		float r;
//		cv::minEnclosingCircle(contours[cidx_max], c, r);
//		cv::circle(imgOver,c,r*1,cv::Scalar(100,100,255),1,1);

//		x=b.x+b.width/2;
//		y=b.y+b.height/2;
		x = c.x;
		y = c.y;
		area = area_max;


		ROI = imgIn(b);
		ROI.adjustROI(2*b.height, 2*b.height, 2*b.width, 2*b.width);
//		ROI.adjustROI(40, 40, 40, 40);

		ROI.locateROI(sz,pt);
		cv::putText(imgOver, color,
					pt, cv::FONT_HERSHEY_PLAIN, 2,
					cv::Scalar(255,255,255), 2, 8);
//		std::cout << color << " " << pt << "\n";
		cv::rectangle(imgOver, cv::Rect(pt.x,pt.y,ROI.cols,ROI.rows), cv::Scalar(255,255,255), 1);

		// failover for first track
		if (px == -1 ) px = x;
		if (py == -1 ) py = y;

		return true;
	}
	else {
		std::cerr << "Lost [" << color << "] object\n";
		return false;
	}

}

tracker::~tracker()
{
	// TODO Auto-generated destructor stub
}

