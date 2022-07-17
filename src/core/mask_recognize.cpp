// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>
// Self
#include "src/core/mask_recognize.h"

std::vector<float> ratios = { 1.0, 0.62, 0.42 };
std::vector<int> scales = { 33, 17, 9, 5, 3 };
std::vector<float> anchor_base = { 0.04, 0.056 };
std::vector<std::vector<float>> anchors = MaskAlgo::generate_anchors(ratios, scales, anchor_base);
cv::dnn::Net PNet_ = cv::dnn::readNetFromCaffe("./models/face_mask_detection.prototxt", "./models/face_mask_detection.caffemodel");

bool increase(const FaceInfo& a, const FaceInfo& b) {
	return a.score > b.score;
}

std::vector<float> MaskAlgo::recognize(cv::Mat capt) {
	cv::Mat rgb_capt;
	cvtColor(capt, rgb_capt, cv::COLOR_BGR2RGB);
	cv::Mat inputBlob = cv::dnn::blobFromImage(rgb_capt, 1 / 255.0, cv::Size(260, 260), cv::Scalar(0, 0, 0), false);
	PNet_.setInput(inputBlob, "data");
	const std::vector<cv::String>  targets_node{ "loc_branch_concat","cls_branch_concat" };
	std::vector<cv::Mat> targets_blobs;
	PNet_.forward(targets_blobs, targets_node);
	cv::Mat y_bboxes = targets_blobs[0];
	cv::Mat y_score = targets_blobs[1];
	float* bboxes = (float*)y_bboxes.data;
	float* confidences = (float*)y_score.data;

	std::vector<cv::Rect2f> decode_rects = MaskAlgo::decode_bbox(anchors, bboxes);
	std::vector<int> classes;
	std::vector<float> scores;
	std::vector<int> keep_idxs = MaskAlgo::single_class_non_max_suppression(decode_rects, confidences, y_score.total(), classes, scores);
	
	std::vector<float> info;
	if (!keep_idxs.empty()) {
		int i = keep_idxs[0];
		cv::Rect2f& r = decode_rects[i];
		info.push_back(r.x * capt.cols);		//x
		info.push_back(r.y * capt.rows);		//y
		info.push_back(r.width * capt.cols);	//w
		info.push_back(r.height * capt.rows);	//h
		info.push_back(scores[i]);				//可能性
		info.push_back(classes[i]);				//是否佩戴口罩
	}
	return info;
}

std::vector<std::vector<float>> MaskAlgo::generate_anchors(const std::vector<float>& ratios, const std::vector<int>& scales, std::vector<float>& anchor_base)
{
	std::vector<std::vector<float>> anchors;
	for (int idx = 0; idx < scales.size(); idx++) {
		std::vector<float> bbox_coords;
		int s = scales[idx];
		std::vector<float> cxys;
		std::vector<float> center_tiled;
		for (int i = 0; i < s; i++) {
			float x = (0.5 + i) / s;
			cxys.push_back(x);
		}

		for (int i = 0; i < s; i++) {
			float x = (0.5 + i) / s;
			for (int j = 0; j < s; j++) {
				for (int k = 0; k < 8; k++) {
					center_tiled.push_back(cxys[j]);
					center_tiled.push_back(x);
					//printf("%f %f ", cxys[j], x);
				}
				//printf("\n");
			}
			//printf("\n");
		}

		std::vector<float> anchor_width_heights;
		for (int i = 0; i < anchor_base.size(); i++) {
			float scale = anchor_base[i] * pow(2, idx);
			anchor_width_heights.push_back(-scale / 2.0);
			anchor_width_heights.push_back(-scale / 2.0);
			anchor_width_heights.push_back(scale / 2.0);
			anchor_width_heights.push_back(scale / 2.0);
			//printf("%f %f %f %f\n", -scale / 2.0, -scale / 2.0, scale / 2.0, scale / 2.0);
		}

		for (int i = 0; i < anchor_base.size(); i++) {
			float s1 = anchor_base[0] * pow(2, idx);
			float ratio = ratios[i + 1];
			float w = s1 * sqrt(ratio);
			float h = s1 / sqrt(ratio);
			anchor_width_heights.push_back(-w / 2.0);
			anchor_width_heights.push_back(-h / 2.0);
			anchor_width_heights.push_back(w / 2.0);
			anchor_width_heights.push_back(h / 2.0);
			//printf("s1:%f, ratio:%f w:%f h:%f\n", s1, ratio, w, h);
			//printf("%f %f %f %f\n", -w / 2.0, -h / 2.0, w / 2.0, h / 2.0);
		}

		int index = 0;
		//printf("\n");
		for (float& a : center_tiled) {
			float c = a + anchor_width_heights[(index++) % anchor_width_heights.size()];
			bbox_coords.push_back(c);
			//printf("%f ", c);
		}

		//printf("bbox_coords.size():%d\n", bbox_coords.size());
		int anchors_size = bbox_coords.size() / 4;
		for (int i = 0; i < anchors_size; i++) {
			std::vector<float> f;
			for (int j = 0; j < 4; j++) {
				f.push_back(bbox_coords[i * 4 + j]);
			}
			anchors.push_back(f);
		}
	}

	return anchors;
}

