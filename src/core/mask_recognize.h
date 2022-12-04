#pragma once

// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>

typedef struct FaceInfo {
	cv::Rect2f rect;
	float score;
	int id;
} FaceInfo;

class MaskAlgo{
public:
	static std::vector<float> recognize(cv::Mat capt);
	static std::vector<int> single_class_non_max_suppression(std::vector<cv::Rect2f>&rects, float* confidences, int c_len, std::vector<int>&classes, std::vector <float>&bbox_max_scores);
	static std::vector<int> do_nms(std::vector<FaceInfo>& bboxes, float thresh, char methodType);
	static std::vector<cv::Rect2f> decode_bbox(std::vector<std::vector<float>>& anchors, float* raw);
	static std::vector<std::vector<float>> generate_anchors(const std::vector<float>& ratios, const std::vector<int>& scales, std::vector<float>& anchor_base);
};


