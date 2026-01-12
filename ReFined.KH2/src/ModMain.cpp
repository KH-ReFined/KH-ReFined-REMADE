#define _CRT_SECURE_NO_WARNINGS

#include "ModMain.h"

using namespace std;

bool IS_INIT = false;

wstring MOD_PATH;

vector<void(*)(const wchar_t*)> moduleinit;
vector<void(*)()> moduleexec;

bool DISCORD_ENABLED = true;

bool IS_FASTBOOT = false;
bool IS_NOASPECT = false;

uint16_t ReFined::Demand::RESET_COMBO = YS::HARDPAD::BUTTONS::NONE;

uint8_t ReFined::Continuous::ROOM_AMOUNT = 3;
uint8_t ReFined::Continuous::SAVE_SLOT_OFFSET = 99;

extern "C"
{
	__declspec(dllexport) void OnInit(wchar_t* mod_path)
	{			

		auto _fetchCommandLine = wstring(GetCommandLine());

		IS_FASTBOOT = _fetchCommandLine.find(L"-fastboot") != wstring::npos;
		IS_NOASPECT = _fetchCommandLine.find(L"-noaspect") != wstring::npos;

		if (IS_FASTBOOT)
		{
			char* _fetchTitle = SignatureScan<char*>("\x74\x69\x74\x6C\x65\x2E\x32\x6C\x64\x00\x00\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx");
			string _fastFile = "title_fast.2ld";

			memcpy(_fetchTitle, _fastFile.c_str(), _fastFile.size());
		}

		MOD_PATH = wstring(mod_path);

		auto _checkSteam = FindModule("steam_api64.dll");

		char* _nopArray = new char[0x10];
		fill(_nopArray, _nopArray + 0x10, 0x90);

		uint8_t _jumpByte = 0xEB;

		if (_checkSteam)
		{
			char* _checkSaveFunc = SignatureScan<char*>("\x40\x55\x56\x57\x48\x81\xEC\xA0\x00\x00\x00\x48\xC7\x44\x24\x38\xFE\xFF\xFF\xFF\x48\x89\x9C\x24\xD0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x90\x00\x00\x00\x8B\xF1\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xED\x8D\x5D\x01\x48\x39\x2D\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x85\xC0\x74\x1D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxx????xx????xxxxxxxx????xx????xxxxxx????xxxxxxxxxx");
			
			memcpy(_checkSaveFunc + 0x189, _nopArray, 0x05);
			memcpy(_checkSaveFunc + 0x196, _nopArray, 0x02);

			memcpy(_checkSaveFunc + 0x1A1, &_jumpByte, 0x01);
		}

		else
		{
			char* _checkSaveFunc = SignatureScan<char*>("\x40\x57\x48\x83\xEC\x50\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x70\x48\x89\x74\x24\x78\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x8B\xF9\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xF6\x48\x39\x35\x00\x00\x00\x00\x0F\x85\x3D\x01\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x38\x48\x85\xC0\x74\x1D\x45\x33\xC9\x44\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx????xx????xxxxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxxx");

			memcpy(_checkSaveFunc + 0x138, _nopArray, 0x05);
			memcpy(_checkSaveFunc + 0x145, _nopArray, 0x02);

			memcpy(_checkSaveFunc + 0x150, &_jumpByte, 0x01);
		}

		char* _readRequestSubMDLX = SignatureScan<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????");
		char* _getCacheBuffStatusMDLX = SignatureScan<char*>("\x4C\x8D\x46\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????");

		char* _fetchMDLX = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x85\xD2\x48\x8D\x79\x08\x48\x8D\x1D\x48", "xxxxxxxxxxxxxxxxxxxxx");
		char* _fetchAPDX = SignatureScan<char*>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx");

		char* _readRequestSubAPDX = _readRequestSubMDLX + 0xCA;
		char* _getCacheBuffStatusAPDX = _getCacheBuffStatusMDLX + 0xDA;

		vector<uint8_t> _instructionREPLACE =
		{
			0x48, 0x8D, 0x0E,
			0x48, 0x31, 0xD2,
			0xE8, 0x00, 0x00, 0x00, 0x00,
			0x48, 0x31, 0xC0
		};

		fill(_getCacheBuffStatusMDLX, _getCacheBuffStatusMDLX + 0x56, 0x90);
		fill(_getCacheBuffStatusAPDX, _getCacheBuffStatusAPDX + 0x62, 0x90);

		uint32_t _calcCall = _fetchMDLX - (_getCacheBuffStatusMDLX + 0x0B);
		memcpy(_instructionREPLACE.data() + 0x07, &_calcCall, 0x04);

		memcpy(_getCacheBuffStatusMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());

		_calcCall = _fetchAPDX - (_getCacheBuffStatusAPDX + 0x0B);
		memcpy(_instructionREPLACE.data() + 0x07, &_calcCall, 0x04);

		memcpy(_getCacheBuffStatusAPDX, _instructionREPLACE.data(), _instructionREPLACE.size());

		fill(_readRequestSubMDLX, _readRequestSubMDLX + 0x56, 0x90);

		_instructionREPLACE[0] += 0x01;
		_instructionREPLACE[2] += 0x01;

		_calcCall = _fetchMDLX - (_readRequestSubMDLX + 0x0B);
		memcpy(_instructionREPLACE.data() + 0x07, &_calcCall, 0x04);

		memcpy(_readRequestSubMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());

		_calcCall = _fetchAPDX - (_readRequestSubAPDX + 0x0B);
		memcpy(_instructionREPLACE.data() + 0x07, &_calcCall, 0x04);

		memcpy(_readRequestSubAPDX, _instructionREPLACE.data(), _instructionREPLACE.size());

		fill(_fetchMDLX, _fetchMDLX + 0x71, 0x90);
		fill(_fetchAPDX, _fetchAPDX + 0x92, 0x90);

		vector<uint8_t> _mdlxRedirect =
		{
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		uint64_t _fetchVerify = reinterpret_cast<uint64_t>(ReFined::Continuous::VerifyMDLX);

		memcpy(_mdlxRedirect.data() + 0x06, &_fetchVerify, 0x08);
		memcpy(_fetchMDLX, _mdlxRedirect.data(), _mdlxRedirect.size());

		_fetchVerify = reinterpret_cast<uint64_t>(ReFined::Continuous::VerifyAPDX);

		memcpy(_mdlxRedirect.data() + 0x06, &_fetchVerify, 0x08);
		memcpy(_fetchAPDX, _mdlxRedirect.data(), _mdlxRedirect.size());


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

		YS::PANACEA_ALLOC::Allocate("CONFIG_MEMORY", 0x200);
		YS::PANACEA_ALLOC::Allocate("INTRO_MEMORY", 0x300);

		ReFined::IntroMenu::Submit();
		ReFined::Continue::Submit();

		Tz::HookConfig::Submit();

		// ======================================================== //

		if (!IS_NOASPECT)
		{
			// Create a memory space for the Gauge overrider. This replacement function is long, and replacing it for each function space requires exactly that. Space.
			// As such I just create an all-encompassing function in there, and redirect and and all functions that use this very specific maths to here.

			YS::PANACEA_ALLOC::Allocate("GAUGE_ASPECT_OVERRIDE", 0x100);

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

			memcpy(YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE"), _gaugeFunc.data(), _gaugeFunc.size());

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

				uint32_t _funcDifference = YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE") - (_fetchFunctions[i] + 0x0E);
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

		dk::NEXT_FORM::instance = (char*)malloc(0xDB8);

		if (dk::NEXT_FORM::instance != nullptr)
		{
			memset(dk::NEXT_FORM::instance, 0x00, 0xDB8);

			*reinterpret_cast<uint64_t*>(dk::NEXT_FORM::instance) = reinterpret_cast<uint64_t>(dk::NEXT_FORM::VTABLE_CLASS);
			*reinterpret_cast<uint64_t*>(dk::NEXT_FORM::instance + 0x38) = reinterpret_cast<uint64_t>(dk::NEXT_FORM::VTABLE_SPRITE);

			YI::SEQUENCE::CreateNew(dk::NEXT_FORM::instance + 0x58);

			char* _seqdChain = dk::NEXT_FORM::instance + 0x230;

			for (int i = 0; i < 7; i++)
			{
				YI::SEQUENCE::CreateNew(_seqdChain);
				_seqdChain += 0x1A0;
			}

			*reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0x048) = 0x00;
			*reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0x214) = 0x00;
		}

		wchar_t search[MAX_PATH];
		wcscpy(search, mod_path);
		wcscat(search, L"\\dll\\modules\\ModuleRF-*.dll");

		WIN32_FIND_DATAW find;
		HANDLE fh = FindFirstFileW(search, &find);

		if (fh != INVALID_HANDLE_VALUE)
		{
			do
			{
				wchar_t filepath[MAX_PATH];
				wcscpy(filepath, mod_path);
				wcscat(filepath, L"\\dll\\modules\\");
				wcscat(filepath, find.cFileName);

				HMODULE dllhandle = LoadLibraryW(filepath);
				if (dllhandle)
				{
					void (*funcInit)(const wchar_t*) = (void(*)(const wchar_t*))GetProcAddress(dllhandle, "RF_ModuleInit");
					void (*funcExec)() = (void(*)())GetProcAddress(dllhandle, "RF_ModuleExecute");

					uint32_t* (*checkIntro)() = (uint32_t *(*)())GetProcAddress(dllhandle, "RF_CheckIntro");
					vector<uint32_t> _vectorIntro(0);

					if (checkIntro)
					{
						auto _fetchIntro = checkIntro();
						auto _sizeVector = 0x0C + 0x04 * (_fetchIntro[0] * 2);

						_vectorIntro.resize(_sizeVector);
						memcpy(_vectorIntro.data(), _fetchIntro, _sizeVector);

						auto _newEntry = ReFined::IntroMenu::Entry(_vectorIntro.at(0), _vectorIntro.at(1), _vectorIntro.at(2), vector<uint32_t>(_vectorIntro.begin() + 3, _vectorIntro.begin() + 3 + _vectorIntro.at(0)), vector<uint32_t>(_vectorIntro.begin() + 3 + _vectorIntro.at(0), _vectorIntro.begin() + 3 + _vectorIntro.at(0) + _vectorIntro.at(0)));
						ReFined::IntroMenu::Add(ReFined::IntroMenu::Children.size(), _newEntry);

						bool** introSeek = (bool**)GetProcAddress(dllhandle, "INTRO_SEEK");

						assert(introSeek != nullptr);
						*introSeek = reinterpret_cast<bool*>(YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x200 + ((ReFined::IntroMenu::Children.size() - 1) * 0x04));
					}

					if (funcInit)
						moduleinit.push_back(funcInit);

					if (funcExec)
						moduleexec.push_back(funcExec);
				}
			} while (FindNextFileW(fh, &find));

			FindClose(fh);
		}

		wchar_t _configPath[MAX_PATH];
		wcscpy(_configPath, mod_path);
		wcscat(_configPath, L"\\dll\\reFined.cfg");
		auto _wideStr = wstring(_configPath);

		mINI::INIFile _configFile(string(_wideStr.begin(), _wideStr.end()));
		mINI::INIStructure _configStruct;

		_configFile.read(_configStruct);

		auto _fetchButtons = _configStruct["General"]["resetCombo"];

		if (_fetchButtons.find("NONE") == string::npos)
		{
			size_t _buttonPos = 0;
			string _buttonToken;
			string _tempStr = _fetchButtons;

			while ((_buttonPos = _tempStr.find(" + ")) != string::npos)
			{
				_buttonToken = _tempStr.substr(0, _buttonPos);
				_tempStr.erase(0, _buttonPos + 3);

				transform(_buttonToken.begin(), _buttonToken.end(), _buttonToken.begin(), ::toupper);

				ReFined::Demand::RESET_COMBO |= YS::HARDPAD::BUTTONS_MAP[_buttonToken];

				if (_tempStr.find(" + ") == string::npos)
					ReFined::Demand::RESET_COMBO |= YS::HARDPAD::BUTTONS_MAP[_tempStr];
			}
		}

		DISCORD_ENABLED = _configStruct["General"]["discordRPC"] == "true" ? true : false;

		ReFined::Continuous::ROOM_AMOUNT = atoi(_configStruct["General"]["saveRoomAmount"].c_str());
		ReFined::Continuous::SAVE_SLOT_OFFSET = atoi(_configStruct["General"]["saveSlot"].c_str());

		if (ReFined::Continuous::ROOM_AMOUNT == 0x00)
			ReFined::Continuous::ROOM_AMOUNT = 1;
	}

	__declspec(dllexport) void OnFrame()
	{
		if (!IS_INIT)
		{
			auto _fetchMessage = YS::MESSAGE::GetData(0x8ADC);

			if (_fetchMessage != nullptr)
				for (auto _execFunc : moduleinit)
					_execFunc(MOD_PATH.c_str());

			char* _fetchFirstShopface = SignatureScan<char*>("\x40\x56\x48\x81\xEC\xC0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xA0\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x33\xF6\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxxxxx????xxx????");
			char* _fetchSecondShopface = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xDA\x8B\xF9\x48\x8B\xCB\x48\x8D\x15\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????x????");
			char* _fetchThirdShopface = SignatureScan<char*>("\x40\x57\x48\x81\xEC\xB0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xA0\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x33\xFF\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxxxxx????xxx????");

			auto _fetchShopface = YS::FILE::GetSize("00shopface.bin");

			if (_fetchShopface != 0x00)
			{
				char* _allocShopface = (char*)malloc(_fetchShopface);
				auto _readShopface = YS::FILE::Read("00shopface.bin", _allocShopface);

				if (_readShopface != 0x00)
				{
					uint8_t _faceCount = *_allocShopface;

					YS::PANACEA_ALLOC::Allocate("SHOPFACE_NAMES", 0x10 * _faceCount);
					YS::PANACEA_ALLOC::Allocate("SHOPFACE_STRUCTS", 0x10 * _faceCount);

					for (int i = 0; i < _faceCount; i++)
					{
						auto _objectID = *reinterpret_cast<uint16_t*>(_allocShopface + 0x10 + (0x10 * i));
						char _faceName[0x0E];

						memcpy(_faceName, _allocShopface + 0x02 + 0x10 + (0x10 * i), 0x0E);

						auto _calculateNameAddr = reinterpret_cast<uint64_t>(YS::PANACEA_ALLOC::Get("SHOPFACE_NAMES") + 0x10 * i);

						memcpy(YS::PANACEA_ALLOC::Get("SHOPFACE_STRUCTS") + 0x10 * i, &_objectID, 0x02);
						memcpy(YS::PANACEA_ALLOC::Get("SHOPFACE_STRUCTS") + (0x10 * i) + 0x08, &_calculateNameAddr, 0x08);

						memcpy(YS::PANACEA_ALLOC::Get("SHOPFACE_NAMES") + 0x10 * i, _faceName, 0x0E);
					}

					char* _structOffset = reinterpret_cast<char*>(YS::PANACEA_ALLOC::Get("SHOPFACE_STRUCTS"));

					RedirectLEA(_fetchFirstShopface + 0x53, _structOffset);
					RedirectLEA(_fetchThirdShopface + 0x50, _structOffset);
					RedirectLEA(_fetchSecondShopface + 0x28, _structOffset);

					RedirectLEA(_fetchFirstShopface + 0x62, _structOffset + 0x08);
					RedirectLEA(_fetchThirdShopface + 0x41, _structOffset + 0x08);
					RedirectLEA(_fetchSecondShopface + 0x83, _structOffset + 0x08);
				}
			}

			auto _fetchEnemyFirst = YS::FILE::GetSize("mdl/ACTOR_SORA.mdlx");
			auto _fetchEnemySecond = YS::FILE::GetSize("o3d/ACTOR_SORA.mdlx");

			vector<uint16_t> _enemyConfig{ 0x01, 0x571F, 0x573A, 0x573B, 0x0000 };

			if (_fetchEnemyFirst != 0x00)
			{
				auto _findName = find(_enemyConfig.begin(), _enemyConfig.end(), 0x573B);
				_enemyConfig.insert(_findName, 0x573C);

				auto _findBitwise = find(_enemyConfig.begin(), _enemyConfig.end(), 0x0000);
				_enemyConfig.insert(_findBitwise, 0x573D);

				_enemyConfig[0] += 1;
			}

			if (_fetchEnemySecond != 0x00)
			{
				auto _findName = find(_enemyConfig.begin(), _enemyConfig.end(), 0x573B);
				_enemyConfig.insert(_findName, 0x573E);

				auto _findBitwise = find(_enemyConfig.begin(), _enemyConfig.end(), 0x0000);
				_enemyConfig.insert(_findBitwise, 0x573F);

				_enemyConfig[0] += 1;
			}

			if (_fetchEnemyFirst != 0x00)
				_enemyConfig.push_back(0x0200);

			if (_fetchEnemySecond != 0x00)
				_enemyConfig.push_back(0x0400);

			if (_enemyConfig[0] > 1)
				Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _enemyConfig);

			auto _fetchMusicFirst = YS::FILE::GetSize("bgm/ps2md050.win32.scd");
			auto _fetchMusicSecond = YS::FILE::GetSize("bgm/mdbgm050.win32.scd");

			vector<uint16_t> _musicConfig { 0x01, 0x5718, 0x5719, 0x571A, 0x0000 };

			if (_fetchMusicFirst != 0x00)
			{
				auto _findName = find(_musicConfig.begin(), _musicConfig.end(), 0x571A);
				_musicConfig.insert(_findName, 0x571B);

				auto _findBitwise = find(_musicConfig.begin(), _musicConfig.end(), 0x0000);
				_musicConfig.insert(_findBitwise, 0x571C);

				_musicConfig[0] += 1;
			}

			if (_fetchMusicSecond != 0x00)
			{
				auto _findName = find(_musicConfig.begin(), _musicConfig.end(), 0x571A);
				_musicConfig.insert(_findName, 0x571D);

				auto _findBitwise = find(_musicConfig.begin(), _musicConfig.end(), 0x0000);
				_musicConfig.insert(_findBitwise, 0x571E);

				_musicConfig[0] += 1;
			}

			if (_fetchMusicFirst != 0x00)
				_musicConfig.push_back(0x0080);

			if (_fetchMusicSecond != 0x00)
				_musicConfig.push_back(0x0100);

			if (_musicConfig[0] > 1)
				Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _musicConfig);

			vector<size_t> _loadedLangs
			{
				YS::FILE::GetSize("voice/jp/battle/tt0_sora.win32.scd"),
				YS::FILE::GetSize("voice/es/battle/tt0_sora.win32.scd"),
				YS::FILE::GetSize("voice/de/battle/tt0_sora.win32.scd"),
				YS::FILE::GetSize("voice/bg/battle/tt0_sora.win32.scd"),
			};

			vector<uint16_t> _subAudioConfig  { 0x00, 0x572B };
			vector<uint16_t> _mainAudioConfig { 0x01, 0x570B, 0x570C, 0x570D };

			for (int i = 1; i < 4; i++)
			{
				if (_loadedLangs[i] != 0x0000)
				{
					_subAudioConfig.push_back(0x570E + 0x02 * i);
					_subAudioConfig[0] += 1;
				}
			}

			for (int i = 1; i < 4; i++)
			{
				if (_loadedLangs[i] != 0x0000)
					_subAudioConfig.push_back(0x570F + 0x02 * i);
			}

			if (_loadedLangs[0] != 0x0000)
			{
				_mainAudioConfig.insert(_mainAudioConfig.end() - 1, 0x570E);

				if (_subAudioConfig[0] >= 0x01)
					_mainAudioConfig.insert(_mainAudioConfig.end() - 1, _subAudioConfig[0] == 0x01 ? _subAudioConfig[0x02] : 0x5716);

				_mainAudioConfig.push_back(0x570F);

				if (_subAudioConfig[0] >= 0x01)
					_mainAudioConfig.push_back(_subAudioConfig[0] == 0x01 ? _subAudioConfig[0x03] : 0x5717);

				_mainAudioConfig.push_back(0x0000);
				_mainAudioConfig.push_back(0x0002);

				_mainAudioConfig[0] += 0x01;

				if (_subAudioConfig[0] >= 0x01)
				{
					uint16_t _fetchBitwise = pow(2, ((_subAudioConfig[2] - 0x570E) / 2) + 0x01);
					_mainAudioConfig.push_back(_subAudioConfig[0] == 0x01 ? _fetchBitwise : 0x0001);

					_mainAudioConfig[0] += 0x01;
				}
			}

			else if (_subAudioConfig[0] >= 0x01)
			{
				if (_subAudioConfig[0] < 0x03)
				{
					for (int i = 0; i < _subAudioConfig[0]; i++)
					{
						_mainAudioConfig.insert(_mainAudioConfig.end() - 1, _subAudioConfig[0x02 + i]);
						_mainAudioConfig[0] += 0x01;
					}

					for (int i = 0; i < _subAudioConfig[0]; i++)
						_mainAudioConfig.push_back(_subAudioConfig[0x02 + (_subAudioConfig[0]) + i]);

					_mainAudioConfig.push_back(0x0000);

					for (int i = 0; i < _subAudioConfig[0]; i++)
						_mainAudioConfig.push_back(pow(2, ((_subAudioConfig[2] - 0x570E) / 2) + 0x01));
				}
			}

			// if (_subAudioConfig[0] >= 0x01)
				// Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _subAudioConfig);

			if (_mainAudioConfig[0] > 0x01)
				Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _mainAudioConfig);

			if (!FindModule("ModuleRF-ShortcutSets.dll"))
			{
				auto _currentTextPtr = YS::MESSAGE::GetData(0x051F);

				auto _soraText = YS::MESSAGE::GetData(0x572E);
				auto _soraSize = YS::MESSAGE::GetSize(_soraText);

				memcpy(const_cast<char*>(_currentTextPtr), _soraText, _soraSize + 0x01);
			}

			IS_INIT = true;
		}


		else
		{
			Tz::HookConfig::Handle();

			ReFined::Demand::TriggerReset();

			ReFined::Critical::ProcessDeath();
			ReFined::Critical::RegisterMagic();
			ReFined::Critical::ShowInformation();
			ReFined::Critical::RegisterMovement();
			ReFined::Critical::RetryBattles();

			ReFined::Critical::HandleIntro();

			if (!IS_NOASPECT)
			ReFined::Critical::AspectCorrection();

			if (DISCORD_ENABLED)
				ReFined::Continuous::DiscordRPC();

			ReFined::Continuous::HandleShake();
			ReFined::Continuous::FixSummonBGM();
			ReFined::Continuous::AutosaveLogic();
			ReFined::Continuous::HandleSaveGlow();
			ReFined::Continuous::EnforcePrompts();
			ReFined::Continuous::ActivateWarpGOA();
			ReFined::Continuous::HandleFrameLimiter();
			ReFined::Continuous::ShowFormEXP();
			ReFined::Continuous::ShowSummonEXP();

			ReFined::Continuous::HotswapMusic();
			ReFined::Continuous::HotswapObjects();

			ReFined::Continuous::EnforceControls();

			for (auto _module : moduleexec)
				_module();
		}
	}
}