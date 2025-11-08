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
#include "party.h"
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
#include "weapon_entry.h"

using namespace std;

bool handled = false;

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

		char* _nopArray = new char[0x10];
		fill(_nopArray, _nopArray + 0x10, 0x90);

	    char* _magicClearFunc = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x48\x89\x74\x24\x50\x48\x8B\xD8\x4C\x89\x74\x24\x58\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x0F\x29\x74\x24\x30\xF3\x0F\x10\x35\x00\x00\x00\x00\x0F\x29\x7C\x24\x20\x0F\x57\xFF\x48\x85\xDB\x75\x08", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxxxxxxxxxxx");
		char* _fadeReset = reinterpret_cast<char*>(dk::SOFTRESET::SoftResetThread) + 0x1ED;

		memcpy(_fadeReset, _nopArray, 0x05); // NOP resetting the Fades so that Soft Reset is actually smooth.
		memcpy(_magicClearFunc + 0x18A, _nopArray, 0x05); // NOP clearing Magic so that it doesn't fucking crash.

		char* _pictAppearFunc = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x48\x63\x41\x34\x48\x8B\xD9\x3B\x41\x30\x0F\x84\x00\x00\x00\x00\x48\x69\xD0\x60\x05\x00\x00\x48\x89\x7C\x24\x48", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx");

		uint32_t _fetchAddress = *reinterpret_cast<const uint32_t*>(_pictAppearFunc + 0x36);
		_fetchAddress -= 0x90;

		memcpy(_pictAppearFunc + 0x36, &_fetchAddress, 0x04);

		auto _fetchAdjustment = SignatureScan<char*>("\x48\x83\xEC\x28\x0F\x10\x41\x48\x4C\x8B\xC9\x4C\x8B\xD2\xF3\x0F\x10\x25\x00\x00\x00\x00\x0F\x57\xED\x0F\x11\x02\x41\x0F\x10\x00\x49\x8B\x41\x40", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx");
		
		memcpy(_fetchAdjustment + 0xF6, _nopArray, 0x06);
		memcpy(_fetchAdjustment + 0x101, _nopArray, 0x06);
															   
		// Fixes a problem with Weapon Swapping that crashes the game.

		auto _fixWeaponHotswap = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x33\xC0\x0F\x1F\x44\x00\x00\x48\x85\xC0\x75\x09\x48\x8B\x05\x00\x00\x00\x00\xEB\x08\x8B\x48\x70\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x15\x48\x39\x58\x58\x75\xDF\xB9\xFF\xFF\x00\x00\x66\x01\x48\x02\x48\x83\xC4\x20\x5B\xC3\xB9\xFF\xFF\x00\x00\x66\x01\x48\x02\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxxxxxxx????xxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		auto _jumpSpace = _fixWeaponHotswap + 0x294;
		auto _moduleStart = reinterpret_cast<uint64_t>(moduleInfo.startAddr) - reinterpret_cast<uint64_t>(_jumpSpace) - 0x07;

		vector<uint8_t> _instJumpWorkaround
		{
			0xE9, 0x59, 0x02, 0x00, 0x00,
			0x90, 0x90, 0x90, 0x90, 0x90,
			0xB9, 0xFF, 0xFF, 0x00, 0x00,
			0xE9, 0x4A, 0x02, 0x00, 0x00
		};

		memcpy(_fixWeaponHotswap + 0x36, _instJumpWorkaround.data(), 0x14);

		vector<uint8_t> _instHotpatchWeapon
		{
			0x4C, 0x8D, 0x15, 0x00, 0x00, 0x00, 0x00,
			0x4C, 0x39, 0xD0,
		    0xEB, 0x03,
			0xC2, 0x00, 0x00,
			0x7C, 0x04,
			0x66, 0x01, 0x48, 0x02,
			0x4D, 0x31, 0xD2,
			0xEB, 0x8B
		};

		vector<uint8_t> _instHotpatchCont
		{
			0x48, 0x83, 0xC4, 0x20,
			0x5B, 0xC3
		};

		memcpy(_instHotpatchWeapon.data() + 0x03, &_moduleStart, 0x04);

		memcpy(_jumpSpace, _instHotpatchWeapon.data(), 0x1A);
		memcpy(_jumpSpace - 0x5B, _instHotpatchCont.data(), 0x06);

		// Fixed Party Limits crashing the game post-fights.

		auto _fetchLimitCheck = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x8B\x79\x10\x48\x8B\xF1\x0F\x29\x74\x24\x20\xF3\x0F\x10\x71\x18\x8B\x49\x08\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x8B\x0E\x48\x8B\xD8\xE8\x00\x00\x00\x00\x0F\x28\xDE\x44\x8B\xC7\x48\x8B\xD3\x48\x8B\xC8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxx????xxxxxxxxxxxx");
		auto _fetchLimitPint = ResolveRelativeAddress<uint64_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x30\x48\x8B\xFA\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\xE8\x0F\xB7\x08\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x85\xC0\x0F\x84\xCF\x00\x00\x00\xB9\x28\x01\x00\x00\xE8\x00\x00\x00\x00\x33\xDB\x48\x85\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxx", 0x6A);

		vector<uint8_t> _instFixLimits
		{
			0x4C, 0x8B, 0x1D, 0x00, 0x00, 0x00, 0x00,
			0x4D, 0x85, 0xDB,
			0x75, 0x0A,
			0xEB, 0x02,
			0xEB, 0xF0,
			0x4D, 0x31, 0xDB,
			0xC3, 0x90, 0x90
		};

		auto _limitAddrCalc = _fetchLimitPint - reinterpret_cast<uint64_t>(_fetchLimitCheck - 0x0E) - 0x07;
		memcpy(_instFixLimits.data() + 0x03, &_limitAddrCalc, 0x04);

		vector<uint64_t> _fetchFunction(0x68);
		memcpy(_fetchFunction.data(), _fetchLimitCheck, 0x68);
		memcpy(_fetchLimitCheck + 0x08, _fetchFunction.data(), 0x68);

		auto _tempPtr = reinterpret_cast<uint32_t*>(_fetchLimitCheck + 0x4E);
		*_tempPtr -= 0x08;

		_tempPtr = reinterpret_cast<uint32_t*>(_fetchLimitCheck + 0x2B);
		*_tempPtr -= 0x08;
		_tempPtr = reinterpret_cast<uint32_t*>(_fetchLimitCheck + 0x33);
		*_tempPtr -= 0x08;
		_tempPtr = reinterpret_cast<uint32_t*>(_fetchLimitCheck + 0x3D);
		*_tempPtr -= 0x08;

		memcpy(_fetchLimitCheck - 0x0E, _instFixLimits.data(), 0x16);

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

		// ======================================================== //

		// Create a memory space for the Gauge overrider. This replacement function is long, and replacing it for each function space requires exactly that. Space.
		// As such I just create an all-encompassing function in there, and redirect and and all functions that use this very specific maths to here.

		ReFined::MemoryManager::Allocate("GAUGE_ASPECT_OVERRIDE", 0x100);

		// The function that will do the math.
		// P.S. - Please excuse me using literal byte arrays for this. I do not have nor want to use an assembler.

		vector<uint8_t> _gaugeFunc =
		{
			0x45, 0x89, 0xE2,				// mov r10d, r12d
			0x41, 0x83, 0xFA, 0x10,			// cmp r10d, 0x10
			0x72, 0x0C,						// jb 0x0C
			0x41, 0x89, 0xEA,				// mov r10d, ebp
			0x41, 0x83, 0xFA, 0x10,			// cmp r10d, 0x10
			0x72, 0x03,						// jb 0x03
			0x41, 0x89, 0xF2,				// mov r10d, esi
			0x45, 0x85, 0xD2,				// test r10d, r10d
			0x74, 0x0E,						// je 0x0E
			0x41, 0x83, 0xFA, 0x01,			// cmp r10d, 0x01
			0x75, 0x0D,						// jne 0x0D
			0x05, 0x55, 0x00, 0x00, 0x00,	// add eax, 0x00000055
			0x41, 0xFF, 0xE1,				// jmp r9
			0x05, 0xAB, 0xFF, 0xFF, 0xFF,   // add eax, 0xFFFFFFAB
			0x41, 0xFF, 0xE1				// jmp r9
		};

		memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE"), _gaugeFunc.data(), _gaugeFunc.size());

		// Fetch all functions that will make use of the function above.

		vector<char*> _fetchFunctions =
		{
			SignatureScan<char*>("\x85\xF6\x74\x0A\x83\xFE\x01\x75\x08\x83\xC0\x55\xEB\x03\x83\xC0\xAB", "xxxxxxxxxxxxxxxxx"),
			SignatureScan<char*>("\x85\xED\x74\x0A\x83\xFD\x01\x75\x08\x83\xC0\x55\xEB\x03\x83\xC0\xAB", "xxxxxxxxxxxxxxxxx"),
			SignatureScan<char*>("\x45\x85\xE4\x74\x0B\x41\x83\xFC\x01\x75\x08\x83\xC0\x55\xEB\x03\x83\xC0\xAB", "xxxxxxxxxxxxxxxxxxx")
		};

		// Redirect all the parts that do the math individually to that big function block.

		for (int i = 0; i < 0x03; i++)
		{
			auto _funcLength = i == 0x02 ? 0x13 : 0x11;

			char* _nopFunction = new char[_funcLength];
			fill(_nopFunction, _nopFunction + _funcLength, 0x90);

			memcpy(_fetchFunctions[i], _nopFunction, _funcLength);

			vector<uint8_t> _redirectFunc =
			{
				0x4C, 0x8B, 0x4C, 0x24, 0xF8,	// mov r9, [rsp-0x08]
				0x49, 0x83, 0xC1, 0x0E,			// add r9, 0x0E
				0xE9							// jmp
			};

			memcpy(_fetchFunctions[i], _redirectFunc.data(), 0x0A);

			uint32_t _funcDifference = ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") - (_fetchFunctions[i] + 0x0E);
			memcpy(_fetchFunctions[i] + 0x0A, &_funcDifference, 0x04);
		}

		// Fetch the function that handles the fade-in and fade-out of the Information Text.

		auto _fetchInformation = SignatureScan<char*>("\x41\xB8\x40\x00\x00\x00\xB9\xAA\x00\x00\x00\x66\x2B\xC1\x66\x44\x89\x44\x24\x20\x44\x0F\xB7\x43\x2C\x48\x8D\x8B\x60\x02\x00\x00\x44\x0F\xB7\xC8\x33\xD2", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		// This basically forces the in and out values of the text to be a specific value.
		// No, I do not know why this works. I did random shit and struck gold. No, I am not complaining. 

		vector<uint8_t> _informationFunc =
		{
			0x41, 0x89, 0xC1,		 // mov r9d, eax
			0x66, 0xBA, 0xFF, 0xFF   // mov dx, 0xFFFF
		};

		vector<uint8_t> _redirInformation =
		{
			0x41, 0xB9, 0xC0, 0x00, 0x00, 0x00 // mov r9d, 0xC0
		};

		memcpy(_fetchInformation + 0x06, _nopArray, 0x08);
		memcpy(_fetchInformation + 0x20, _nopArray, 0x06);

		memcpy(_fetchInformation + 0x06, _informationFunc.data(), 0x07);
		memcpy(_fetchInformation + 0x20, _redirInformation.data(), 0x06);


		// Fetch all functions that handle fade-in and fade-outs in any way within the 2dFade rectangle.
											     
		auto _fetchAllFade = MultiSignatureScan("\x41\xB8\xFF\xFF\xFF\xFF\x48\x8D\x0D\x00\x00\x00\x00\x0F\xB7\xD3\x66\xF7\xD2\xE8\x00\x00\x00\x00\xB8\x01\x01\x00\x00", "xxxxxxxxx????xxxxxxx????xxxxx");

		for (auto _function : _fetchAllFade)
		{
			uint32_t _fadeValue = 0x500;
			vector<uint8_t> _replaceFade { 0xBA, 0x00, 0xFF, 0x00, 0x00 };

			memcpy(_function + 0x0D, _nopArray, 0x06);
			memcpy(_function + 0x0D, _replaceFade.data(), 0x05);

			memcpy(_function + 0x19, &_fadeValue, 0x04);
		}

		// Simple counter offset hack.

		auto _fetchSimpleCont = MultiSignatureScan("\x41\x8D\x46\xAB\x41\x89\x84\x3F\x8C\x0D\x00\x00", "xxxxxxxxxxxx");

		for (auto _function : _fetchSimpleCont)
		{
			uint64_t _fetchCall = ResolveCallRelative(_function + 0x31);

			vector<uint8_t> _replaceCounter
			{
				0x41, 0x8D, 0x86, 0x40, 0xFF, 0xFF, 0xFF,		// lea eax, [r14-0xC0]
				0x41, 0x89, 0x84, 0x3F, 0x8C, 0x0D, 0x00, 0x00,	// mov [r15+rdi+0x0D8C], eax
				0x3B, 0xB5, 0xB0, 0x0D, 0x00, 0x00,				// cmp esi, [rbp+0x0DB0];
				0x7C, 0xB6,										// jl "_function - 0x33"
				0x4C, 0x8B, 0xBC, 0x24, 0x88, 0x00, 0x00, 0x00,	// mov r15, [rsp+0x88]
				0x48, 0x8B, 0xBC, 0x24, 0x80, 0x00, 0x00, 0x00,	// mov rdi, [rsp+0x80]
				0x48, 0x8B, 0x5C, 0x24, 0x78,					// mov rbx, [rsp+0x78]
				0x48, 0x8B, 0x4C, 0x24, 0x40,					// mov rcx, [rsp+0x40]
				0x48, 0x31, 0xE1,								// xor rcx, rsp
				0xE8, 0x00, 0x00, 0x00, 0x00,					// call "_fetchCall"
				0x48, 0x83, 0xC4, 0x50,							// add rsp, 0x50
				0x41, 0x5E,										// pop r14
				0x5E,											// pop rsi
				0x5D,											// pop rbp
				0xC3											// ret
			};

			uint32_t _fetchCalc = static_cast<uint32_t>(_fetchCall - reinterpret_cast<uint64_t>(_function + 0x39));

			memcpy(_function, _replaceCounter.data(), _replaceCounter.size());
			memcpy(_function + 0x35, &_fetchCalc, 0x04);
		}

		// Disables culling. Causes some side effects that I don't believe anyone will notice.
								
		uint8_t _jmpByte = 0xEB;
		uint8_t _falseByte = 0x00;

		auto _fetchCulling3D = SignatureScan<char*>("\x48\x8B\xC4\x48\x89\x58\x18\x48\x89\x70\x20\x55\x57\x41\x54\x41", "xxxxxxxxxxxxxxxx");
		auto _fetchCulling2D = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xFA\xE8", "xxxxxxxxxxxxxxxxxxx");

		memcpy(_fetchCulling3D + 0x11D, &_jmpByte, 0x01);
		memcpy(_fetchCulling3D + 0x12B, &_jmpByte, 0x01);
		memcpy(_fetchCulling3D + 0x133, &_jmpByte, 0x01);
		memcpy(_fetchCulling3D + 0x141, &_jmpByte, 0x01);
		memcpy(_fetchCulling3D + 0x149, &_jmpByte, 0x01);
		memcpy(_fetchCulling3D + 0x152, &_jmpByte, 0x01);

		memcpy(_fetchCulling2D + 0x06C, &_falseByte, 0x01);
	}

	__declspec(dllexport) void OnFrame()
	{
		ReFined::Demand::FormKeyLogic();
		ReFined::Demand::TriggerReset();
		ReFined::Demand::ShortcutSets();
		ReFined::Demand::EncounterPlus();

		ReFined::Critical::HandleCrown();
		ReFined::Critical::ProcessDeath();
		ReFined::Critical::RegisterMagic();
		ReFined::Critical::ShowInformation();
		ReFined::Critical::RegisterMovement();
		ReFined::Critical::RetryBattles();
		ReFined::Critical::HandleConfiguration();
		ReFined::Critical::HandleIntro();
		ReFined::Critical::PrologueSkip();
		ReFined::Critical::AspectCorrection();

		ReFined::Continuous::DiscordRPC();
		ReFined::Continuous::Autoattack();
		ReFined::Continuous::HandleShake();
		ReFined::Continuous::FixSummonBGM();
		ReFined::Continuous::AutosaveLogic();
		ReFined::Continuous::HandleSaveGlow();
		ReFined::Continuous::EnforcePrompts();
		ReFined::Continuous::ActivateWarpGOA();
		ReFined::Continuous::HandleFrameLimiter();

		/*
		if (!*YS::TITLE::IsTitle && *YS::AREA::IsInMap && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x00 && !handled)
		{
			YS::PARTY::ChangeWeapon(0x01, false, 0x0029);
			handled = true;
		}
		*/
	}
}