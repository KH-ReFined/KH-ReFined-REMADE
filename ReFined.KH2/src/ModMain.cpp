#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>

#include "information.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "menu.h"
#include "message.h"
#include "softreset.h"

#include "Demand.h"
#include "Continuous.h"
#include "Critical.h"
#include "SigScan.h"
#include "intro_menu.h"
#include "config_menu.h"
#include "continue_menu.h"
#include "memory_manager.h"

using namespace std;

extern "C"
{
	__declspec(dllexport) void OnInit(wchar_t* mod_path)
	{			
		auto _checkSteam = FindModule("steam_api64.dll");

		if (_checkSteam)
		{
			vector<uint8_t> _nopArraySave(0x05);
			uint8_t _jumpByte = 0xEB;

			char* _checkSaveFunc = SignatureScan<char*>("\x40\x55\x56\x57\x48\x81\xEC\xA0\x00\x00\x00\x48\xC7\x44\x24\x38\xFE\xFF\xFF\xFF\x48\x89\x9C\x24\xD0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x90\x00\x00\x00\x8B\xF1\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xED\x8D\x5D\x01\x48\x39\x2D\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x85\xC0\x74\x1D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxx????xx????xxxxxxxx????xx????xxxxxx????xxxxxxxxxx");
			
			memcpy(_checkSaveFunc + 0x189, _nopArraySave.data(), 0x05);
			memcpy(_checkSaveFunc + 0x196, _nopArraySave.data(), 0x02);

			memcpy(_checkSaveFunc + 0x1A1, &_jumpByte, 0x01);
		}

		else
		{
			vector<uint8_t> _nopArraySave(0x05);
			uint8_t _jumpByte = 0xEB;

			char* _checkSaveFunc = SignatureScan<char*>("\x40\x57\x48\x83\xEC\x50\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x70\x48\x89\x74\x24\x78\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x8B\xF9\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xF6\x48\x39\x35\x00\x00\x00\x00\x0F\x85\x3D\x01\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x38\x48\x85\xC0\x74\x1D\x45\x33\xC9\x44\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx????xx????xxxxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxxx");

			memcpy(_checkSaveFunc + 0x138, _nopArraySave.data(), 0x05);
			memcpy(_checkSaveFunc + 0x145, _nopArraySave.data(), 0x02);

			memcpy(_checkSaveFunc + 0x150, &_jumpByte, 0x01);
		}

		char* _nopArray = new char[0x05];
		fill(_nopArray, _nopArray + 0x05, 0x90);

	    char* _magicClearFunc = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x48\x89\x74\x24\x50\x48\x8B\xD8\x4C\x89\x74\x24\x58\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x0F\x29\x74\x24\x30\xF3\x0F\x10\x35\x00\x00\x00\x00\x0F\x29\x7C\x24\x20\x0F\x57\xFF\x48\x85\xDB\x75\x08", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxxxxxxxxxxx");
		char* _fadeReset = reinterpret_cast<char*>(dk::SOFTRESET::SoftResetThread) + 0x1ED;

		memcpy(_fadeReset, _nopArray, 0x05); // NOP resetting the Fades so that Soft Reset is actually smooth.
		memcpy(_magicClearFunc + 0x18A, _nopArray, 0x05); // NOP clearing Magic so that it doesn't fucking crash.

		char* _pictAppearFunc = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x48\x63\x41\x34\x48\x8B\xD9\x3B\x41\x30\x0F\x84\x00\x00\x00\x00\x48\x69\xD0\x60\x05\x00\x00\x48\x89\x7C\x24\x48", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx");

		uint32_t _fetchAddress = *reinterpret_cast<const uint32_t*>(_pictAppearFunc + 0x36);
		_fetchAddress -= 0x90;

		memcpy(_pictAppearFunc + 0x36, &_fetchAddress, 0x04);
				
		// ======================================================== //
		// THIS ENTIRE REGION IS TO MAKE DRIVE FORMS SHORTCUTTABLE! //
		// ======================================================== //

		// TODO: Toggleability.

		char* _equipOffset = SignatureScan<char*>("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x0F\xB6\x48\x02\x84\xC9\x74\x19", "xxxxx????xxxxxxxx");
		char* _listOffset = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x57\x48\x83\xEC\x20\x33\xDB\x48\x89\x6C\x24\x30\x41\x8B\xF8\x48\x8B\xE9", "xxxxxxxxxxxxxxxxxxxxxxx");
		char* _iconOffset = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xFA\x41\x0F\xB6\xD9\x41\x0F\xB6\xD0\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxx????");
		char* _categoryOffset = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x80\x01\x00\x00\x33\xF6\x89\x4C\x24\x28\x85\xC9\x48\x8D\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxx????");
	
		vector<uint8_t> _instShortcut
		{ 
			0xEB, 0x19, 
			0x3C, 0x0B, 0x75, 0x02, 0xB0, 
			0x88, 0x47, 0x01, 0xEB, 0xDC, 
			0xCE, 
			0xEB, 0xAA, 
			0xEB, 0x4E, 0x90, 0x90, 
			0x81, 0xCB, 0x00, 0x00, 0x24, 0x00, 
			0x80, 0xF9, 0x15, 0x74, 0xF2, 
			0xEB, 0x1B, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x31, 0xC0, 0x48, 0x83, 0xC4, 0x28, 0xC3,
			0xEB, 0x45, 0x90, 0x90, 
			0x81, 0xC3, 0x00, 0x00, 0x20, 0x00, 0xEB, 0xB5, 
			0x90, 0x90 
		};

		memcpy(_iconOffset + 0x1C, _iconOffset + 0x1D, 0x19);
		memcpy(_iconOffset + 0x1A, _instShortcut.data(), 0x02);

		memcpy(_iconOffset + 0x35, _instShortcut.data() + 0x02, 0x05);
		memcpy(_iconOffset + 0x3A, _instShortcut.data() + 0x0C, 0x01);
		memcpy(_iconOffset + 0x3B, _instShortcut.data() + 0x07, 0x05);

		memcpy(_listOffset + 0x18E, _instShortcut.data() + 0x0D, 0x02);
		memcpy(_listOffset + 0x138, _instShortcut.data() + 0x0F, 0x04);
		memcpy(_listOffset + 0x188, _instShortcut.data() + 0x13, 0x06);

		memcpy(_listOffset + 0x12C, _instShortcut.data() + 0x2C, 0x04);
		memcpy(_listOffset + 0x173, _instShortcut.data() + 0x30, 0x08);

		memcpy(_equipOffset + 0x33, _instShortcut.data() + 0x19, 0x05);
		memcpy(_equipOffset + 0x16, _instShortcut.data() + 0x1E, 0x07);
		memcpy(_equipOffset + 0x38, _instShortcut.data() + 0x25, 0x07);

		memcpy(_categoryOffset + 0x4AF, _instShortcut.data() + 0x38, 0x02);

		_instShortcut.clear();

		// ======================================================== //

		ReFined::MemoryManager::Allocate("CONFIG_MEMORY", 0x200);
		ReFined::MemoryManager::Allocate("INTRO_MEMORY", 0x300);

		ReFined::IntroMenu::Submit();
		ReFined::ConfigMenu::Submit();
		ReFined::Continue::Submit();
	}

	__declspec(dllexport) void OnFrame()
	{
		ReFined::Demand::TriggerReset();

		ReFined::Critical::HandleCrown();
		ReFined::Critical::ProcessDeath();
		ReFined::Critical::RegisterMagic();
		ReFined::Critical::ShowInformation();
		ReFined::Critical::RegisterMovement();
		ReFined::Critical::RetryBattles();
		ReFined::Critical::HandleConfiguration();

		ReFined::Continuous::DiscordRPC();
		ReFined::Continuous::FixSummonBGM();
		ReFined::Continuous::AutosaveLogic();
		ReFined::Continuous::EnforcePrompts();
		ReFined::Continuous::ActivateWarpGOA();
		ReFined::Continuous::HandleFrameLimiter();
	}
}