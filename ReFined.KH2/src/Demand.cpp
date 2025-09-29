#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <random>

#include "information.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "menu.h"
#include "message.h"
#include "event.h"
#include "hardpad.h"
#include "treasure_info.h"
#include "softreset.h"
#include "treasure_info.h"
#include "jumpeffect.h"
#include "command_draw.h"
#include "sound.h"

#include "Demand.h"
#include "SigScan.h"
#include <algorithm>

using namespace std;

bool IS_RESETING = false;
bool ENCOUNTER_ACTIVE = false;

// TODO: Config implementation.
void ReFined::Demand::TriggerReset()
{
	// Declare what buttons activate Soft-Reset.
	uint16_t _seekButtons = YS::HARDPAD::BUTTONS::L2 | YS::HARDPAD::BUTTONS::R2;

	// Fetch the input and whether a soft reset can happen.
	auto _fetchButtons = *YS::HARDPAD::Input;
	auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

	bool _canReset = _commandPointer != 0x00 && *YS::AREA::IsInMap && !*YS::TITLE::IsTitle && !*YS::MENU::IsMenu;


	// If the buttons are pushed, a reset can happen and it isn't happening:
	if (_fetchButtons == _seekButtons && _canReset && !IS_RESETING)
	{
		// Declare the reset is happening for timing purposes.
		IS_RESETING = true;

		// Initiate the fadeout for BGMs.
		YS::SOUND::BGMFadeOut(200, 0x00);
		YS::SOUND::BGMFadeOut(200, 0x01);

		// Initiate fade-to-black.
		dk::JUMPEFFECT::Out(0x01);
	}

	// If it's resetting, and the fade is complete:
	if (IS_RESETING && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x04)
	{
		// Set the reset byte and declare we are no longer resetting.
		*(dk::SOFTRESET::RESET) = 0x01;
		IS_RESETING = false;
	}
}

void ReFined::Demand::EncounterPlus()
{
	auto _roomInfo = CalculatePointer(YS::AREA::pint_enemyinfo, { 0x08 });

	vector<uint16_t> _abilityRead(0x60);
	memcpy(_abilityRead.data(), YS::AREA::SaveData + 0x2544, 0xC0);

	bool _moogleGood = *(YS::AREA::SaveData + 0x4C34) >= 0x04;

	bool _hasAbility = any_of(_abilityRead.begin(), _abilityRead.end(), [](int x) { return (x == 0x00F8); });
	bool _abilityActive = any_of(_abilityRead.begin(), _abilityRead.end(), [](int x) { return (x == 0x80F8); });

	if (!*YS::TITLE::IsTitle && *YS::AREA::IsInMap && _moogleGood && (!_hasAbility && !_abilityActive))
	{
		auto _indexZero = std::distance(_abilityRead.begin(), std::find(_abilityRead.begin(), _abilityRead.end(), 0x0000));
		*(YS::AREA::SaveData + 0x2544 + (_indexZero * 2)) = 0xF8;
	}

	if (!*YS::AREA::IsInMap && _abilityActive && !ENCOUNTER_ACTIVE && _roomInfo != nullptr)
	{
		char _fillZero[0x100];

		fill(_fillZero, _fillZero + 0x100, 0x00);
		memcpy(const_cast<char*>(_roomInfo), _fillZero, 0x100);

		auto _vendorOffset = YS::AREA::Current->World == 0x05 && YS::AREA::Current->Room == 0x08 ? 0x00 :
			(YS::AREA::Current->World == 0x08 && YS::AREA::Current->Room == 0x02 ? 0x01 :
				(YS::AREA::Current->World == 0x06 && YS::AREA::Current->Room == 0x07 ? 0x02 :
					(YS::AREA::Current->World == 0x07 && YS::AREA::Current->Room == 0x01 ? 0x03 :
						(YS::AREA::Current->World == 0x0E && YS::AREA::Current->Room == 0x06 ? 0x04 : 0xFF))));

		if (_vendorOffset != 0xFF && *(YS::AREA::IsVendor + _vendorOffset) != 0x02)
		{
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 100);

			auto _randomChance = dist6(rng);

			if (_randomChance >= 80)
				*(YS::AREA::IsVendor + _vendorOffset) = 0x02;
		}

		ENCOUNTER_ACTIVE = true;
	}

	else if (*YS::AREA::IsInMap && ENCOUNTER_ACTIVE)
		ENCOUNTER_ACTIVE = false;
}