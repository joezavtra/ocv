#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow
#include <iostream>
#include <algorithm>    // std::max
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"

using namespace cv;
using namespace std;

//TODO: hsv по клику мыши, закраска всего дерьма
int iLowH = 170;
int iHighH = 179;

int iLowS = 150;
int iHighS = 255;

int iLowV = 60;
int iHighV = 255;
Mat imgO;

VideoCapture cap(0); //capture the video from webcam

void SetTrackBars()
{
	setTrackbarPos("LowH", "Control", iLowH); //Hue (0 - 179)
	setTrackbarPos("HighH", "Control", iHighH);

	setTrackbarPos("LowS", "Control", iLowS); //Saturation (0 - 255)
	setTrackbarPos("HighS", "Control", iHighS);

	setTrackbarPos("LowV", "Control", iLowV); //Value (0 - 255)
	setTrackbarPos("HighV", "Control", iHighV);

}

void ResetBorders()
{
	iLowH = 255;
	iHighH = 0;

	iLowS = 255;
	iHighS = 0;

	iLowV = 255;
	iHighV = 0;
	SetTrackBars();
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN
			|| (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON)) {
		cap.read(imgO);
		Mat imgTmp;
		cvtColor(imgO, imgTmp, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		blur(imgTmp, imgTmp, Size(3, 3));
		cv::Vec3b p = imgTmp.at<cv::Vec3b>(y, x); // read pixel (0,0) (make copy)

		cout << "Point: " << p << "\n";

		iLowH = std::min(iLowH, int(p[0]));
		iHighH = std::max(iHighH, int(p[0]));

		iLowS = std::min(iLowS, int(p[1]));
		iHighS = std::max(iHighS, int(p[1]));

		iLowV = std::min(iLowV, int(p[2]));
		iHighV = std::max(iHighV, int(p[2]));

		SetTrackBars();
	}
}

int main(int argc, char** argv)
{
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << "\n";
		return -1;
	}

	ResetBorders();
	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	createTrackbar("LowH", "Control", &iLowH, 255);
	createTrackbar("HighH", "Control", &iHighH, 255);
	createTrackbar("LowS", "Control", &iLowS, 255);
	createTrackbar("HighS", "Control", &iHighS, 255);
	createTrackbar("LowV", "Control", &iLowV, 255);
	createTrackbar("HighV", "Control", &iHighV, 255);

	Mat imgTmp;
	cap.read(imgTmp);
//	resize(imgTmp, imgTmp, Size(), 0.5, 0.5);

	int fontFace = FONT_HERSHEY_PLAIN;
	double fontScale = 1;
	int thickness = 1;

	time_t start, end;
	int fps_cnt = 0;
	double sec;
	double fps;
	char fps_str[100] = "";
	char log_str[100] = "";

//	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);
	while (true) {
		if (fps_cnt == 0) {
			time(&start);
		}

		bool bSuccess = cap.read(imgO);
		flip(imgO, imgO, 1);
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;
		Mat imgT;

		cvtColor(imgO, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV),
				Scalar(iHighH, iHighS, iHighV), imgT); //Threshold the image

		//morphological opening (removes small objects from the foreground)
		erode(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		//morphological closing (removes small holes from the foreground)
		dilate(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(imgT, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

		if (hierarchy.size() > 0) {
			Mat imgTmp;
			blur(imgO, imgTmp, Size(2, 2));

			int idx = 0;
			for (; idx >= 0; idx = hierarchy[idx][0]) {

//				Moments m = moments(contours[idx]);
//				int x = (int) (m.m10 / m.m00); //contours[idx][0].x
//				int y = (int) (m.m01 / m.m00); //contours[idx][0].y

//				cv::Vec3b p = imgTmp.at<cv::Vec3b>(y,x);

//				cout << idx << " > " <<  contours[idx] << "\n";
				Scalar color(rand() & 255, rand() & 255, rand() & 255);
				drawContours(imgO, contours, idx, color, -1, 8,
						hierarchy);
			}
		}

		time(&end);
		fps_cnt++;
		sec = difftime(end, start);
		fps = fps_cnt / sec;
		if (sec >= .5) {
			sprintf(fps_str, "FPS: %.2f [%dX%d]", fps, imgT.cols, imgT.rows);
			fps_cnt = 0;
		}
		// overflow protection
		if (fps_cnt == (INT_MAX - 1000))
			fps_cnt = 0;

//		resize(imgO, imgO, Size(), 1.2, 1.2);
		resize(imgT, imgT, Size(), 0.2, 0.2);

		cvtColor(imgT, imgT, COLOR_GRAY2BGR); //Convert the captured frame from BGR to HSV

		imgT.copyTo(
				imgO(
						Rect(imgO.cols - imgT.cols, imgO.rows - imgT.rows,
								imgT.cols, imgT.rows)));

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

//		imgOriginal = imgOriginal + imgLines;
		imshow("Original", imgO); //show the original image

		//set the callback function for any mouse event
		setMouseCallback("Original", CallBackFunc, NULL);

		int key = waitKey(1);
		switch (key) {
			case 27: {
				cout << "esc key is pressed by user" << endl;
				return 0;
			}
			case 32:
				ResetBorders();
				break;
		}
	}

	return 0;
}
