#pragma 

//OpenCV
#include <opencv.hpp>
// Self
#include "src/core/register.h"
#include "src/core/mask_recognize.h"
#include "src/core/face_recognize.h"
#include "src/core/result_back.h"

class Capture {
public:
	static int initCapture(int capture_id, bool debug);
	static int recognition(int capture_id, bool debug);
	static cv::Mat getCapt();
};


