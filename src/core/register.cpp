// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>
// Self
#include "src/core/register.h"
#include "src/core/result_back.h"
#include "src/core/face_recognize.h"

/*
 * 颜色判断
 * @prem 0 : Green
 * @prem 1 : Yellow
 * @prem 2 : Red
 * @prem 3 : ERROR
 * @author 徐照杰
 * @date 2022/5/26
 */
int Register::getColor(cv::Mat image) {
	int num[4] = { 0,0,0,0 };			//绿,黄,红,白
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {   //遍历图片的每一个像素点
			//对该像素是否为绿色进行判断
			//(image.at<cv::Vec3b>(i, j)[0] >= 200 && image.at<cv::Vec3b>(i, j)[1] >= 200 && image.at<cv::Vec3b>(i, j)[2] >= 0)
			if (image.at<cv::Vec3b>(i, j)[0] <= 100
				&& image.at<cv::Vec3b>(i, j)[1] >= 100 && image.at<cv::Vec3b>(i, j)[1] <= 200
				&& image.at<cv::Vec3b>(i, j)[2] <= 100) {
				num[0]++;
			}
			//对该像素是否为黄色进行判断
			if (image.at<cv::Vec3b>(i, j)[0] >= 80 && image.at<cv::Vec3b>(i, j)[0] <= 200
				&& image.at<cv::Vec3b>(i, j)[1] >= 180
				&& image.at<cv::Vec3b>(i, j)[2] >= 200) {
				num[1]++;
			}
			//对该像素是否为红色进行判断
			if (image.at<cv::Vec3b>(i, j)[0] <= 200
				&& image.at<cv::Vec3b>(i, j)[1] <= 200
				&& image.at<cv::Vec3b>(i, j)[2] >= 180) {
				num[2]++;
			}
			//对该像素是否为白色进行判断
			if (image.at<cv::Vec3b>(i, j)[0] >= 200
				&& image.at<cv::Vec3b>(i, j)[1] >= 200
				&& image.at<cv::Vec3b>(i, j)[2] >= 200) {
				num[3]++;
			}
		}
	}
	std::cout << "Green:" << num[0] << ",rate:" << (float)num[0] / (float)(image.rows * image.cols) << std::endl;
	std::cout << "Yellow:" << num[1] << ",rate:" << (float)num[1] / (float)(image.rows * image.cols) << std::endl;
	std::cout << "Red:" << num[2] << ",rate:" << (float)num[2] / (float)(image.rows * image.cols) << std::endl;
	std::cout << "Write:" << num[3] << ",rate:" << (float)num[3] / (float)(image.rows * image.cols) << std::endl;
	std::cout << "ImageSize:" << (image.rows * image.cols) << std::endl;
	if ((num[0] > num[1]) && (num[0] > num[2]) && (((float)num[0] / (float)(image.rows * image.cols)) > 0.3)) {
		return 0;
	}
	if ((num[1] > num[0]) && (num[1] > num[2]) && (((float)num[1] / (float)(image.rows * image.cols)) > 0.3)) {
		return 1;
	}
	if ((num[2] > num[0]) && (num[2] > num[1]) && (((float)num[2] / (float)(image.rows * image.cols)) > 0.3)) {
		return 2;
	}
	return 3;
}

/*
 * 二维码定位
 * @author 徐照杰
 * @date 2022/5/3
 */
cv::Mat Register::getQRcode(cv::Mat& img) {
	cv::QRCodeDetector QRdetecter;
	std::vector<cv::Point> list;
	cv::Mat QRcode;
	QRdetecter.detect(img, list);
	if (list.empty()) {
		ResultBack::show_your_QRcode();
	}
	else {
		QRcode = img(cv::Rect(list[1], list[3]));
		resize(QRcode, QRcode, { 200 , 200 });
	}
	for (int i = 0; i < list.size(); i++) {
		if (i == 3) {
			line(img, list[i], list[0], cv::Scalar(255, 0, 0), 1);
		}
		else {
			line(img, list[i], list[i + 1], cv::Scalar(255, 0, 0), 1);
		}
		std::cout << list[i] << std::endl;
	}
	return QRcode;
}

//int items = 210;
/*
 * 人脸注册（健康码注册）
 * @author 徐照杰
 * @date 2022/5/25
 */
int Register::registFaceByQRcode(cv::Mat face) {
	cv::VideoCapture QRcapture;
	cv::TickMeter tm_getQRcode;
	cv:: Mat QRcode, capt_qr;
	while (!QRcode.data) {
		tm_getQRcode.start();
		capt_qr.data = { 0 };
		while (capt_qr.empty()) {
			QRcapture >> capt_qr;
		}
		resize(capt_qr, capt_qr, { 640,480 });
		flip(capt_qr, capt_qr, 1);	// 镜像
		QRcode = Register::getQRcode(capt_qr);
		tm_getQRcode.stop();
		putText(capt_qr, cv::format("FPS : %.2f", (float)tm_getQRcode.getFPS()), cv::Point(0, 15), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
		imshow("capt", capt_qr);
	}
	if (QRcode.data) {
		imshow("QRcode", QRcode);
		switch (Register::getColor(QRcode)) {
		case 0:
			ResultBack::green_QRcode();
			break;
		case 1:
			ResultBack::yellow_QRcode();
			break;
		case 2:
			ResultBack::red_QRcode();
			break;
		case 3:
			ResultBack::error_QRcode();
			Register::registFaceByQRcode(face);
			break;
		}
		//imwrite("./data/temp/QRcode_" + std::to_string(items) + ".jpg", QRcode);
		//items++;
	}
	return 0;
}

/*
 * 人脸注册（手动）
 * @author 徐照杰
 * @date 2022/5/24
 */
int Register::registFaceByInput(cv::Mat face) {
	std::string face_name;
	std::cout << "请注册此脸：";
	std::cin >> face_name;
	imwrite("./data/registFace/" + face_name + ".jpg", face);
	FaceAlgo::registFace(face, face_name);
	std::cout << "registFace : " << face_name << std::endl;
	return 0;
}


