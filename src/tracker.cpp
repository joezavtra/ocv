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

std::map< std::string, hueInterval > hues = {
		{ "red",     {-10, 7} },
		{ "yellow",  {7,  40} },
		{ "green",   {40,  90} },
		{ "cyan",    {90,  110} },
		{ "blue",    {110, 140} },
		{ "magenta", {140, 160} }
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

//double
//tracker::x()
//{
//	return pt.x+sz.width/2;
//};
//
//double
//tracker::X()
//{
//};

bool
tracker::track()
{
	cv::Mat imgHSV;
	cv::Mat imgTres;
	cv::cvtColor(ROI, imgHSV, cv::COLOR_BGR2HSV);
	cv::inRange(imgHSV, cv::Scalar(hues[color].minH, 200, 150),
			cv::Scalar(hues[color].maxH, 255, 255), imgTres);

	cv::erode(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	cv::dilate(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	cv::dilate(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	cv::erode(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(imgTres, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, pt);

	if (hierarchy.size() > 0) {
		int idx = 0;

		double area_max = 0;
		int    cidx_max = 0;

		for (; idx >= 0; idx = hierarchy[idx][0]) {
			if( contourArea(contours[idx]) > area_max ) { area_max=contourArea(contours[idx]); cidx_max=idx; };
		}


		cv::Rect b = cv::boundingRect(contours[cidx_max]);
		cv::drawContours(imgOver, contours, cidx_max, cv::Scalar(255,255,255), 1, 8);

		area = area_max;
		x=b.x+b.width/2;
		y=b.y+b.height/2;

		ROI = imgIn(b);
		ROI.adjustROI(2*b.height, 2*b.height, 2*b.width, 2*b.width);

		ROI.locateROI(sz,pt);
		cv::putText(imgOver, color,
					pt, cv::FONT_HERSHEY_PLAIN, 2,
					cv::Scalar(255,255,255), 2, 8);
		std::cout << color << " " << pt << "\n";
		cv::rectangle(imgOver, cv::Rect(pt.x,pt.y,ROI.cols,ROI.rows), cv::Scalar(255,255,255), 2);
		return true;
	}
	else {
		return false;
	}

}

tracker::~tracker()
{
	// TODO Auto-generated destructor stub
}

