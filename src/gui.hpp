/*
 * gui.hpp
 *
 *  Created on: 11 нояб. 2015 г.
 *      Author: zelenov
 */

#ifndef SRC_GUI_HPP_
#define SRC_GUI_HPP_

#include <string>
#include <map>

#include <time.h>

class gui {
public:
	gui(int device);
	virtual ~gui();

	int iLowH;
	int iHighH;

	int iLowS;
	int iHighS;

	int iLowV;
	int iHighV;

	void
	loop();
	static void
	mouseCB(int event, int x, int y, int flags, void* userdata);
	void
	mouseCB(int event, int x, int y, int flags);

//	static void
//	trackbarsCB(int, void*, void* userdata);
//	void
//	trackbarsCB(int event, int x, int y, int flags);
	typedef struct {
		int minH; int maxH;
	} hueInterval;

private:

	void
	log(std::string str);
	void
	log(std::string str, std::string key);
	void
	printLog();
	void
	show();
	void
	tresholdFrame();
	cv::Mat
	tresholdFrameH(hueInterval &interval);
	void
	processFrame();
	void
	getFrame();
	void
	createWindows();
	void
	setTrackBars();
	void
	resetBorders();

private:
	std::map<std::string, std::string> logs;

	cv::VideoCapture camera;

	cv::Mat imgIn;
	cv::Mat imgHSV;
	cv::Mat imgTres;

	cv::Mat imgOver;
	cv::Mat imgOut;

	struct {
		int x; int y;
	} resIn;

	const double qThumb = 1;
	const double qGlob = .8;

	time_t second_start, second_end;
	int fps_cnt = 0;
	double sec;
	double fps;
	char fps_str[100] = "";

//	const int H_STEP = 32;

};

#endif /* SRC_GUI_HPP_ */
