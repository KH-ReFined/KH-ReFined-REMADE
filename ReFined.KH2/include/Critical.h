#pragma once

#include <map>
#include <cstdio>
#include <chrono>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <algorithm>

#include "pax.h"
#include "area.h"
#include "file.h"
#include "menu.h"
#include "sora.h"
#include "title.h"
#include "magic.h"
#include "event.h"
#include "gauge.h"
#include "cmconfig.h"
#include "cache_buff.h"
#include "information.h"
#include "treasure_info.h"
#include "jumpeffect.h"
#include "member_table.h"
#include "continue_menu.h"

#include "SigScan.h"

namespace ReFined
{
	class Critical
	{
	public:
		static char SAVE_MODE;
		static bool CONTROLLER_MODE;

		static void RegisterMagic();
		static void RegisterMovement();
		static void ShowInformation();
		static void ProcessDeath();
		static void RetryBattles();
		static void HandleCrown();
		static void HandleConfiguration();
		static void HandleIntro();
	};
}