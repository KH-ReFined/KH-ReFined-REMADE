#pragma once
#include "itempic.h"

namespace dk {

	class TREASURE_INFO {
	public:
		using openPrizeWindow_t = void(*)(const char* message);
		static openPrizeWindow_t openPrizeWindow;

		static void openBoxWindow(const char* message, uint16_t image);
	};
}