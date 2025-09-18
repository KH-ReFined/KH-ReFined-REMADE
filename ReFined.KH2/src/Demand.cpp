#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include<conio.h>

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
#include "sound.h"

#include "Demand.h"
#include "SigScan.h"

using namespace std;

bool IS_RESETING = false;

// TODO: Config implementation.
void ReFined::Demand::TriggerReset()
{
	// Declare what buttons activate Soft-Reset.
	uint16_t _seekButtons = YS::HARDPAD::BUTTONS::L2 | YS::HARDPAD::BUTTONS::R2;

	// Fetch the input and whether a soft reset can happen.
	auto _fetchButtons = *YS::HARDPAD::Input;
	bool _canReset = *YS::AREA::IsInMap && !*YS::TITLE::IsTitle && !*YS::MENU::IsMenu;

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