// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>
// Self
#include "src/core/capture.h"

int main(int argc, char* argv[]) {
	FaceAlgo::initFaceModels("./models/yunet.onnx", "./models/face_recognizer_fast.onnx", "./data/registFace");

	int id = 0;
	while (true) {
		std::cout << "请输入您的摄像头设备序号：";
		std::cin >> id;
		if (Capture::initCapture(id, 0)) {
			break;
		}
		else {
			std::cout << "未找到此摄像头，请重试！" << std::endl;
		}
	}
	
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	std::cout << "Running on Windows..." << std::endl;
	Capture::recognition(id, 1, 0);
#else
	std::cout << "Running on Linux..." << std::endl;
	Capture::recognition(id, 0, 0);
#endif
	return 0;
}


