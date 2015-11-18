/*
 * gui.cpp
 *
 *  Created on: 11 нояб. 2015 г.
 *      Author: zelenov
 */

#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow
#include <iostream>
#include <algorithm>    // std::max
#include <vector>    // std::max
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"
#include "gui.hpp"

int fontFace = cv::FONT_HERSHEY_PLAIN;
double fontScale = 2;
int thickness = 1;

time_t second_start, second_end;
int fps_cnt = 0;
double sec;
double fps;
char fps_str[100] = "";
char log_str[100] = "";

gui::gui(int device)
{
//	while ( device < 100 ) {
	camera.open(device);

	if (!camera.isOpened())  // if not success, exit program
	{
		std::cerr << "Cannot open the web cam" << device << "\n";
		exit(-1);
		//		return -1;
	}
//	}
	for (int i = -100; i < 100; i++) {
		std::cout << "camera_get(" << i << "): " << camera.get(i) << "\n";
	}
//	exp = camera.get(CV_CAP_PROP_EXPOSURE);
//	std::cout << "exp: " << exp << "\n";
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

void gui::start()
{
	while (true) {
		getFrame();
		tresholdFrame();
		processFrame();

		show();
	}
}

void gui::show()
{
	cv::Mat imgTmp = imgIn;
//	imgTmp = imgOver + imgTmp;
	bitwise_or(imgOver, imgTmp, imgTmp);
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
	cv::Mat imgHSV, imgTmp = imgIn;

//	bilateralFilter(imgHSV, imgHSV, 5, 80, 80);
//	cv::blur(imgIn, imgTmp, cv::Size(3, 3));
	cv::cvtColor(imgTmp, imgHSV, cv::COLOR_BGR2HSV);
	cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV),
			cv::Scalar(iHighH, iHighS, iHighV), imgTres);

	cv::erode(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(imgTres, imgTres,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
//	cv::dilate(imgTres, imgTres,
//			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
//	cv::erode(imgTres, imgTres,
//			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
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
		std::cerr << "Cannot read a frame from video stream" << "\n";
		exit(-1);
	}

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

	iLowS = 100;
	iHighS = 255;

	iLowV = 100;
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

		sprintf(log_str, "RGB: %d %d %d", p[0], p[1], p[2]);
		log(log_str, "RGB");

	} else if (event == cv::EVENT_LBUTTONDOWN
			|| (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_LBUTTON)) {

		if (x > resIn.x * qGlob || y > resIn.y * qGlob)
			return;

		cv::Mat imgTmp;
		cv::blur(imgIn, imgTmp, cv::Size(3, 3));
//		bilateralFilter(imgIn, imgTmp, 5, 80, 80);

		cv::cvtColor(imgTmp, imgTmp, cv::COLOR_BGR2HSV);
		cv::Vec3b p = imgTmp.at<cv::Vec3b>((int) y / qGlob, (int) x / qGlob);

		sprintf(log_str, "HSV: %d %d %d", p[0], p[1], p[2]);
		log(log_str, "HSV");

		int* funnel[] = { &iLowH, &iHighH, &iLowS, &iHighS, &iLowV, &iHighV };
		for (int i = 0; i < 3; ++i) {
			*funnel[i * 2] = std::max(0, p[i] - 30);
			*funnel[i * 2 + 1] = std::min(255, p[i] + 30);
		}

		setTrackBars();
	}
}

void gui::log(char* str)
{
	logs.insert( std::make_pair(" ", str) );
}

void gui::log(char* str, std::string key)
{
	logs[key] = str;
}

void gui::printLog()
{
	int rn = 0;
	for (auto row: logs) {
		cv::putText(imgOut, cv::String(row.second), cv::Point(25, 25+25*rn++), fontFace,
				fontScale, cv::Scalar(100, 255, 100), thickness, 8);
//		if (row.first == "")
	}
	logs.erase(" ");

}
