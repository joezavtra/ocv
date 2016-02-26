/*
 * monitor.hpp
 *
 *  Created on: 2 февр. 2016 г.
 *      Author: pavelzelenov
 */

#ifndef SRC_MONITOR_HPP_
#define SRC_MONITOR_HPP_

#include <map>
#include <vector>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"
//#include <boost/asio.hpp>
//#include <boost/bind.hpp>

#include "tracker.hpp"

class monitor {
public:
//	monitor() {};
	monitor(int device);
	monitor(std::string file);
	virtual ~monitor() {};

public:
	void
	log(std::string str);
	void
	log(std::string str, std::string key);
	void
	printLog();
	void
	createWindows();
	void
	getFrame();
	void
	find();
	void
	track();
	void
	show();

	int
	players_count() { return trackers.size(); };

	void
	send();

private:
	void init();

	std::vector<tracker> trackers;

	std::map<std::string, std::string> logs;

	cv::VideoCapture source;

	cv::Mat imgIn;
	cv::Mat imgOver;
	cv::Mat imgOut;

	struct {
		int x; int y;
	} resIn;

	const double qThumb = 1;
	const double qGlob = .5;

	time_t second_start, second_end;

	int    fps_cnt = 0;
	double sec;
	double fps;
	char   fps_str[100] = "";

	int    objcnt = 0;
};

#endif /* SRC_MONITOR_HPP_ */
