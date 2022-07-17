// C++
#include <iostream>
// Self
#include "src/core/result_back.h"

void ResultBack::too_far() {
	std::cout << "您离设想头太远了，请靠近摄像头!\n";
}

void ResultBack::no_mask() {
	std::cout << "疫情防控期间，佩戴口罩!\n";
}

void ResultBack::show_your_QRcode() {
	std::cout << "请您出示您的健康码！\n";
}

void ResultBack::green_QRcode() {
	std::cout << "健康码为绿色！\n";
}

void ResultBack::yellow_QRcode() {
	std::cout << "健康码为黄色！\n";
}

void ResultBack::red_QRcode() {
	std::cout << "健康码为红色！\n";
}

void ResultBack::error_QRcode() {
	std::cout << "错误的健康码颜色！\n";
}
