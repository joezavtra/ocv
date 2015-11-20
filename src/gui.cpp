/*
 * gui.cpp
 *
 *  Created on: 11 нояб. 2015 г.
 *      Author: zelenov
 */

#include <limits.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"
#include "gui.hpp"

int fontFace = cv::FONT_HERSHEY_PLAIN;
double fontScale = 2;
int thickness = 1;



std::map<std::string, gui::hueInterval> hues = {
		{ "red", {-10,10} },
		{ "yellow", {10,30} },
		{ "green", {50,100} },
		{ "cyan", {90,110} },
		{ "blue", {110,140} },
		{ "magenta", {140,160} }
//		{ "red", 0 },
//		{ "yellow", 42 },
//		{ "green", 70 },
//		{ "cyan", 128 },
//		{ "blue", 170 },
//		{ "magenta", 213 }
};

gui::gui(int device)
{
		camera.open(device);
//	camera.open("/Users/zelenov/ws/ocv/balls.mov");

	if (!camera.isOpened())  // if not success, exit program
	{
		std::cerr << "Cannot open the web cam" << device << "\n";
		exit(-1);
	}
	resIn.x = camera.get(CV_CAP_PROP_FRAME_WIDTH);
	resIn.y = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

	imgOver = cv::Mat::zeros(cv::Size(resIn.x, resIn.y), CV_8UC3);
	imgOut = cv::Mat::zeros(
			cv::Size((int) resIn.x * (1 + 0 * qThumb),
					(int) resIn.y * (1 + 0 * qThumb)), CV_8UC3);

	std::cout << "Camera " << resIn.x << "x" << resIn.y << " opened\n";

	createWindows();
	resetBorders();
}

gui::~gui()
{
	// TODO Auto-generated destructor stub
}

void gui::loop()
{
	while (true) {
		getFrame();
//		tresholdFrame();
//		processFrame();

		for ( auto &hue: hues) {
			cv::Mat imgHT = tresholdFrameH(hue.second);
//			std::cerr << hue.first << " " << hue.second << "\n";

			std::vector<std::vector<cv::Point> > contours;
			std::vector<cv::Vec4i> hierarchy;
			cv::findContours(imgHT, contours, hierarchy, CV_RETR_EXTERNAL, //CV_RETR_CCOMP,
					CV_CHAIN_APPROX_SIMPLE);

//			imshow("qqq", imgHT);
//			std::cerr << contours.size() << "\n";

			if (hierarchy.size() > 0) {
				int idx = 0;
				for (; idx >= 0; idx = hierarchy[idx][0]) {

					if (contourArea(contours[idx]) < 50 ) continue;

					cv::Moments m = cv::moments(contours[idx]);
					int x = (int) (m.m10 / m.m00);
					int y = (int) (m.m01 / m.m00);
					cv::Vec3b p = imgHSV.at<cv::Vec3b>(y,x);

					cv::Rect bound = cv::boundingRect(contours[idx]);
					cv::rectangle(imgOver, bound, cv::Scalar(255,255,255), 2);
//
//					std::ostringstream log_str;
//					log_str << "HSV: " << (int) p[0] << " " << (int) p[1] << " "
//							<< (int) p[2];

					cv::Point tl = bound.tl();
					cv::Point br = bound.tl();

					cv::putText(imgIn, (cv::String)std::to_string(tl.x),
							cv::Point(tl.x,tl.y-20), fontFace, 1,
							cv::Scalar(255,255,255), 1, 8);
					cv::putText(imgIn, (cv::String)std::to_string(tl.y),
							cv::Point(tl.x-50,tl.y+20), fontFace, 1,
							cv::Scalar(255,255,255), 1, 8);


					cv::putText(imgIn, cv::String(hue.first.c_str()),
							bound.br(), fontFace, 2,
							cv::Scalar(255,255,255), 2, 8);

					cv::putText(imgIn, std::to_string(p[0]),
							cv::Point(tl.x,br.y+40), fontFace, 1,
							cv::Scalar(255,255,255), 1, 8);
					cv::putText(imgIn, std::to_string(p[1]),
							cv::Point(tl.x,br.y+55), fontFace, 1,
							cv::Scalar(255,255,255), 1, 8);
					cv::putText(imgIn, std::to_string(p[2]),
							cv::Point(tl.x,br.y+70), fontFace, 1,
							cv::Scalar(255,255,255), 1, 8);
				}
			}
		}

		show();
	}
}

