/*
 * monitor.cpp
 *
 *  Created on: 2 февр. 2016 г.
 *      Author: pavelzelenov
 */

#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"

#include "sender.cpp"
#include "tracker.hpp"
#include "monitor.hpp"

monitor::monitor(std::string file)
{
	source.open(file);
	if (!source.isOpened())  // if not success, exit program
	{
		std::cerr << "Cannot open the file '" << file << "'\n";
		exit(-1);
	}
	init();
}

monitor::monitor(int device)
{
	source.open(device);
	if (!source.isOpened())  // if not success, exit program
	{
		std::cerr << "Cannot open the web cam " << device << "\n";
		exit(-1);
	}
	init();
}

void
monitor::init()
{
    source.set(CV_CAP_PROP_FRAME_WIDTH,1920);//1920
    source.set(CV_CAP_PROP_FRAME_HEIGHT,1080);//1080
    source.set(CV_CAP_PROP_FPS, 60);
//    source.set(CV_CAP_PROP_SPEED, 1);
    source.set(CV_CAP_PROP_BRIGHTNESS, .1);
    source.set(CV_CAP_PROP_CONTRAST, .1);
    source.set(CV_CAP_PROP_SATURATION, .1);
    source.set(CV_CAP_PROP_EXPOSURE, .1);

	resIn.x = 1920;//source.get(CV_CAP_PROP_FRAME_WIDTH);
	resIn.y = 1080;//source.get(CV_CAP_PROP_FRAME_HEIGHT);

	imgOver = cv::Mat::zeros(cv::Size(resIn.x, resIn.y), CV_8UC3);
	imgOut = cv::Mat::zeros(cv::Size(resIn.x, resIn.y), CV_8UC3);

	std::cout << "Camera " << resIn.x << "x" << resIn.y << " opened\n";

//	createWindows();
//	resetBorders();
}

void
monitor::send()
{
	try
	{
		boost::asio::ip::address address = boost::asio::ip::address_v4::broadcast();
		boost::asio::io_service  io_service;
		sender s(io_service, address);
		for(auto t=trackers.begin(); t!=trackers.end(); t++)
			s.add(*t);
		s.send();
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

}

void
monitor::createWindows()
{
	cv::namedWindow("Main", CV_WINDOW_OPENGL); //CV_WINDOW_AUTOSIZE | CV_GUI_EXPANDED |
	cv::setOpenGlContext ( "Main" );
}

void
monitor::getFrame()
{
	if (fps_cnt == 0) {
		time(&second_start);
	}

	bool bSuccess = source.read(imgIn);
	if (!bSuccess) {
		source.read(imgIn);
	}
}

void monitor::find()
{
	trackers.clear();
	getFrame();

	cv::Mat imgHSV;
	cv::Mat imgTres;
	cv::cvtColor(imgIn, imgHSV, cv::COLOR_BGR2HSV);
	cv::inRange(imgHSV, cv::Scalar(0, 200, 150),
			cv::Scalar(255, 255, 255), imgTres);

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
	cv::findContours(imgTres, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);


	objcnt = 0;
	if (hierarchy.size() > 0) {
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0]) {
			double area = contourArea(contours[idx]);

			cv::Point2f c;
			float r;
			cv::minEnclosingCircle(contours[idx], c, r);
			cv::circle(imgOver,c,r*1.5,cv::Scalar(100,100,255),2,1);
			cv::putText(imgOver, cv::String(std::to_string(area)),
					cv::Point(c.x + r, c.y - r), cv::FONT_HERSHEY_PLAIN, 1.5,
					cv::Scalar(100, 100, 255), 2, 8);


			if (area < 150 ) continue;
			if (area > 1800 ) continue;


			cv::Rect bound = cv::boundingRect(contours[idx]);

			objcnt++;
			tracker t( imgIn, imgOver, bound );
			trackers.push_back(t);
		}
	}
	log("Objects: " + std::to_string(objcnt), "Objects");
	std::cout << "Objects: " << objcnt << "\n";

}

void monitor::track()
{
	int tracked = 0;
	for(auto t=trackers.begin(); t!=trackers.end(); t++)
		if(t->track()) tracked++;

	if(objcnt==0 || tracked < objcnt) find();
}

void monitor::show()
{
	cv::Mat imgTmp = imgIn;

	time(&second_end);
	fps_cnt++;
	sec = difftime(second_end, second_start);
	fps = fps_cnt / sec;
	if (sec >= 1) {
		sprintf(fps_str, "FPS: %.2f [%dX%d]", fps, resIn.x, resIn.y);
		fps_cnt = 0;
		std::cout << fps_str << "\n";
	}

	log(fps_str, "FPS");
	printLog();

	cv::bitwise_or(imgOver, imgTmp, imgTmp);
	cv::resize(imgTmp, imgOut, cv::Size(), qGlob, qGlob);
	cv::imshow("Main", imgOut);

	imgOver *= 0;
}

void monitor::log(std::string str)
{
	logs.insert(std::make_pair(" ", str));
}

void monitor::log(std::string str, std::string key)
{
	logs[key] = str;
}

void monitor::printLog()
{
	int rn = 0;
	for (auto row : logs) {
		cv::putText(imgOver, cv::String(row.second.c_str()),
				cv::Point(25, 25 + 25 * rn++), cv::FONT_HERSHEY_PLAIN, 1.5,
				cv::Scalar(100, 255, 100), 2, 8);
//		if (row.first == "")
	}
	logs.erase(" ");

}
