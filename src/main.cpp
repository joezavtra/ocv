#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow
#include <iostream>
#include <algorithm>    // std::max
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"

#include "gui.hpp"

using namespace cv;
using namespace std;

//TODO: hsv по клику мыши, закраска всего дерьма





int main(int argc, char** argv)
{
	gui gui(0);

	gui.start();
}
/*
		putText(imgO, String(fps_str), Point(20, 20), fontFace, fontScale,
				Scalar(50, 50, 50), thickness + 2, 8);
		putText(imgO, String(fps_str), Point(20, 20), fontFace, fontScale,
				Scalar(100, 255, 100), thickness, 8);

		sprintf(log_str, "HUE: %d - %d", iLowH, iHighH);
		putText(imgO, String(log_str), Point(20, 35), fontFace, fontScale,
				Scalar(100, 255, 100), thickness, 8);
		sprintf(log_str, "SAT: %d - %d", iLowS, iHighS);
		putText(imgO, String(log_str), Point(20, 50), fontFace, fontScale,
				Scalar(100, 255, 100), thickness, 8);
		sprintf(log_str, "VAL: %d - %d", iLowV, iHighV);
		putText(imgO, String(log_str), Point(20, 65), fontFace, fontScale,
				Scalar(100, 255, 100), thickness, 8);
*/
