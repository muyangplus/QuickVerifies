// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>
// Self
#include "src/core/capture.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	FaceAlgo::initFaceModels("./models/yunet.onnx", "./models/face_recognizer_fast.onnx", "./data/registFace");

	int id = 0;
	while (true) {
		cout << "请输入您的摄像头设备序号：";
		cin >> id;
		if (Capture::initCapture(id, 0)) {
			break;
		}
		else {
			cout << "未找到此摄像头，请重试！" << endl;
		}
	}
	
	Capture::recognition(id, 0);

	return 0;
}