void gui::show()
{
	cv::Mat imgTmp = imgIn;
//	imgTmp = imgOver + imgTmp;
	bitwise_or(imgOver, imgTmp, imgTmp);
	imgOver *= 0.7;
	imgTmp.copyTo(imgOut(cv::Rect(0, 0, imgIn.cols, imgIn.rows)));

//	resize(imgTres, imgTmp, cv::Size(), qThumb, qThumb);
//	cv::cvtColor(imgTmp, imgTmp, cv::COLOR_GRAY2BGR);
//	imgTmp.copyTo(
//			imgOut(
//					cv::Rect(imgOut.cols - imgTmp.cols, 0, imgTmp.cols,
//							imgTmp.rows)));

	time(&second_end);
	fps_cnt++;
	sec = difftime(second_end, second_start);
	fps = fps_cnt / sec;
	if (sec >= 1) {
		sprintf(fps_str, "FPS: %.2f [%dX%d]", fps, resIn.x, resIn.y);
		fps_cnt = 0;
	}
//	cv::putText(imgTmp, cv::String(fps_str), cv::Point(20, 20), fontFace, fontScale,
//			Scalar(50, 50, 50), thickness + 2, 8);
//	cv::putText(imgTmp, cv::String(fps_str), cv::Point(20, 20), fontFace,
//			fontScale, cv::Scalar(100, 255, 100), thickness, 8);
//	cv::putText(imgTmp, cv::String(fps_str), cv::Point(20, 20), fontFace,
//			fontScale, cv::Scalar(100, 255, 100), thickness, 8);

	log(fps_str, "FPS");
	printLog();
	resize(imgOut, imgTmp, cv::Size(), qGlob, qGlob);
	imshow("Main", imgTmp);

	int key = cv::waitKey(1);
	switch (key) {
		case 27: {
			std::cout << "esc key is pressed by user\n";
			exit(0);
		}
		case 32:
			resetBorders();
			break;
	}
}

void gui::processFrame()
{
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(imgTres, contours, hierarchy, CV_RETR_CCOMP,
			CV_CHAIN_APPROX_SIMPLE);

	if (hierarchy.size() > 0) {
		cv::Mat imgTmp;
//		cv::blur(imgIn, imgTmp, cv::Size(3, 3));

		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0]) {

//				cv::Moments m = cv::moments(contours[idx]);
//				int x = (int) (m.m10 / m.m00); //contours[idx][0].x
//				int y = (int) (m.m01 / m.m00); //contours[idx][0].y
//
//				cv::Vec3b p = imgTmp.at<cv::Vec3b>(y,x);

//				cout << idx << " > " <<  contours[idx] << "\n";
			cv::Scalar color(128 + rand() & 127, 128 + rand() & 127,
					128 + rand() & 127);
			cv::drawContours(imgIn, contours, idx, color, 1, 8, hierarchy);
//			cv::drawContours(imgIn, contours, idx, (cv::Scalar)p, -1, 8, hierarchy);
		}
	}
}

