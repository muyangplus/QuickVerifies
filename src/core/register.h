#pragma once

// OpenCV
#include <opencv.hpp>

class Register {
public:
	static int registFaceByQRcode(cv::Mat face);
	static int registFaceByInput(cv::Mat face);
private:
	static int getColor(cv::Mat image);
	static cv::Mat getQRcode(cv::Mat& img);
};


