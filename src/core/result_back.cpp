// C++
#include <iostream>
// Self
#include "src/core/result_back.h"

void ResultBack::too_far() {
	std::cout << "ÄúÀëÉèÏëÍ·Ì«Ô¶ÁË£¬Çë¿¿½üÉãÏñÍ·!\n";
}

void ResultBack::no_mask() {
	std::cout << "ÒßÇé·À¿ØÆÚ¼ä£¬Åå´÷¿ÚÕÖ!\n";
}

void ResultBack::show_your_QRcode() {
	std::cout << "ÇëÄú³öÊ¾ÄúµÄ½¡¿µÂë£¡\n";
}

void ResultBack::green_QRcode() {
	std::cout << "½¡¿µÂëÎªÂÌÉ«£¡\n";
}

void ResultBack::yellow_QRcode() {
	std::cout << "½¡¿µÂëÎª»ÆÉ«£¡\n";
}

void ResultBack::red_QRcode() {
	std::cout << "½¡¿µÂëÎªºìÉ«£¡\n";
}

void ResultBack::error_QRcode() {
	std::cout << "´íÎóµÄ½¡¿µÂëÑÕÉ«£¡\n";
}
