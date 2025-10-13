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
#include <algorithm>

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
#include "member_table.h"

#include "Demand.h"
#include "SigScan.h"

using namespace std;

bool IS_RESETING = false;
bool ENCOUNTER_ACTIVE = false;

bool SUBMIT_SHORTCUTS = false;
bool REVERTED_SHORTNAME = false;
bool DEBOUNCE_SHORTCUT = false;

uint8_t CURRENT_SHORTCUT_SET = 0x80;
vector<vector<char>> DEFAULT_SHORTCUT_NAMES;


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

void ReFined::Demand::ShortcutSets()
{
	if (DEFAULT_SHORTCUT_NAMES.size() == 0x00)
	{
		auto _shortString = YS::MESSAGE::DecodeKHSCII(YS::MESSAGE::GetData(0x051F));

		replace(_shortString.begin(), _shortString.end(), 'X', 'A');
		DEFAULT_SHORTCUT_NAMES.push_back(YS::MESSAGE::EncodeKHSCII(_shortString));

		replace(_shortString.begin(), _shortString.end(), 'A', 'B');
		DEFAULT_SHORTCUT_NAMES.push_back(YS::MESSAGE::EncodeKHSCII(_shortString));

		replace(_shortString.begin(), _shortString.end(), 'B', 'C');
		DEFAULT_SHORTCUT_NAMES.push_back(YS::MESSAGE::EncodeKHSCII(_shortString));
	}

	if (!*YS::AREA::IsInMap && CURRENT_SHORTCUT_SET != *(YS::AREA::SaveData + 0xE600))
		CURRENT_SHORTCUT_SET = *(YS::AREA::SaveData + 0xE600);

	bool IS_CUSTOMIZE = *YS::MENU::IsMenu && *YS::MENU::MenuType == 0x08 && *YS::MENU::SubMenuType == 0x19 && CalculatePointer(YS::MENU::pint_jiminymenu, { 0x00 }) == 0x00;
	bool IS_SHORTEDIT = *YS::MENU::IsMenu && *YS::MENU::MenuType == 0x08 && (*YS::MENU::SubMenuType == 0x1A || *YS::MENU::SubMenuType == 0x1D || *YS::MENU::SubMenuType == 0x1E || *YS::MENU::SubMenuType == 0x1F) && CalculatePointer(YS::MENU::pint_jiminymenu, { 0x00 }) == 0x00;

	bool IS_ROXAS = (*reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable) == 0x5A || *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable) == 0x0323);

	auto _helpBitwise = *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x4270);

	if ((_helpBitwise & 0x0600) == 0x0600)
	{
		_helpBitwise += 0x0F00;
		memcpy(YS::AREA::SaveData + 0x4270, &_helpBitwise, 0x02);
	}

	if (!*YS::TITLE::IsTitle && !IS_ROXAS)
	{
		if (IS_SHORTEDIT && !SUBMIT_SHORTCUTS)
			SUBMIT_SHORTCUTS = true;

		else if (!IS_SHORTEDIT && SUBMIT_SHORTCUTS)
		{
			memcpy(YS::AREA::SaveData + 0xE700 + (0x08 * CURRENT_SHORTCUT_SET), YS::AREA::SaveData + 0x36F8, 0x08);
			SUBMIT_SHORTCUTS = false;
		}

		auto _currentTextPtr = YS::MESSAGE::GetData(0x051F);
		auto _currentTextSize = YS::MESSAGE::GetSize(_currentTextPtr);

		vector<char> _currentText(_currentTextSize + 0x01);
		memcpy(_currentText.data(), _currentTextPtr, _currentTextSize + 0x01);

		if (!IS_CUSTOMIZE && !IS_SHORTEDIT && !REVERTED_SHORTNAME)
		{
			auto _soraText = YS::MESSAGE::GetData(0x572E);
			auto _soraSize = YS::MESSAGE::GetSize(_soraText);

			memcpy(const_cast<char*>(_currentTextPtr), _soraText, _soraSize + 0x01);
			REVERTED_SHORTNAME = true;
		}

		else if ((IS_CUSTOMIZE || IS_SHORTEDIT) && (!equal(_currentText.begin(), _currentText.end(), DEFAULT_SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].begin()) || REVERTED_SHORTNAME))
		{
			memcpy(const_cast<char*>(_currentTextPtr), DEFAULT_SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].data(), DEFAULT_SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size());
			REVERTED_SHORTNAME = false;
		}

		bool IS_INPUT_MENU = (*YS::HARDPAD::Input & 0x0400) == 0x0400 || (*YS::HARDPAD::Input & 0x0800) == 0x0800;
		bool IS_INPUT_GAME = (*YS::HARDPAD::Input & 0x0010) == 0x0010 || (*YS::HARDPAD::Input & 0x0040) == 0x0040;

		auto _commandType = CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x00 });
		auto _subOptionSelect = CalculatePointer(YS::MENU::pint_suboptionselect, { 0x00 });

		if (_commandType != nullptr)
		{
			if (((IS_CUSTOMIZE || IS_SHORTEDIT) && !IS_INPUT_MENU) || (*_commandType == 0x05 && !IS_INPUT_GAME))
				DEBOUNCE_SHORTCUT = false;

			if (!DEBOUNCE_SHORTCUT)
			{
				if (*_commandType == 0x05)
				{
					auto _flowDirection = (*YS::HARDPAD::Input & 0x0010) == 0x0010 ? -1 : ((*YS::HARDPAD::Input & 0x0040) == 0x0040 ? 1 : 0);

					if (*(YS::AREA::SaveData + 0x3524) != 0x03 && *(YS::AREA::SaveData + 0x3524) != 0x06)
					{
						if (_flowDirection != 0x00)
						{
							YS::SOUND::PlaySFX(0x14);

							CURRENT_SHORTCUT_SET += _flowDirection;
							DEBOUNCE_SHORTCUT = true;
						}
					}

					else if (_flowDirection != 0x00)
					{
						YS::SOUND::PlaySFX(0x05);
						DEBOUNCE_SHORTCUT = true;
					}
				}

				else if (IS_CUSTOMIZE && _subOptionSelect != nullptr)
				{
					if (*_subOptionSelect == 0x00)
					{
						auto _flowDirection = (*YS::HARDPAD::Input & 0x0400) == 0x0400 ? -1 : ((*YS::HARDPAD::Input & 0x0800) == 0x0800 ? 1 : 0);

						if (_flowDirection != 0x00)
						{
							YS::SOUND::PlaySFX(0x02);

							CURRENT_SHORTCUT_SET += _flowDirection;
							DEBOUNCE_SHORTCUT = true;
						}
					}
				}
			}
		}
		
		if (CURRENT_SHORTCUT_SET == 0x80)
			CURRENT_SHORTCUT_SET = *(YS::AREA::SaveData + 0xE600);

		if (CURRENT_SHORTCUT_SET >= 0x81)
			CURRENT_SHORTCUT_SET = 0x02;

		if (CURRENT_SHORTCUT_SET >= 0x03)
			CURRENT_SHORTCUT_SET = 0x00;

		if (*YS::AREA::IsInMap && CURRENT_SHORTCUT_SET != *(YS::AREA::SaveData + 0xE600) && DEBOUNCE_SHORTCUT)
		{
			memcpy(YS::AREA::SaveData + 0x36F8, YS::AREA::SaveData + 0xE700 + (0x08 * CURRENT_SHORTCUT_SET), 0x08);
			*(YS::AREA::SaveData + 0xE600) = CURRENT_SHORTCUT_SET;

			if (IS_CUSTOMIZE)
				YS::MENU::UpdateListShortcut(0x00);
		}
	}
}