#ifndef XZJ_GETIMAGE_H
#define XZJ_GETIMAGE_H

// C++
#include <iostream>

// OpenCV
#include <opencv.hpp>

using namespace cv;
using namespace std;

VideoCapture capture;
Mat capt, image;

/*
 * BrennerÌÝ¶È·½·¨
 *
 * Inputs:
 * @param image:
 * Return: double
 */
double brenner(cv::Mat& image){
	//assert(image.empty());
	Mat gray_img;
	if (image.channels() == 3) {
		cvtColor(image, gray_img, COLOR_BGR2GRAY);
	}
	double result = .0f;
	for (int i = 0; i < gray_img.rows; ++i) {
		uchar* data = gray_img.ptr<uchar>(i);
		for (int j = 0; j < gray_img.cols - 2; ++j) {
			result += pow(data[j + 2] - data[j], 2);
		}
	}
	return result / gray_img.total();
}



#endif // !XZJ_GETIMAGE_H
