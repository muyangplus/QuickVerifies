#pragma once

// C++
#include <iostream>
// OpenCV
#include <opencv.hpp>

struct faceInfo {
    std::string name;
    cv::Mat detResult;
};

class FaceAlgo {
public:
    static void initFaceModels(std::string detect_model_path, std::string recog_model_path, std::string face_db_dir);
    static void detectFace(cv::Mat& frame, std::vector<std::shared_ptr<faceInfo>>& results);
    static void matchFace(cv::Mat& frame, std::vector<std::shared_ptr<faceInfo>>& results, bool l2 = false);
    static void registFace(cv::Mat& frame, std::string name);
};