void gui::tresholdFrame()
{
//	bilateralFilter(imgHSV, imgHSV, 5, 80, 80);
//	cv::blur(imgIn, imgTmp, cv::Size(3, 3));
	cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV),
			cv::Scalar(iHighH, iHighS, iHighV), imgTres);

	cv::erode(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::erode(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
}

cv::Mat gui::tresholdFrameH(hueInterval &interval)
{
//	cv::blur(imgHSV, imgHSV, cv::Size(3, 3));
	cv::Mat imgTmp;

	cv::inRange(imgHSV, cv::Scalar(interval.minH+3, iLowS, iLowV),
			cv::Scalar(interval.maxH-3, iHighS, iHighV), imgTmp);
//
	cv::erode(imgTmp, imgTmp,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	cv::dilate(imgTmp, imgTmp,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	cv::dilate(imgTmp, imgTmp,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	cv::erode(imgTmp, imgTmp,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

	return imgTmp;
}

void gui::getFrame()
{
	if (fps_cnt == 0) {
		time(&second_start);
	}
//	camera.set(CV_CAP_PROP_EXPOSURE,exp/100);
//
//	std::cout << exp/100 << "\n";

	bool bSuccess = camera.read(imgIn);
//	flip(imgIn, imgIn, 1);
	if (!bSuccess) {
		camera.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
		camera.read(imgIn);
//		std::cerr << "Cannot read a frame from video stream" << "\n";
//		exit(-1);
	}
	cv::cvtColor(imgIn, imgHSV, cv::COLOR_BGR2HSV);
}

void gui::createWindows()
{
	cv::namedWindow("Main", CV_WINDOW_AUTOSIZE | CV_GUI_EXPANDED);
//	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);

	cv::setMouseCallback("Main", mouseCB, this);

	cv::createTrackbar("H-", "Main", &iLowH, 255, NULL);
	cv::createTrackbar("H+", "Main", &iHighH, 255, NULL);

//	cv::createTrackbar("S-", "Main", &iLowS,  255, NULL);
//	cv::createTrackbar("S+", "Main", &iHighS, 255, NULL);
//
//	cv::createTrackbar("V-", "Main", &iLowV,  255, NULL);
//	cv::createTrackbar("V+", "Main", &iHighV, 255, NULL);
}

void gui::resetBorders()
{
	iLowH = 50;
	iHighH = 128;

	iLowS = 180;
	iHighS = 255;

	iLowV = 180;
	iHighV = 255;
	setTrackBars();
}

void gui::setTrackBars()
{
	cv::setTrackbarPos("H-", "Main", iLowH); //Hue (0 - 179)
	cv::setTrackbarPos("H+", "Main", iHighH);

//	cv::setTrackbarPos("S-", "Main", iLowS); //Saturation (0 - 255)
//	cv::setTrackbarPos("S+", "Main", iHighS);
//
//	cv::setTrackbarPos("V-", "Main", iLowV); //Value (0 - 255)
//	cv::setTrackbarPos("V+", "Main", iHighV);

}

void gui::mouseCB(int event, int x, int y, int flags, void* userdata)
{
	gui* g = reinterpret_cast<gui*>(userdata);
	g->mouseCB(event, x, y, flags);
}

void gui::mouseCB(int event, int x, int y, int flags)
{
	imgOver = cv::Mat::zeros(cv::Size(resIn.x, resIn.y), CV_8UC3);

	if (event == cv::EVENT_MOUSEMOVE && x <= resIn.x * qGlob
			&& y <= resIn.y * qGlob) {
		cv::Mat imgTmp;
		cv::blur(imgIn, imgTmp, cv::Size(2, 2));

		cv::Vec3b p = imgTmp.at<cv::Vec3b>((int) y / qGlob, (int) x / qGlob);
		cv::circle(imgOver, cv::Point((int) x / qGlob, (int) y / qGlob), 15,
				(cv::Scalar) p, -1);

		std::ostringstream log_str;
		log_str << "RGB: " << (int) p[0] << " " << (int) p[1] << " "
				<< (int) p[2];
		log(log_str.str(), "RGB");

	} else if (event == cv::EVENT_LBUTTONDOWN
			|| (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_LBUTTON)) {

		if (x > resIn.x * qGlob || y > resIn.y * qGlob)
			return;

		// Pick color
//		cv::Mat imgTmp;
//		cv::blur(imgIn, imgTmp, cv::Size(3, 3));
//		cv::cvtColor(imgTmp, imgTmp, cv::COLOR_BGR2HSV);
		cv::Vec3b p = imgHSV.at<cv::Vec3b>((int) y / qGlob, (int) x / qGlob);

		std::ostringstream log_str;
		log_str << "HSV: " << (int) p[0] << " " << (int) p[1] << " "
				<< (int) p[2];
		log(log_str.str(), "HSV");

		int* funnel[] = { &iLowH, &iHighH }; //, &iLowS, &iHighS, &iLowV, &iHighV };
		for (int i = 0; i < 1; ++i) {
			*funnel[i * 2] = std::max(0, p.val[i] - 10);
			*funnel[i * 2 + 1] = std::min(255, p.val[i] + 10);
		}

		setTrackBars();
	}
}

void gui::log(std::string str)
{
	logs.insert(std::make_pair(" ", str));
}

void gui::log(std::string str, std::string key)
{
	logs[key] = str;
}

void gui::printLog()
{

	int rn = 0;
	for (auto row : logs) {
		cv::putText(imgOut, cv::String(row.second.c_str()),
				cv::Point(25, 25 + 25 * rn++), fontFace, 1.5,
				cv::Scalar(100, 255, 100), 2, 8);
//		if (row.first == "")
	}
	logs.erase(" ");

}
