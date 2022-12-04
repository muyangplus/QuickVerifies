// C++
#include <iostream>
#include <time.h>
#include <queue>
// Self
#include "src/core/result_back.h"
#include "src/core/chcp.h"

bool ResultBack::outCheckByTime(int waittingTime, int pos) {
	int now = time(0);
	if (now - waittingTime >= lastRunningTime[pos]) {
		lastRunningTime[pos] = now;
		return true;
	}
	else {
		return false;
	}
}

bool ResultBack::outCheckByResult(std::string message, int showTime) {
	int now = time(0);
	while (!resultList.empty()) {
		int messageTime = resultList.front().time;
		if (now - waittingTime >= messageTime) {
			resultList.pop();
		}
		else break;
	}
	
	bool flag = true;
	int loop = resultList.size();
	while(loop--) {
		if (resultList.front().message == message) flag = false;
		resultList.push({ resultList.front().message, resultList.front().time });
		resultList.pop();
	}

	if (flag) {
		resultList.push({ message,showTime });
		return true;
	}
	else return false;
}

void ResultBack::matchedFace(std::string name) {
	std::string print = "匹配到面部特征 : " + name;
	if(GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::too_far() {
	std::string print = "您离摄像头太远了，请靠近摄像头";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::no_mask() {
	std::string print = "疫情防控期间，佩戴口罩";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::show_your_QRcode() {
	std::string print = "请您出示您的健康码";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::green_QRcode() {
	std::string print = "健康码为绿色";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::yellow_QRcode() {
	std::string print = "健康码为黄色";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::red_QRcode() {
	std::string print = "健康码为红色";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}

void ResultBack::error_QRcode() {
	std::string print = "错误的健康码颜色";
	if (GBK) print = Chcp::GbkToUtf8(print.c_str());
	if (ResultBack::outCheckByResult(print, time(0))) std::cout << print << std::endl;
	return;
}
