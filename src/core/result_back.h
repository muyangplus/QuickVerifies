#pragma once

const int waittingTime = 3;		// seconds

static int lastRunningTime[10];

struct results {
	std::string message;
	int time;
};

static std::queue<results> resultList;

class ResultBack {
private:
	static bool outCheckByTime(int waittingTime, int pos);
	static bool outCheckByResult(std::string message, int showTime);
public:
	static void matchedFace(std::string name);
	static void too_far();
	static void no_mask();
	static void show_your_QRcode();
	static void green_QRcode();
	static void yellow_QRcode();
	static void red_QRcode();
	static void error_QRcode();
};