std::vector<cv::Rect2f> MaskAlgo::decode_bbox(std::vector<std::vector<float>>& anchors, float* raw)
{
	std::vector<cv::Rect2f> rects;
	float v[4] = { 0.1, 0.1, 0.2, 0.2 };

	int i = 0;
	for (std::vector<float>& k : anchors) {
		float acx = (k[0] + k[2]) / 2;
		float acy = (k[1] + k[3]) / 2;
		float cw = (k[2] - k[0]);
		float ch = (k[3] - k[1]);

		float r0 = raw[i++] * v[i % 4];
		float r1 = raw[i++] * v[i % 4];
		float r2 = raw[i++] * v[i % 4];
		float r3 = raw[i++] * v[i % 4];

		float centet_x = r0 * cw + acx;
		float centet_y = r1 * ch + acy;

		float w = exp(r2) * cw;
		float h = exp(r3) * ch;
		float x = centet_x - w / 2;
		float y = centet_y - h / 2;
		rects.push_back(cv::Rect2f(x, y, w, h));
	}

	return rects;
}

std::vector<int> MaskAlgo::do_nms(std::vector<FaceInfo>& bboxes, float thresh, char methodType) {
	std::vector<int> bboxes_nms;
	if (bboxes.size() == 0) {
		return bboxes_nms;
	}
	std::sort(bboxes.begin(), bboxes.end(), increase);

	int32_t select_idx = 0;
	int32_t num_bbox = static_cast<int32_t>(bboxes.size());
	std::vector<int32_t> mask_merged(num_bbox, 0);
	bool all_merged = false;

	while (!all_merged) {
		while (select_idx < num_bbox && mask_merged[select_idx] == 1)
			select_idx++;
		if (select_idx == num_bbox) {
			all_merged = true;
			continue;
		}

		bboxes_nms.push_back(bboxes[select_idx].id);
		mask_merged[select_idx] = 1;

		cv::Rect2f& select_bbox = bboxes[select_idx].rect;
		float area1 = (select_bbox.width + 1) * (select_bbox.height + 1);

		select_idx++;
#pragma omp parallel for num_threads(8)
		for (int32_t i = select_idx; i < num_bbox; i++) {
			if (mask_merged[i] == 1)
				continue;

			cv::Rect2f& bbox_i = bboxes[i].rect;
			float x = std::max<float>(select_bbox.x, bbox_i.x);
			float y = std::max<float>(select_bbox.y, bbox_i.y);
			float w = std::min<float>(select_bbox.width + select_bbox.x, bbox_i.x + bbox_i.width) - x + 1;
			float h = std::min<float>(select_bbox.height + select_bbox.y, bbox_i.y + bbox_i.height) - y + 1;
			if (w <= 0 || h <= 0)
				continue;

			float area2 = (bbox_i.width + 1) * (bbox_i.height + 1);
			float area_intersect = w * h;

			switch (methodType) {
			case 'u':
				if (area_intersect / (area1 + area2 - area_intersect) > thresh)
					mask_merged[i] = 1;
				break;
			case 'm':
				if (area_intersect / std::min(area1, area2) > thresh)
					mask_merged[i] = 1;
				break;
			default:
				break;
			}
		}
	}
	return bboxes_nms;
}

std::vector<int> MaskAlgo::single_class_non_max_suppression(std::vector<cv::Rect2f>& rects, float* confidences, int c_len, std::vector<int>& classes, std::vector <float>& bbox_max_scores)
{
	std::vector<int> keep_idxs;

	float conf_thresh = 0.2;
	float iou_thresh = 0.5;
	int keep_top_k = -1;
	if (rects.size() <= 0) {
		return keep_idxs;
	}

	for (int i = 0; i < c_len; i += 2) {
		float max = confidences[i];
		int classess = 0;
		if (max < confidences[i + 1]) {
			max = confidences[i + 1];
			classess = 1;
		}
		classes.push_back(classess);
		bbox_max_scores.push_back(max);
	}

	std::vector <FaceInfo>infos;
	for (int i = 0; i < bbox_max_scores.size(); i++) {
		if (bbox_max_scores[i] > conf_thresh) {
			FaceInfo info;
			info.rect = rects[i];
			info.score = bbox_max_scores[i];
			info.id = i;
			infos.push_back(info);
		}
	}

	keep_idxs = do_nms(infos, 0.7, 'u');
	return keep_idxs;
}

