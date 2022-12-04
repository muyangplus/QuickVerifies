// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>
// Self
#include "src/core/face_recognize.h"
#include "src/core/result_back.h"

std::map<std::string, cv::Mat> face_models;
cv::Ptr<cv::FaceDetectorYN> faceDetector = nullptr;
cv::Ptr<cv::FaceRecognizerSF> faceRecognizer = nullptr;

void FaceAlgo::initFaceModels(std::string detect_model_path, std::string recog_model_path, std::string face_db_dir) {
	faceDetector = cv::FaceDetectorYN::create(detect_model_path, "", cv::Size(300, 300), 0.9f, 0.3f, 500);
	faceRecognizer = cv::FaceRecognizerSF::create(recog_model_path, "");
	std::vector<std::string> fileNames;
	cv::glob(face_db_dir, fileNames);
	for (std::string file_path : fileNames) {
		cv::Mat image = cv::imread(file_path);
		int pos = static_cast<int>(file_path.find("\\"));
		std::string image_name = file_path.substr(pos + 1, file_path.length() - pos - 5);
		registFace(image, image_name);
		std::cout << "registFace : " << image_name << std::endl;
	}
}

void FaceAlgo::detectFace(cv::Mat& image, std::vector<std::shared_ptr<faceInfo>>& infoList) {
	faceDetector->setInputSize(image.size());

	// Inference
	cv::Mat faces;
	faceDetector->detect(image, faces);
	//tm.stop();
	// Draw results on the input image
	int thickness = 2;
	for (int i = 0; i < faces.rows; i++)
	{
		// Draw bounding box
		auto fi = std::shared_ptr<faceInfo>(new faceInfo());
		fi->name = "Unknown";
		faces.row(0).copyTo(fi->detResult);
		infoList.push_back(fi);
	}
}

void FaceAlgo::matchFace(cv::Mat& frame, std::vector<std::shared_ptr<faceInfo>>& infoList, bool l2) {
	double cosine_similar_thresh = 0.363;
	double l2norm_similar_thresh = 1.128;
	for (auto face : infoList) {
		cv::Mat aligned_face, feature;
		faceRecognizer->alignCrop(frame, face->detResult, aligned_face);
		faceRecognizer->feature(aligned_face, feature);
		double min_dist = 100.0;
		double max_cosine = 0.0;
		std::string matchedName = "Unknown";
		for (auto item : face_models) {
			//std::cout<<"face_models.item :" << item.first << std::endl;
			//std::cout<<"face_models.item :" << item.second << std::endl;
			double L2_score = faceRecognizer->match(feature, item.second, cv::FaceRecognizerSF::DisType::FR_NORM_L2);
			double cos_score = faceRecognizer->match(feature, item.second, cv::FaceRecognizerSF::DisType::FR_COSINE);
			if ((L2_score < min_dist) && (cos_score > max_cosine)) {
				min_dist = L2_score;
				max_cosine = cos_score;
				matchedName = item.first;
			}
		}
		if ((max_cosine > cosine_similar_thresh) && (min_dist < l2norm_similar_thresh)) {
			face->name.clear();
			face->name.append(matchedName);
			
		}
		ResultBack::matchedFace(matchedName);
	}
}

void FaceAlgo::registFace(cv::Mat& frame, std::string name) {
	faceDetector->setInputSize(frame.size());

	// Inference
	cv::Mat faces;
	faceDetector->detect(frame, faces);
	cv::Mat aligned_face, feature;
	faceRecognizer->alignCrop(frame, faces.row(0), aligned_face);
	faceRecognizer->feature(aligned_face, feature);
	face_models.insert(std::pair<std::string, cv::Mat>(name, feature.clone()));
}
