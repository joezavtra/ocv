/*
 * gui.hpp
 *
 *  Created on: 11 нояб. 2015 г.
 *      Author: zelenov
 */

#ifndef SRC_GUI_HPP_
#define SRC_GUI_HPP_

class gui {
public:
	gui(int device);
	virtual ~gui();

	void
	start();
	static void
	mouseCB(int event, int x, int y, int flags, void* userdata);
	void
	mouseCB(int event, int x, int y, int flags);
//	std:vector<string> logs;

private:
	void
	show();
	void
	tresholdFrame();
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
	int iLowH;
	int iHighH;

	int iLowS;
	int iHighS;

	int iLowV;
	int iHighV;

	int exp;

	cv::VideoCapture camera;

	cv::Mat imgIn;
	cv::Mat imgOver;
	cv::Mat imgTres;
	cv::Mat imgOut;

	struct {
		int x; int y;
	} resIn;

	static const double qThumb = 1;
	static const double qGlob = 0.5;
};

#endif /* SRC_GUI_HPP_ */
