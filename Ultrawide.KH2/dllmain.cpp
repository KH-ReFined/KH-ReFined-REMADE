// dllmain.cpp : Defines the entry point for the DLL application.
#include "Windows.h"
#include "SigScan.h"
#include "memory_manager.h"
#include "area.h"
#include "cache_buff.h"
#include "command_draw.h"
#include "event.h"

#include <map>
#include <cstdio>
#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

vector<char*> POSITIVE_ASPECT_SHORT;
vector<char*> NEGATIVE_ASPECT_SHORT = MultiSignatureScan("\xC7\x00\x00\x00\xAB\xFF\xFF\xFF", "x???xxxx");
vector<char*> POSITIVE_ASPECT_LONG;
vector<char*> NEGATIVE_ASPECT_LONG = MultiSignatureScan("\xC7\x00\x00\x00\x00\x00\xAB\xFF\xFF\xFF", "x?????xxxx");

vector<char*> POSITIVE_ASPECT_BYTE;
vector<char*> NEGATIVE_ASPECT_BYTE;

char* VIEWPORT3D_ADDR = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);
char* INFORMATION_OFFSET = SignatureScan<char*>("\x41\xB8\x40\x00\x00\x00\xB9\xAA\x00\x00\x00\x66\x2B\xC1\x66\x44\x89\x44\x24\x20\x44\x0F\xB7\x43\x2C\x48\x8D\x8B\x60\x02\x00\x00\x44\x0F\xB7\xC8\x33\xD2", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

auto _fetchSimpleCont = MultiSignatureScan("\x41\x8D\x46\xAB\x41\x89\x84\x3F\x8C\x0D\x00\x00", "xxxxxxxxxxxx");

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C"
{
	__declspec(dllexport) void OnInit(wchar_t* mod_path)
	{
		char* _nopArray = new char[0x10];
		fill(_nopArray, _nopArray + 0x10, 0x90);

		char* _pictAppearFunc = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x48\x63\x41\x34\x48\x8B\xD9\x3B\x41\x30\x0F\x84\x00\x00\x00\x00\x48\x69\xD0\x60\x05\x00\x00\x48\x89\x7C\x24\x48", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx");

		uint32_t _fetchAddress = *reinterpret_cast<const uint32_t*>(_pictAppearFunc + 0x36);
		_fetchAddress -= 0x90;

		memcpy(_pictAppearFunc + 0x36, &_fetchAddress, 0x04);

		auto _fetchAdjustment = SignatureScan<char*>("\x48\x83\xEC\x28\x0F\x10\x41\x48\x4C\x8B\xC9\x4C\x8B\xD2\xF3\x0F\x10\x25\x00\x00\x00\x00\x0F\x57\xED\x0F\x11\x02\x41\x0F\x10\x00\x49\x8B\x41\x40", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx");

		memcpy(_fetchAdjustment + 0xF6, _nopArray, 0x06);
		memcpy(_fetchAdjustment + 0x101, _nopArray, 0x06);


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
			vector<uint8_t> _replaceFade{ 0xBA, 0x00, 0xFF, 0x00, 0x00 };

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
		float _resolutionHorizontal = *reinterpret_cast<float*>(VIEWPORT3D_ADDR + 0x10);
		float _resolutionVertical = *reinterpret_cast<float*>(VIEWPORT3D_ADDR + 0x14);

		float _commonDiv = 0x00F;

		if (_resolutionHorizontal != 0x00 && _resolutionVertical != 0x00)
		{
			uint32_t _offsetPositive = 0x55;
			uint32_t _offsetNegative = 0xFFFFFFAB;

			uint16_t _offsetInformation = 0xFFFF;

			auto _tempWidth = _resolutionHorizontal;
			auto _tempHeight = _resolutionVertical;

			while (_tempWidth != 0 && _tempHeight != 0)
			{
				if (_tempWidth > _tempHeight)
					_tempWidth = fmodf(_tempWidth, _tempHeight);

				else
					_tempHeight = fmodf(_tempHeight, _tempWidth);
			}

			_commonDiv = _tempWidth == 0 ? _tempHeight : _tempWidth;

			auto _ratioNum = _resolutionHorizontal / _commonDiv;
			auto _checkRatio = _resolutionVertical / _commonDiv;

			if (_checkRatio != 9)
			{
				auto _commonMulp = 9 / _checkRatio;
				_ratioNum = _ratioNum * _commonMulp;
			}

			auto _ratioMultiplier = 0.0625F * _ratioNum;

			memcpy(VIEWPORT3D_ADDR + 0x20, &_ratioMultiplier, 0x04);

			auto _heightFactor = _resolutionVertical / 1080;
			auto _widthCalc = floorf(_resolutionHorizontal / _heightFactor);

			_offsetPositive = ceilf(0.177F * (_widthCalc - 1440));
			_offsetNegative = _offsetPositive * -1;

			_offsetInformation = 0xFFFFU - floorf((((_widthCalc - 1440) - 480) * 0.1780F));

			if (POSITIVE_ASPECT_BYTE.size() == 0x00)
			{
				for (int i = 0xB0; i < 0xC0; i++)
				{
					stringstream stream;
					stream << char(i);

					auto _listPos = MultiSignatureScan(stream.str().append("\x55\x00\x00\x00").c_str(), "xxxxx");

					for (auto _ptr : _listPos)
					{
						if (_ptr > moduleInfo.startAddr + 0x1E0000)
							continue;

						POSITIVE_ASPECT_BYTE.push_back(_ptr);
					}
				}

				for (int i = 0xB0; i < 0xC0; i++)
				{
					stringstream stream;
					stream << char(i);

					auto _listNeg = MultiSignatureScan(stream.str().append("\xAB\xFF\xFF\xFF").c_str(), "xxxxx");

					for (auto _ptr : _listNeg)
					{
						if (_ptr > moduleInfo.startAddr + 0x1E0000)
							continue;

						NEGATIVE_ASPECT_BYTE.push_back(_ptr);
					}
				}
			}

			if (POSITIVE_ASPECT_SHORT.size() == 0x00)
			{
				auto _listPos = MultiSignatureScan("\xC7\x00\x00\x00\x55\x00\x00\x00", "x???xxxx");

				for (auto _ptr : _listPos)
				{
					if (_ptr > moduleInfo.startAddr + 0x1E0000)
						continue;

					POSITIVE_ASPECT_SHORT.push_back(_ptr);
				}
			}

			if (POSITIVE_ASPECT_LONG.size() == 0x00)
			{
				auto _listPos = MultiSignatureScan("\xC7\x00\x00\x00\x00\x00\x55\x00\x00\x00", "x?????xxxx");

				for (auto _ptr : _listPos)
				{
					if (_ptr > moduleInfo.startAddr + 0x1E0000)
						continue;

					POSITIVE_ASPECT_LONG.push_back(_ptr);
				}
			}

			for (int i = 0; i < POSITIVE_ASPECT_SHORT.size(); i++)
				memcpy(POSITIVE_ASPECT_SHORT[i] + 0x04, &_offsetPositive, 0x04);
			for (int i = 0; i < NEGATIVE_ASPECT_SHORT.size(); i++)
				memcpy(NEGATIVE_ASPECT_SHORT[i] + 0x04, &_offsetNegative, 0x04);

			for (int i = 0; i < POSITIVE_ASPECT_LONG.size(); i++)
				memcpy(POSITIVE_ASPECT_LONG[i] + 0x06, &_offsetPositive, 0x04);
			for (int i = 0; i < NEGATIVE_ASPECT_LONG.size(); i++)
				memcpy(NEGATIVE_ASPECT_LONG[i] + 0x06, &_offsetNegative, 0x04);

			for (int i = 0; i < POSITIVE_ASPECT_BYTE.size(); i++)
				memcpy(POSITIVE_ASPECT_BYTE[i] + 0x01, &_offsetPositive, 0x04);
			for (int i = 0; i < NEGATIVE_ASPECT_BYTE.size(); i++)
				memcpy(NEGATIVE_ASPECT_BYTE[i] + 0x01, &_offsetNegative, 0x04);

			for (int i = 0; i < _fetchSimpleCont.size(); i++)
				memcpy(_fetchSimpleCont[i] + 0x03, &_offsetNegative, 0x04);

			memcpy(INFORMATION_OFFSET + 0x0B, &_offsetInformation, 0x02);

			auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);
			auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

			if ((_commandPointer == 0x00 || _eventPointer != 0x00 || !*YS::AREA::IsInMap) && _offsetPositive != 0x55)
			{
				vector<char*> _fetchMission =
				{
					YS::CACHE_BUFF::SearchByName("msn/us/EH26_MS108.bar", -1),
					YS::CACHE_BUFF::SearchByName("msn/us/EH14_MS103.bar", -1),
					YS::CACHE_BUFF::SearchByName("msn/us/HB33_FM_LEX.bar", -1)
				};

				bool _isFetched = any_of(_fetchMission.begin(), _fetchMission.end(), [](char* x) {
					return (x != nullptr);
					});

				if (_isFetched)
				{
					uint32_t _positiveDefault = 0x55;
					uint32_t _negativeDefault = -0x55;

					memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x21, &_positiveDefault, 0x04);
					memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x29, &_negativeDefault, 0x04);
				}
			}

			else if (_commandPointer != 0x00 && _eventPointer == 0x00 && *YS::AREA::IsInMap)
			{
				memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x21, &_offsetPositive, 0x04);
				memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x29, &_offsetNegative, 0x04);
			}
		}
    }
}