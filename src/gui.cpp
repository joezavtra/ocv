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
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"
#include "gui.hpp"

int fontFace = cv::FONT_HERSHEY_PLAIN;
double fontScale = 1;
int thickness = 1;

time_t start, end;
int fps_cnt = 0;
double sec;
double fps;
char fps_str[100] = "";
char log_str[100] = "";

gui::gui(int device)
{
	camera.open(device);

	if (!camera.isOpened())  // if not success, exit program
	{
		std::cerr << "Cannot open the web cam\n";
		exit(-1);
		//		return -1;
	}

	resIn.x = camera.get(CV_CAP_PROP_FRAME_WIDTH);
	resIn.y = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

	imgOver = cv::Mat::zeros(cv::Size(resIn.x, resIn.y), CV_8UC3);
	imgOut  = cv::Mat::zeros(cv::Size((int)resIn.x*(1+qThumb), (int)resIn.y*(1+qThumb)), CV_8UC3);

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
//		if (fps_cnt == 0) {
//			time(&start);
//		}

		getFrame();
		tresholdFrame();
//		processFrame();

//		time(&end);
//		fps_cnt++;
//		sec = difftime(end, start);
//		fps = fps_cnt / sec;
//		if (sec >= .5) {
//			sprintf(fps_str, "FPS: %.2f [%dX%d]", fps, imgT.cols, imgT.rows);
//			fps_cnt = 0;
//		}
//		// overflow protection
//		if (fps_cnt == (INT_MAX - 1000))
//			fps_cnt = 0;
		show();
	}
}

void gui::show()
{
	camera.set(CV_CAP_PROP_AUTO_EXPOSURE, -1);
	std::cout << "CV_CAP_PROP_MODE" << camera.get(CV_CAP_PROP_MODE) << "\n";
	std::cout << "CV_CAP_PROP_BRIGHTNESS" << camera.get(CV_CAP_PROP_BRIGHTNESS) << "\n";
	std::cout << "CV_CAP_PROP_CONTRAST" << camera.get(CV_CAP_PROP_CONTRAST) << "\n";
	std::cout << "CV_CAP_PROP_EXPOSURE" << camera.get(CV_CAP_PROP_EXPOSURE) << "\n\n";


	cv::Mat imgTmp = imgIn;
//	bilateralFilter ( imgIn, imgTmp, 4, 40, 40 );
	imgTmp = imgOver+imgTmp;
	imgTmp.copyTo(imgOut(cv::Rect(0, 0, imgIn.cols, imgIn.rows)));

	resize(imgTres, imgTmp, cv::Size(), qThumb, qThumb);
	cv::cvtColor(imgTmp, imgTmp, cv::COLOR_GRAY2BGR);
	imgTmp.copyTo(
			imgOut(
					cv::Rect(imgOut.cols - imgTmp.cols,
							imgOut.rows - imgTmp.rows, imgTmp.cols,
							imgTmp.rows)));

	resize(imgOut, imgTmp, cv::Size(), qGlob, qGlob);
	imshow("Main", imgTmp);

	cv::setMouseCallback("Main", mouseCB, this);

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
		cv::blur(imgIn, imgTmp, cv::Size(2, 2));

		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0]) {

//				Moments m = moments(contours[idx]);
//				int x = (int) (m.m10 / m.m00); //contours[idx][0].x
//				int y = (int) (m.m01 / m.m00); //contours[idx][0].y

//				cv::Vec3b p = imgTmp.at<cv::Vec3b>(y,x);

//				cout << idx << " > " <<  contours[idx] << "\n";
			cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
			cv::drawContours(imgIn, contours, idx, color, -1, 8, hierarchy);
		}
	}
}

void gui::tresholdFrame()
{
	cv::Mat imgHSV;

	cv::cvtColor(imgIn, imgHSV, cv::COLOR_BGR2HSV);
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

void gui::getFrame()
{
	bool bSuccess = camera.read(imgIn);
	flip(imgIn, imgIn, 1);
	if (!bSuccess) {
		std::cerr << "Cannot read a frame from video stream" << "\n";
		exit(-1);
	}

}

void gui::createWindows()
{
//	cv::namedWindow("Main", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);

	cv::createTrackbar("LowH", "Control", &iLowH, 255);
	cv::createTrackbar("HighH", "Control", &iHighH, 255);
	cv::createTrackbar("LowS", "Control", &iLowS, 255);
	cv::createTrackbar("HighS", "Control", &iHighS, 255);
	cv::createTrackbar("LowV", "Control", &iLowV, 255);
	cv::createTrackbar("HighV", "Control", &iHighV, 255);
}

void gui::resetBorders()
{
	iLowH = 255;
	iHighH = 0;

	iLowS = 255;
	iHighS = 0;

	iLowV = 255;
	iHighV = 0;
	setTrackBars();
}

void gui::setTrackBars()
{
	cv::setTrackbarPos("LowH", "Control", iLowH); //Hue (0 - 179)
	cv::setTrackbarPos("HighH", "Control", iHighH);

	cv::setTrackbarPos("LowS", "Control", iLowS); //Saturation (0 - 255)
	cv::setTrackbarPos("HighS", "Control", iHighS);

	cv::setTrackbarPos("LowV", "Control", iLowV); //Value (0 - 255)
	cv::setTrackbarPos("HighV", "Control", iHighV);

}

void gui::mouseCB(int event, int x, int y, int flags, void* userdata)
{
	gui* g = reinterpret_cast<gui*>(userdata);
	g->mouseCB(event, x, y, flags);
}

void gui::mouseCB(int event, int x, int y, int flags)
{
	imgOver = cv::Mat::zeros(cv::Size(resIn.x, resIn.y), CV_8UC3);

	if (event == cv::EVENT_MOUSEMOVE && x<=resIn.x*qGlob && y<=resIn.y*qGlob) {
		cv::Mat imgTmp;
		cv::blur(imgIn, imgTmp, cv::Size(2, 2));

		cv::Vec3b p = imgTmp.at<cv::Vec3b>((int)y/qGlob, (int)x/qGlob);
		cv::circle(imgOver, cv::Point((int)x/qGlob, (int)y/qGlob), 15, (cv::Scalar) p, -1);
	}
	else if (event == cv::EVENT_LBUTTONDOWN
			|| (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_LBUTTON)) {
		camera.read(imgIn);

		cv::Mat imgTmp;
		cv::blur(imgIn, imgTmp, cv::Size(2, 2));

		cv::cvtColor(imgTmp, imgTmp, cv::COLOR_BGR2HSV);
		cv::Vec3b p = imgTmp.at<cv::Vec3b>((int)y/qGlob, (int)x/qGlob);

		iLowH = std::min(iLowH, int(p[0]));
		iHighH = std::max(iHighH, int(p[0]));

		iLowS = std::min(iLowS, int(p[1]));
		iHighS = std::max(iHighS, int(p[1]));

		iLowV = std::min(iLowV, int(p[2]));
		iHighV = std::max(iHighV, int(p[2]));

		setTrackBars();
	}
}
