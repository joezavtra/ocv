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
Mat imgOriginal;

void ResetBorders() {
	iLowH = 170;
	iHighH = 179;

	iLowS = 150;
	iHighS = 255;

	iLowV = 60;
	iHighV = 255;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
//	if (event == EVENT_LBUTTONDOWN) {
//		cout << "Left button of the mouse is clicked - position (" << x << ", "
//				<< y << ")" << endl;
//	} else if (event == EVENT_RBUTTONDOWN) {
//		cout << "Right button of the mouse is clicked - position (" << x << ", "
//				<< y << ")" << endl;
//	} else if (event == EVENT_MBUTTONDOWN) {
//		cout << "Middle button of the mouse is clicked - position (" << x
//				<< ", " << y << ")" << endl;
//	} else
	if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON) {
		Mat imgTmp;
		if (x <= 0 || y <= 0)
			return;

		blur(imgOriginal, imgTmp, Size(5, 5), Point(-1, -1));
		cv::Vec3b p = imgTmp.at<cv::Vec3b>(y, x); // read pixel (0,0) (make copy)

		iLowH = std::min(iLowH, int(p[0]));
		iHighH = std::max(iHighH, int(p[0]));

		iLowS = std::min(iLowS, int(p[1]));
		iHighS = std::max(iHighS, int(p[1]));

		iLowV = std::min(iLowV, int(p[2]));
		iHighV = std::max(iHighV, int(p[2]));
	}
}

int main(int argc, char** argv) {
	VideoCapture cap(0); //capture the video from webcam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 255);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);
	resize(imgTmp, imgTmp, Size(), 0.5, 0.5);

	int fontFace = FONT_HERSHEY_PLAIN;
	double fontScale = 1;
	int thickness = 1;

	time_t start, end;
	int fps_cnt = 0;
	double sec;
	double fps;
	char fps_str[100] = "";

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);

	while (true) {
		if (fps_cnt == 0) {
			time(&start);
		}

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
		resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV),
				Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
//
//		//morphological opening (removes small objects from the foreground)
//		erode(imgThresholded, imgThresholded,
//				getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholded, imgThresholded,
//				getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		//morphological closing (removes small holes from the foreground)
//		dilate(imgThresholded, imgThresholded,
//				getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(imgThresholded, imgThresholded,
//				getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
		if (dArea > 10000) {
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0) {
				medianBlur(imgLines, imgLines, 3);
				//Draw a red line from the previous point to the current point
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY),
						Scalar(0, 0, 255), 2);
			}

			iLastX = posX;
			iLastY = posY;
		}

		time(&end);
		fps_cnt++;
		sec = difftime(end, start);
		fps = fps_cnt / sec;
		if (sec >= .5) {
			sprintf(fps_str, "FPS: %.2f [%dX%d]", fps, imgThresholded.cols,
					imgThresholded.rows);
			fps_cnt = 0;
		}
		// overflow protection
		if (fps_cnt == (INT_MAX - 1000))
			fps_cnt = 0;

		// then put the text itself
		putText(imgThresholded, String(fps_str), Point(20, 20), fontFace,
				fontScale, Scalar::all(255), thickness, 8);

		imshow("Thresholded Image", imgThresholded); //show the thresholded image

		imgOriginal = imgOriginal + imgLines;
		blur(imgOriginal, imgOriginal, Size(5, 5));
		imshow("Original", imgOriginal); //show the original image

		//set the callback function for any mouse event
		setMouseCallback("Original", CallBackFunc, NULL);

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
			cout << "esc key is pressed by user" << endl;
			break;
		}
		if (waitKey(30) == 32) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
			ResetBorders();
		}
	}

	return 0;
}
