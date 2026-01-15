#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <iomanip>
#include <sstream>

#include "information.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "egs.h"
#include "menu.h"
#include "message.h"
#include "event.h"
#include "vsync.h"
#include "sound.h"
#include "world.h"
#include "discord.h"
#include "command_draw.h"
#include "shake.h"
#include "command_elem.h"
#include "item.h"
#include "party.h"
#include "form_level.h"
#include "next_form.h"
#include "region.h"

#include "Continuous.h"
#include "Critical.h"
#include "SigScan.h"
#include <hardpad.h>

using namespace std;

bool IS_SHOWN = false;
bool SYSTEM_LOADED = false;
bool SAVE_INITIATE = false;
bool SYSTEM_WRITTEN = false;

bool ROUND_BACK = false;

YS::AREA::INFO SAVE_AREA;
int SAVE_ITERATOR = 0;

vector<uint32_t> CHECKSUM_TABLE;
vector<char> LIMITER_FUNCTION;

uint8_t IS_STEAM = FindModule("steam_api64.dll");
uint8_t SAVE_CHECK = 0xEB;

discord::Core* Discord;

discord::Activity RICH_PRESENCE;
discord::Timestamp BEGIN_TIMESTAMP;

vector<string> TEXT_PRESENCE;
vector<string> TEXT_MODE;
vector<string> TEXT_FORM;

bool IS_MIRAGE;
bool IS_IN_FORM = false;
bool RPC_ENABLED = true;

uint64_t PAST_EXP_FORM = 0;
uint64_t PAST_EXP_SUMM = 0;

char* SAVE_OFFSET = SignatureScan<char*>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\xC8\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x3F\x48\x8B\xD9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????");

uint8_t* COMMAND_TYPE = ResolveRelativeAddress<uint8_t*>("\x48\x83\xEC\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9\x00\x00\x00\x00\xCC\xCC\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxxx????x????xxx????xxx????xxx????xxxxx????xxxxx????xxx????x", 0x1A);

char* PROMPT_INSTRUCTION = SignatureScan<char*>("\xC7\x05\x00\x00\x00\x00\x01\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00", "xx????xxxxx????xx????");
bool* PROMPT_MODE = ResolveRelativeAddress<bool*>("\x40\x57\x48\x83\xEC\x20\x4C\x8B\x0D\x00\x00\x00\x00\x33\xD2\x4D\x85\xC9\x49\x8D\x81\xA0\x12\x00\x00\x48\x0F\x45\xD0\x4D\x8D\x81\x3C\x02\x00\x00\x48\x85\xD2\x0F\x84\x2B\x01\x00\x00\x33\xC0\x4D\x85\xC9\x49\x0F\x45\xC0\x48\x85\xC0\x74\x16\x48\x63\x82\x00\x02\x00\x00\x48\xC1\xE0\x08\x80\x7C\x10\x3B\x00\x0F\x85\x00\x00\x00\x00\x48\x85\xD2\x0F\x84\xFE\x00\x00\x00\x33\xC0\x4D\x85\xC9\x49\x0F\x45\xC0\x48\x85\xC0\x74\x16", "xxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxx", IS_STEAM ? 0x969 : 0x959);
	
char* VSYNC_SETLIMIT_FUNCTION = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8D\x79\x01\x8B\xD9\x8B\xCF\xE8\x00\x00\x00\x00\xF3\x0F\x10\x0D\x00\x00\x00\x00\x33\xC0\x00\x00\x00\x00\x00\x00\x0F\x57\xC0\x89\x05\x00\x00\x00\x00\x0F\x57\xD2\x83\xFF\x08", "xxxxxxxxxxxxxxxxxx????xxxx????xx??????xxxxx????xxxxxx");

char* ADJUST_GLOW_FUNCTION = SignatureScan<char*>("\x4C\x8B\xDC\x49\x89\x5B\x20\x55\x56\x57\x41\x54\x41\x56\x49\x8D\xAB\x18\xF2\xFF\xFF\x48\x81\xEC\xC0\x0E\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
char* INIT_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x38\xE8\x00\x00\x00\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x0F\x10\x54\x24\x20\xF3\x0F\x10\x48\x10\xF3\x0F\x10\x40\x14\x0F\xC6\xD2\xD2\xF3\x0F\x10\xD1\x0F\xC6\xD2\x27\xF3\x0F\x10\xD0\x0F\xC6\xD2\x39\x0F\x11\x90\x5C\x01\x00\x00\x48\x83\xC4\x38\xC3", "xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* ADJUST_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x78\x0F\x29\x74\x24\x60\x0F\x28\xF1\x0F\x29\x7C\x24\x50\x0F\x28\xFA\x44\x0F\x29\x44\x24\x40\x44\x0F\x28\xC3\x44\x0F\x29\x4C\x24\x30\x44\x0F\x28\xC8\xE8\x00\x00\x00\x00\x45\x0F\xC6\xC9\xE1\xF3\x44\x0F\x10\xCE\x0F\x28\x74\x24\x60\x45\x0F\xC6\xC9\xC6\xF3\x44\x0F\x10\xCF\x0F\x28\x7C\x24\x50\x45\x0F\xC6\xC9\x27\xF3\x45\x0F\x10\xC8\x44\x0F\x28\x44\x24\x40\x45\x0F\xC6\xC9\x39\x44\x0F\x11\x88\x5C\x01\x00\x00\x44\x0F\x28\x4C\x24\x30\x48\x83\xC4\x78\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

vector<char> CALL_FUNC_MUNNY;

vector<char> ADJUST_GLOW_ARRAY;
vector<char> INIT_VIEWPORT_ARRAY;
vector<char> ADJUST_VIEWPORT_ARRAY;

chrono::system_clock::time_point _currentTime = chrono::system_clock::time_point();

bool SHAKE_WRITTEN;

char* VIEWPORT3D = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

uint16_t CURRENT_MUSIC = 0xFFFF;
char* MUSIC_PATH = SignatureScan<char*>("\x62\x67\x6D\x2F\x6D\x75\x73\x69\x63\x25\x30\x33\x64\x2E\x77\x69\x6E\x33\x32\x2E\x73\x63\x64", "xxxxxxxxxxxxxxxxxxxxxxx");

char* FIELD_ALLOC;
char* BATTLE_ALLOC;

bool TRANSFER_FIELD = false;
bool TRANSFER_BATTLE = false;

size_t FETCH_SIZE_FIRST = 0xFFFF;
size_t FETCH_SIZE_SECOND = 0xFFFF;

uint16_t CURRENT_OBJECTS = 0xFFFF;

char* MDLX_PATH = SignatureScan<char*>("\x6F\x62\x6A\x2F\x25\x73\x2E\x6D\x64\x6C\x78\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx");
char* APDX_PATH = SignatureScan<char*>("\x6F\x62\x6A\x2F\x25\x73\x2E\x61\x2E\x25\x73\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx");

char* OBJENTRY_READREQUESTSUB = SignatureScan<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????");
char* OBJENTRY_GETCACHEBUFFSTATUS = SignatureScan<char*>("\x4C\x8D\x46\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????");

char* OBJENTRY_GETMDLX = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x85\xD2\x48\x8D\x79\x08\x48\x8D\x1D\x48", "xxxxxxxxxxxxxxxxxxxxx");

char* BGM_WRITE_BUFFER;

char* MDLX_WRITE_BUFFER = ResolveRelativeAddress<char*>(OBJENTRY_READREQUESTSUB, 0xE);
char* APDX_WRITE_BUFFER = MDLX_WRITE_BUFFER + 0x28;
char* MSET_WRITE_BUFFER = APDX_WRITE_BUFFER + 0x28;

char* MENU_FNAME_BUFFER = ResolveRelativeAddress<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0D);
char* FAC_WRITE_BUFFER = ResolveRelativeAddress<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x", 0x1F9);

uint8_t CONTROL_SCHEME = 0x00;

uint32_t PAST_LOCKON;
bool LOCKON_PLAY = false;

vector<uint8_t> SCHEME_VANILLA;
vector<uint8_t> SCHEME_EDITED;

char* LOCKON_FUNCTION = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8B", "xxxxxxxxxxxxxxxx");
char* LOCKON_FLOATS = SignatureScan<char*>("\x00\x00\x80\xBF\xF3\x04\xB5\xBF\x00\x00\x00\x00\x00\x00\xE0\xBF", "xxxxxxxxxxxxxxxx");

char* LOCKON_CHANGE = ResolveFunctionFromCall<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8B", "xxxxxxxxxxxxxxxx", 0x16A);
uint32_t* LOCKON_TARGET = ResolveRelativeAddress<uint32_t*>(LOCKON_CHANGE, 0x0B);

bool ReFined::Continuous::FIRST_MUSIC_FOUND = false;
bool ReFined::Continuous::SECOND_MUSIC_FOUND = false;

bool ReFined::Continuous::FIRST_RESOURCE_FOUND = false;
bool ReFined::Continuous::SECOND_RESOURCE_FOUND = false;

// Fixes an odd issue where summon animations may cause both field and battle musics to play.
void ReFined::Continuous::FixSummonBGM()
{
	// If the battle status is not peaceful, is in menu, and if a summon is out: Kill the FIELD music.
	if (*YS::AREA::BattleStatus != 0x00 && *YS::MENU::IsMenu && *(YS::AREA::SaveData + 0x3525) != 0x00)
		YS::SOUND::KillBGM(0x00);
}

// Autosave. Plain and simple.
void ReFined::Continuous::AutosaveLogic()
{
	if (CHECKSUM_TABLE.size() == 0x00)
	{
		for (auto x = 0; x <= 0xFF; x++)
		{
			auto r = x << 24;

			for (auto j = 0; j < 0xFF; j++)
				r = r << 1 ^ (r < 0 ? 0x4C11DB7 : 0);

			CHECKSUM_TABLE.push_back(r);
		}
	}

	auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);
	const char* _gaugeTypePointer = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88 });
	const char* _mainPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);

	uint64_t _savePointer = IS_STEAM ? PC::STEAM::pint_saveinformation : PC::EGS::pint_saveinformation;

	if (!SYSTEM_WRITTEN)
	{
		char* _systemInfo = const_cast<char*>(CalculatePointer(_savePointer, { 0x10, 0x10 }));

		const char* _systemText = "BISLPM-66675FM-SYS";
		uint32_t _systemLength = 0x400;

		if (strcmp(_systemInfo, _systemText) != 0x00)
		{
			const auto _currTime = chrono::system_clock::now();
			auto _unixTime = static_cast<time_t>(chrono::duration_cast<chrono::seconds>(_currTime.time_since_epoch()).count());

			memcpy(_systemInfo, _systemText, 0x12);

			memcpy(_systemInfo + 0x40, &_unixTime, 4);
			memcpy(_systemInfo + 0x48, &_unixTime, 4);

			memcpy(_systemInfo + 0x50, &_systemLength, 4);
		}

		SYSTEM_WRITTEN = true;
	}

	if (*YS::AREA::IsInMap && !*YS::TITLE::IsTitle && !SYSTEM_LOADED)
		SYSTEM_LOADED = true;

	else if (*YS::TITLE::IsTitle && SYSTEM_LOADED)
	{
		SAVE_ITERATOR = 0;
		SYSTEM_LOADED = false;
		SAVE_AREA = *YS::AREA::Current;
	}

	if (_commandPointer != 0x00 && _gaugeTypePointer != 0x00)
	{
		bool _checkBlacklist = YS::AREA::Current->World == 0x0F || YS::AREA::Current->World == 0x0B ||
					     	  (YS::AREA::Current->World == 0x08 && YS::AREA::Current->Room == 0x03) ||
							  (YS::AREA::Current->World == 0x0C && YS::AREA::Current->Room == 0x02) ||
							  (YS::AREA::Current->World == 0x02 && YS::AREA::Current->Room <= 0x01) ||
							  (YS::AREA::Current->World == 0x04 && YS::AREA::Current->Room == 0x10) ||
							  (YS::AREA::Current->World == 0x12 && YS::AREA::Current->Room >= 0x13 && YS::AREA::Current->Room <= 0x1D);

		if (!*YS::TITLE::IsTitle && *YS::AREA::IsInMap && !_checkBlacklist)
		{
			if (SAVE_AREA.World == 0x00)
				SAVE_AREA = *YS::AREA::Current;

			bool _isAutosave = (*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x0002) || (*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x0004);
			bool _checkStatus = !*YS::MENU::IsMenu && _commandPointer != 0x00 && *YS::AREA::IsInMap && _mainPointer == 0x00 && *YS::AREA::BattleStatus == 0x00 && SAVE_AREA.World >= 0x02 && SYSTEM_LOADED && _isAutosave && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x00;

			if (!_checkStatus)
			{
				SAVE_INITIATE = false;
				return;
			}

			if (SAVE_AREA.World != YS::AREA::Current->World)
			{
				SAVE_INITIATE = true;
				SAVE_ITERATOR = 0;
			}

			if (SAVE_AREA.Room != YS::AREA::Current->Room)
			{
				SAVE_ITERATOR++;

				if (SAVE_ITERATOR == ROOM_AMOUNT)
				{
					SAVE_INITIATE = true;
					SAVE_ITERATOR = 0;
				}
			}

			SAVE_AREA = *YS::AREA::Current;
		}
	}

	if (SAVE_INITIATE)
	{
		auto _saveOffset = SAVE_SLOT_OFFSET;

		START_FUNC:

		ostringstream _stringStream;
		_stringStream << setw(2) << setfill('0') << (_saveOffset - 1);

		string _saveName = "BISLPM-66675FM-" + _stringStream.str();
		const char* _saveHeader = "KH2J";

		vector<string> _usedSlots;

		char* _saveFilePath = const_cast<char*>(CalculatePointer(_savePointer, { 0x40 }));
		string _saveFileString(_saveFilePath);

		_saveFileString = _saveFileString.append(IS_STEAM ? "\\KHIIFM_WW.png" : "\\KHIIFM.png");

		const auto _currTime = chrono::system_clock::now();
		auto _unixTime = static_cast<time_t>(chrono::duration_cast<chrono::seconds>(_currTime.time_since_epoch()).count());

		uint32_t _saveSlot = 0x00;
		uint32_t _saveHeaderID = 0x3A;
		uint32_t _saveInfoLength = 0x158;
		uint32_t _saveDataLength = 0x10FC0;

		uint32_t _autoSaveTag = 0xFFFFFFFF;
		uint32_t _regularSaveTag = 0x00000000;

		uint32_t _saveInfoStartFILE = 0x1C8;
		uint32_t _saveDataStartFILE = 0x19690;

		char* _saveInfoStartRAM = CalculatePointer(_savePointer, { 0x10, 0x168 });
		char* _saveDataStartRAM = CalculatePointer(_savePointer, { 0x10, 0x19630 });

		memcpy(YS::AREA::SaveData + 0x10, &_autoSaveTag, 0x04);

		const char* _saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);

		for (int i = 0; i < 99; i++)
			if (*(_saveInfoStartRAM + (_saveInfoLength * i)) != 0x00)
				_usedSlots.push_back(string(_saveInfoStartRAM + (_saveInfoLength * i)));

		while (_saveSlotRAM[0] != 0x00 && strcmp(_saveSlotRAM, _saveName.c_str()) != 0x00)
		{
			_saveSlot++;
			_saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
		}

		auto _fetchCheck = *reinterpret_cast<uint32_t*>(_saveDataStartRAM + (_saveDataLength * _saveSlot) + 0x10);

		while (_fetchCheck != _autoSaveTag && _saveSlotRAM[0] != 0x00)
		{
			_saveSlot++;
			_saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
			_fetchCheck = *reinterpret_cast<uint32_t*>(_saveDataStartRAM + (_saveDataLength * _saveSlot) + 0x10);

			while (find(_usedSlots.begin(), _usedSlots.end(), _saveName) != _usedSlots.end())
			{
				_saveOffset--;

				if (_saveOffset == 0x00)
				{
					if (!ROUND_BACK)
					{
						_saveOffset = 99;
						ROUND_BACK = true;
					}

					else
					{
						const char* _unableMessage = YS::MESSAGE::GetData(0x5703);
						dk::INFORMATION::openInformationWindow(_unableMessage);
						ROUND_BACK = false;
						return;
					}
				}

				_stringStream.str("");
				_stringStream << setw(2) << setfill('0') << (_saveOffset - 1);

				_saveName = "BISLPM-66675FM-" + _stringStream.str();
			}

			if (_saveSlot >= 99)
			{
				const char* _unableMessage = YS::MESSAGE::GetData(0x5703);
				dk::INFORMATION::openInformationWindow(_unableMessage);
				return;
			}
		}

		char* _magicData = (char*)malloc(0x08);
		char* _saveData = (char*)malloc(0x10FB4);

		memcpy(_magicData, YS::AREA::SaveData, 0x08);
		memcpy(_saveData, YS::AREA::SaveData + 0x0C, 0x10FB4);

		auto _calculateChecksum = [](uint32_t _startChecksum, char* _dataArray, int _dataLength)
			{
				uint32_t _checksum = _startChecksum;

				for (uint32_t i = 0; i < _dataLength; i++)
					_checksum = CHECKSUM_TABLE[(_checksum >> 24) ^ static_cast<unsigned char>(_dataArray[i])] ^ (_checksum << 8);

				return _checksum ^ 0xFFFFFFFF;
			};

		auto _magicChecksum = _calculateChecksum(0xFFFFFFFF, _magicData, 0x08);
		auto _dataChecksum = _calculateChecksum(_magicChecksum ^ 0xFFFFFFFF, _saveData, 0x10FB4);

		char* _saveInfoAddrRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
		char* _saveDataAddrRAM = _saveDataStartRAM + (_saveDataLength * _saveSlot);

		memcpy(_saveInfoAddrRAM, _saveName.c_str(), 0x11);

		memcpy(_saveInfoAddrRAM + 0x40, &_unixTime, 4);
		memcpy(_saveInfoAddrRAM + 0x48, &_unixTime, 4);

		memcpy(_saveInfoAddrRAM + 0x50, &_saveDataLength, 4);

		memcpy(_saveDataAddrRAM, _saveHeader, 4);

		memcpy(_saveDataAddrRAM + 0x04, &_saveHeaderID, 4);
		memcpy(_saveDataAddrRAM + 0x08, &_dataChecksum, 4);

		memcpy(_saveDataAddrRAM + 0x0c, _saveData, 0x10FB4);

		memcpy(YS::AREA::SaveData + 0x10, &_regularSaveTag, 0x04);

		uint32_t _saveInfoAddr = _saveInfoStartFILE + _saveInfoLength * _saveSlot;
		uint32_t _saveDataAddr = _saveDataStartFILE + _saveDataLength * _saveSlot;

		ofstream _stream(_saveFileString, ios::in | ios::out | ios::binary);

		_stream.seekp(_saveInfoAddr);
		_stream.write(_saveName.c_str(), 0x11);

		_stream.seekp(_saveInfoAddr + 0x40);
		_stream.write(reinterpret_cast<const char*>(&_unixTime), 0x04);
		_stream.seekp(_saveInfoAddr + 0x48);
		_stream.write(reinterpret_cast<const char*>(&_unixTime), 0x04);

		_stream.seekp(_saveInfoAddr + 0x50);
		_stream.write(reinterpret_cast<const char*>(&_saveDataLength), 0x04);

		_stream.seekp(_saveDataAddr);
		_stream.write(_saveHeader, 0x04);

		_stream.seekp(_saveDataAddr + 0x04);
		_stream.write(reinterpret_cast<const char*>(&_saveHeaderID), 0x04);
		_stream.seekp(_saveDataAddr + 0x08);
		_stream.write(reinterpret_cast<const char*>(&_dataChecksum), 0x04);

		_stream.seekp(_saveDataAddr + 0x0C);
		_stream.write(_saveData, 0x10FB4);

		_stream.close();

		free(_saveData);
		free(_magicData);

		if (*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x0004)
		{
			const char* _saveMessage = YS::MESSAGE::GetData(0x5702);
			dk::INFORMATION::openInformationWindow(_saveMessage);
		}

		SAVE_INITIATE = false;
	}
}

void ReFined::Continuous::EnforcePrompts()
{
	bool _isEnforced = *(PROMPT_INSTRUCTION + 0x06) == 0x00 ? true : false;
	bool _fetchConfig = *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x2000;

	if (_isEnforced != _fetchConfig)
	{
		bool _targetMode = _fetchConfig ? 0x00 : 0x01;

		memcpy(PROMPT_INSTRUCTION + 0x06, &_targetMode, 0x01);
		memcpy(PROMPT_MODE, &_targetMode, 0x01);
	}
}

void ReFined::Continuous::ActivateWarpGOA()
{
	auto _gardenKnown = (*(YS::AREA::SaveData + 0x231B) & 0x04) == 0x04;
	auto _canLandGarden = (*(YS::AREA::SaveData + 0x1EF6) & 0x40) == 0x40;

	if (_gardenKnown && !_canLandGarden)
		*(YS::AREA::SaveData + 0x1EF6) += 0x40;
}

void ReFined::Continuous::HandleFrameLimiter()
{					
	auto _limiterOffset = VSYNC_SETLIMIT_FUNCTION + 0x20;

	if (LIMITER_FUNCTION.size() == 0x00)
	{
		LIMITER_FUNCTION.resize(0x06);
		memcpy(LIMITER_FUNCTION.data(), _limiterOffset, 0x06);
	}

	uint8_t _fetchFunction = *_limiterOffset;
	auto _fetchFramerate = IS_STEAM ? *(PC::STEAM::MareConfig + 0x0A) : *(PC::EGS::MareConfig + 0x0A);

	if (_fetchFramerate == 0x00 && _fetchFunction == 0x90)
	{
		memcpy(_limiterOffset, LIMITER_FUNCTION.data(), 0x06);
		*(dk::VSYNC::IsFrameLimited) = 0x01;
	}

	else if (_fetchFramerate != 0x00 && _fetchFunction != 0x90)
	{
		char* _nopArray = new char[0x06];
		fill(_nopArray, _nopArray + 0x06, 0x90);

		memcpy(_limiterOffset, _nopArray, 0x06);
		*(dk::VSYNC::IsFrameLimited) = 0x00;
	}
}

void ReFined::Continuous::DiscordRPC()
{
	int _resultant = 0xFF;

	if (RPC_ENABLED)
	{
		if (Discord == nullptr)
		{
			discord::Core::Create(833511404274974740, DiscordCreateFlags_NoRequireDiscord, &Discord);

			if (Discord == nullptr)
			{
				RPC_ENABLED = false;
				return;
			}

			const auto _currTime = chrono::system_clock::now();
			auto _unixTime = static_cast<time_t>(chrono::duration_cast<chrono::seconds>(_currTime.time_since_epoch()).count());

			BEGIN_TIMESTAMP = _unixTime;

			RICH_PRESENCE.SetApplicationId(833511404274974740);
			RICH_PRESENCE.GetTimestamps().SetStart(BEGIN_TIMESTAMP);

			for (uint16_t i = 0x5740; i < 0x5745; i++)
			{
				auto _msgData = YS::MESSAGE::GetData(i);
				auto _msgConvert = YS::MESSAGE::DecodeKHSCII(_msgData);

				TEXT_PRESENCE.push_back(_msgConvert);
			}

			for (auto i = 0x00; i < 0x04; i++)
			{
				uint16_t _stringID = 0x3738 + i;

				if (i == 0x03)
					_stringID = 0x4E30;

				auto _msgData = YS::MESSAGE::GetData(_stringID);
				auto _msgConvert = YS::MESSAGE::DecodeKHSCII(_msgData);

				TEXT_MODE.push_back(_msgConvert);
			}

			for (auto i = 0x00; i < 0x06; i++)
			{
				uint16_t _stringID = 0x01E5 + (i >= 0x02 ? i - 1 : i);

				if (i == 0x02)
					_stringID = 0x4E7F;

				auto _msgData = YS::MESSAGE::GetData(_stringID);
				auto _msgConvert = YS::MESSAGE::DecodeKHSCII(_msgData);

				TEXT_FORM.push_back(_msgConvert);
			}

			if (YS::FILE::GetSize("mirageArena.bin") != 0x00)
				IS_MIRAGE = true;
		}

		if (*YS::TITLE::IsTitle)
		{
			RICH_PRESENCE.GetAssets().SetLargeImage("title");

			RICH_PRESENCE.SetState("");
			RICH_PRESENCE.SetDetails("");

			RICH_PRESENCE.GetAssets().SetLargeText("");
			RICH_PRESENCE.GetAssets().SetSmallText("");

			RICH_PRESENCE.GetAssets().SetSmallImage("");
		}

		else if (YS::AREA::Current->World >= 0x02 && YS::AREA::Current->World <= 0x12)
		{
			bool _checkUnderdrome = YS::AREA::Current->World == 0x06 && YS::AREA::Current->Room == 0x09 && YS::AREA::Current->Set.Map >= 0xBD && YS::AREA::Current->Set.Map >= 0xC4;

			auto _detailText = _checkUnderdrome ? TEXT_PRESENCE.at(0x02) : TEXT_PRESENCE.at(0x00);

			_detailText.replace(_detailText.find("[0]"), 0x03, to_string(*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308)));
			_detailText.replace(_detailText.find("[1]"), 0x03, *(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x180) > 0x00 ? to_string(*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x180)) : TEXT_PRESENCE.at(0x04));

			if (_checkUnderdrome)
				_detailText.replace(_detailText.find("[2]"), 0x03, to_string(YS::AREA::Current->Entrance));

			RICH_PRESENCE.SetDetails(_detailText.c_str());

			auto _stateText = TEXT_PRESENCE.at(0x01);

			_stateText.replace(_stateText.find("[0]"), 0x03, to_string(*(YS::AREA::SaveData + 0x24FF)));
			_stateText.replace(_stateText.find("[1]"), 0x03, *(YS::AREA::SaveData + 0x3524) == 0x00 ? "N/A" : (*COMMAND_TYPE == 0x01 ? "Mickey" : TEXT_FORM.at(*(YS::AREA::SaveData + 0x3524) - 0x01)));

			RICH_PRESENCE.SetState(_stateText.c_str());

			auto _fetchTime = floorf(*reinterpret_cast<const uint32_t*>(YS::AREA::SaveData + 0x2444) / 60.0F);

			auto _playHours = floorf(_fetchTime / 3600.0F);
			auto _playMinutes = floorf(fmodf(_fetchTime, 3600.0F) / 60.0F);

			ostringstream _timeStream;
			auto _timeText = TEXT_PRESENCE.at(0x03);

			_timeStream << std::setw(2) << std::setfill('0') << _playHours << ":"
				<< std::setw(2) << std::setfill('0') << _playMinutes;

			_timeText.replace(_timeText.find("[0]"), 0x03, _timeStream.str());

			RICH_PRESENCE.GetAssets().SetLargeText(_timeText.c_str());
			RICH_PRESENCE.GetAssets().SetSmallText(TEXT_MODE.at(*(YS::AREA::SaveData + 0x2498)).c_str());

			RICH_PRESENCE.GetAssets().SetSmallImage(*YS::AREA::BattleStatus == 0x00 ? "safe" : (*YS::AREA::BattleStatus == 0x01 ? "mob" : "boss"));
			RICH_PRESENCE.GetAssets().SetLargeImage(IS_MIRAGE && YS::AREA::Current->World == 0x0B ? "ma" : string(YS::WORLD::GetName(YS::AREA::Current->World), 0x02).c_str());
		}

		Discord->ActivityManager().UpdateActivity(RICH_PRESENCE, [&_resultant](discord::Result v) { _resultant = (int)v; });
		Discord->RunCallbacks();
	}
}

void ReFined::Continuous::HandleSaveGlow()
{
	if (*reinterpret_cast<const uint16_t*>(YS::COMMAND_ELEM::ReactionID) == 0x0037 && *YS::AREA::IsInMap)
	{
		auto _statsSlot1 = YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C;
		auto _statsSlot2 = YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C - 0x278;
		auto _statsSlot3 = YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C - 0x4F0;

		uint8_t _healthChecks = 0x00;
		uint8_t _magicChecks = 0x00;

		_healthChecks += *_statsSlot3 == 0x00 ? 0x00 : (*_statsSlot3 != *(_statsSlot3 - 0x04) ? 0x01 : 0x00);
		_healthChecks += *_statsSlot2 == 0x00 ? 0x00 : (*_statsSlot2 != *(_statsSlot2 - 0x04) ? 0x01 : 0x00);
		_healthChecks += *_statsSlot1 == 0x00 ? 0x00 : (*_statsSlot1 != *(_statsSlot1 - 0x04) ? 0x01 : 0x00);

		_magicChecks += *_statsSlot3 == 0x00 ? 0x00 : (*(_statsSlot3 + 0x180) != *(_statsSlot3 + 0x180 - 0x04) ? 0x01 : 0x00);
		_magicChecks += *_statsSlot2 == 0x00 ? 0x00 : (*(_statsSlot2 + 0x180) != *(_statsSlot2 + 0x180 - 0x04) ? 0x01 : 0x00);
		_magicChecks += *_statsSlot1 == 0x00 ? 0x00 : (*(_statsSlot1 + 0x180) != *(_statsSlot1 + 0x180 - 0x04) ? 0x01 : 0x00);

		if (_healthChecks > 0x00 || _magicChecks > 0x00)
			SAVE_CHECK = 0x75;

		else if (SAVE_CHECK == 0x75)
			SAVE_CHECK = 0xEB;

		memcpy(SAVE_OFFSET + 0x25B, &SAVE_CHECK, 0x01);
	}
}

void ReFined::Continuous::HandleShake()
{
	if (ADJUST_GLOW_ARRAY.size() == 0x00)
	{
		ADJUST_GLOW_ARRAY.resize(0x10);
		INIT_VIEWPORT_ARRAY.resize(0x07);
		ADJUST_VIEWPORT_ARRAY.resize(0x08);

		memcpy(ADJUST_GLOW_ARRAY.data(), ADJUST_GLOW_FUNCTION + 0xE6E, 0x10);
		memcpy(INIT_VIEWPORT_ARRAY.data(), INIT_VIEWPORT_FUNCTION + 0x35, 0x07);
		memcpy(ADJUST_VIEWPORT_ARRAY.data(), ADJUST_VIEWPORT_FUNCTION + 0x60, 0x08);
	}

	if (*ryj::SHAKE::ShakeTimer > 0 || *ryj::SHAKE::ShakeCoords > 0x00 || *(ryj::SHAKE::ShakeCoords + 0x02) > 0x00)
	{
		if (!SHAKE_WRITTEN)
		{
			char _nopArray[0x08];
			fill(_nopArray, _nopArray + 0x08, 0x90);

			memcpy(INIT_VIEWPORT_FUNCTION + 0x35, _nopArray, 0x07);
			memcpy(ADJUST_VIEWPORT_FUNCTION + 0x60, _nopArray, 0x08);

			SHAKE_WRITTEN = true;
		}

		float _writeFloat[0x04];

		_writeFloat[0x00] = *ryj::SHAKE::ShakeCoords;
		_writeFloat[0x01] = *(ryj::SHAKE::ShakeCoords + 0x02);
		_writeFloat[0x02] = *(reinterpret_cast<int*>(VIEWPORT3D + 0x08)) + *ryj::SHAKE::ShakeCoords;
		_writeFloat[0x03] = *(reinterpret_cast<int*>(VIEWPORT3D + 0x0C)) + *(ryj::SHAKE::ShakeCoords + 0x02);

		memcpy(VIEWPORT3D + 0x15C, _writeFloat, 0x10);
	}

	else if (SHAKE_WRITTEN)
	{
		memcpy(ADJUST_GLOW_FUNCTION + 0xE6E, ADJUST_GLOW_ARRAY.data(), 0x10);
		memcpy(INIT_VIEWPORT_FUNCTION + 0x35, INIT_VIEWPORT_ARRAY.data(), 0x07);
		memcpy(ADJUST_VIEWPORT_FUNCTION + 0x60, ADJUST_VIEWPORT_ARRAY.data(), 0x08);

		SHAKE_WRITTEN = false;
	}
}

void ReFined::Continuous::ShowFormEXP()
{
	auto _currFormID = *(YS::AREA::SaveData + 0x3524);
	auto _maxFormLevel = 0x02 + YS::ITEM::GetNumBackyard(0x001A) + YS::ITEM::GetNumBackyard(0x001B) + YS::ITEM::GetNumBackyard(0x001D) + YS::ITEM::GetNumBackyard(0x001F) + YS::ITEM::GetNumBackyard(0x0233);

	if (*(YS::AREA::SaveData + 0x3524) != 0x00 && *(YS::AREA::IsInMap) == 1 && *(YS::TITLE::IsTitle) == 0)
	{
		uint8_t _currLevel = *(YS::AREA::SaveData + 0x32F4 + (0x38 * (_currFormID - 1)) + 0x02);

		if (_maxFormLevel == _currLevel && PAST_EXP_FORM != 0x00)
		{
			dk::NEXT_FORM::create(0x00, ReFined::Critical::NEGATIVE_ASPECT_OFFSET);
			PAST_EXP_FORM = 0x00;
		}

		uint32_t _currExp = *reinterpret_cast<uint32_t*>(YS::AREA::SaveData + 0x32F4 + (0x38 * (_currFormID - 1)) + 0x04);
		uint32_t _expFetch = *reinterpret_cast<uint32_t*>(YS::FORM_LEVEL::Search(_currFormID, _currLevel) + 0x04);

		if (PAST_EXP_FORM == 0x00)
			PAST_EXP_FORM = _currExp;

		else if (PAST_EXP_FORM != _currExp)
		{
			dk::NEXT_FORM::create(_expFetch - _currExp, ReFined::Critical::NEGATIVE_ASPECT_OFFSET);
			PAST_EXP_FORM = _currExp;
		}
	}

	else if (*(YS::AREA::IsInMap) == 0 || *(YS::TITLE::IsTitle) == 1 || _currFormID == 0x00)
		PAST_EXP_FORM = 0x00;
}

void ReFined::Continuous::ShowSummonEXP()
{
	auto _surrSummID = *(YS::AREA::SaveData + 0x3525);
	auto _maxSummLevel = 0x02 + YS::ITEM::GetNumBackyard(0x0019) + YS::ITEM::GetNumBackyard(0x017F) + YS::ITEM::GetNumBackyard(0x009F) + YS::ITEM::GetNumBackyard(0x00A0);

	if (_maxSummLevel == 0x06)
		_maxSummLevel = 0x07;

	if (*(YS::AREA::SaveData + 0x3525) != 0x00 && *(YS::AREA::IsInMap) == 1 && *(YS::TITLE::IsTitle) == 0)
	{
		uint8_t _currLevel = *(YS::AREA::SaveData + 0x3526);

		if (_maxSummLevel == _currLevel && PAST_EXP_SUMM != 0x00)
		{
			dk::NEXT_FORM::create(0x00, ReFined::Critical::NEGATIVE_ASPECT_OFFSET);
			PAST_EXP_SUMM = 0x00;
		}

		uint32_t _currExp = *reinterpret_cast<uint32_t*>(YS::AREA::SaveData + 0x36E4);
		uint32_t _expFetch = *reinterpret_cast<uint32_t*>(YS::FORM_LEVEL::GetSummonTable() + 0x04);

		if (PAST_EXP_SUMM == 0x00)
			PAST_EXP_SUMM = _currExp;

		else if (PAST_EXP_SUMM != _currExp)
		{
			dk::NEXT_FORM::create(_expFetch - _currExp, ReFined::Critical::NEGATIVE_ASPECT_OFFSET);
			PAST_EXP_SUMM = _currExp;
		}
	}

	else if (*(YS::AREA::IsInMap) == 0 || *(YS::TITLE::IsTitle) == 1 || _surrSummID == 0x00)
		PAST_EXP_SUMM = 0x00;
}

void ReFined::Continuous::HotswapMusic()
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	auto _fetchMusic = (_fetchConfig & 0x0080) == 0x0080 ? 0x0080 : ((_fetchConfig & 0x0100) == 0x0100 ? 0x0100 : 0x0000);

	if (*YS::TITLE::IsTitle)
			CURRENT_MUSIC = 0xFFFF;

	if (*YS::AREA::IsInMap)
	{
		if (CURRENT_MUSIC == 0xFFFF)
			CURRENT_MUSIC = _fetchMusic;

		else if (CURRENT_MUSIC != _fetchMusic)
		{
			if (_fetchMusic == 0x0080 && !FIRST_MUSIC_FOUND)
			{
				_fetchMusic -= 0x0080;
				_fetchConfig -= 0x0080;

				*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;
			}

			if (_fetchMusic == 0x0100 && !SECOND_MUSIC_FOUND)
			{
				_fetchMusic -= 0x0100;
				_fetchConfig -= 0x0100;

				*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;
			}

			string _fetchPath = _fetchMusic == 0x0080 ? "bgm_2nd/music%03d.win32.scd" : (_fetchMusic == 0x0100 ? "bgm_3rd/music%03d.win32.scd" : "bgm/music%03d.win32.scd");

			auto _fetchMode = *YS::AREA::BattleStatus == 0x00 ? 0x00 : 0x01;

			auto _fetchVolumeStart = *reinterpret_cast<uint32_t*>(YS::SOUND::CurrentMusic + 0x04);
			auto _fetchVolumeFinish = *reinterpret_cast<uint32_t*>(YS::SOUND::CurrentMusic + 0x08);

			auto _fetchCurrentField = *reinterpret_cast<uint16_t*>(YS::SOUND::CurrentMusic);
			auto _fetchCurrentBattle = *reinterpret_cast<uint16_t*>(YS::SOUND::CurrentMusic + 0x10);

			char _fieldMusicPath[0x28];
			char _battleMusicPath[0x28];

			sprintf(_fieldMusicPath, _fetchPath.c_str(), _fetchCurrentField);

			if (YS::FILE::GetSize(_fieldMusicPath) == 0x00)
				sprintf(_fieldMusicPath, "bgm/music%03d.win32.scd", _fetchCurrentField);

			sprintf(_battleMusicPath, _fetchPath.c_str(), _fetchCurrentBattle);

			if (YS::FILE::GetSize(_battleMusicPath) == 0x00)
				sprintf(_battleMusicPath, "bgm/music%03d.win32.scd", _fetchCurrentBattle);

			auto _sizeField = YS::FILE::GetSize(_fieldMusicPath);
			auto _sizeBattle = YS::FILE::GetSize(_battleMusicPath);

			if (TRANSFER_FIELD)
				goto FIELD_AFTERMATH;

			if (TRANSFER_BATTLE)
				goto BATTLE_AFTERMATH;

			FIELD_ALLOC = (char*)malloc(_sizeField);

			if (FIELD_ALLOC != nullptr)
			{
				auto _loadField = YS::FILE::Read(_fieldMusicPath, FIELD_ALLOC);

				if (_fetchMode == 0x00 && _fetchVolumeStart != 0x00)
					YS::SOUND::KillBGM(0x00);

				YS::SOUND::SetTransfer(0x00, 0x02, FIELD_ALLOC, _sizeField, nullptr, nullptr);
				TRANSFER_FIELD = true;
			}

			FIELD_AFTERMATH:

			if (*YS::SOUND::IsTransferActive != 0x00)
				return;

			free(FIELD_ALLOC);
			TRANSFER_FIELD = false;

			if (_fetchMode == 0x00 && _fetchVolumeStart != 0x00)
				YS::SOUND::StartBGM(0x00, 0x3000, 0x3000, 0x00);

			BATTLE_ALLOC = (char*)malloc(_sizeBattle);

			if (BATTLE_ALLOC != nullptr)
			{
				auto _loadBattle = YS::FILE::Read(_battleMusicPath, BATTLE_ALLOC);

				if (_fetchMode == 0x01 && _fetchVolumeStart != 0x00)
					YS::SOUND::KillBGM(0x01);

				YS::SOUND::SetTransfer(0x01, 0x02, BATTLE_ALLOC, _sizeBattle, nullptr, nullptr);
				TRANSFER_BATTLE = true;
			}

			BATTLE_AFTERMATH:

			if (*YS::SOUND::IsTransferActive != 0x00)
				return;

			free(BATTLE_ALLOC);
			TRANSFER_BATTLE = false;

			if (_fetchMode == 0x01 && _fetchVolumeStart != 0x00)
				YS::SOUND::StartBGM(0x01, 0x3000, 0x3000, 0x00);

			CURRENT_MUSIC = _fetchMusic;
		}
	}
	
	else if (!*YS::AREA::IsInMap && !*YS::TITLE::IsTitle && CURRENT_MUSIC != _fetchMusic)
	{
		if (CURRENT_MUSIC == 0xFFFF)
			CURRENT_MUSIC = _fetchMusic;

		else if (_fetchMusic == 0x0080 && !FIRST_MUSIC_FOUND)
			*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;

		if (_fetchMusic == 0x0100 && !SECOND_MUSIC_FOUND)
			*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;

		CURRENT_MUSIC = _fetchMusic;
	}
}

void ReFined::Continuous::HotswapObjects()
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	auto _fetchObject = (_fetchConfig & 0x0200) == 0x0200 ? 0x0200 : ((_fetchConfig & 0x0400) == 0x0400 ? 0x0400 : 0x0000);

	if (!*YS::AREA::IsInMap)
	{
		if (!*YS::TITLE::IsTitle)
		{
			if (CURRENT_OBJECTS == 0xFFFF)
				CURRENT_OBJECTS = _fetchObject;

			if (_fetchObject == 0x0200 && !FIRST_RESOURCE_FOUND)
				*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;

			else if (_fetchObject == 0x0400 && !SECOND_RESOURCE_FOUND)
				*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;
		}

		else
			CURRENT_OBJECTS = 0xFFFF;
	}

	if (*YS::AREA::IsInMap)
	{
		if (CURRENT_OBJECTS == 0xFFFF)
			CURRENT_OBJECTS = _fetchObject;

		if (_fetchObject == 0x0200 && !FIRST_RESOURCE_FOUND)
			*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;

		else if (_fetchObject == 0x0400 && !SECOND_RESOURCE_FOUND)
			*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;

		else if (CURRENT_OBJECTS != _fetchObject && !*YS::MENU::IsMenu)
		{
			YS::AREA::MapJump(YS::AREA::Current, 0x01, 0x00, false);
			CURRENT_OBJECTS = _fetchObject;
		}
	}
}

char* ReFined::Continuous::ConstructBGM(int number)
{
	auto _calcNumber = number;

	if (BGM_WRITE_BUFFER == nullptr)
	BGM_WRITE_BUFFER = (char*)malloc(0x28);

	if (YS::AREA::Current->World == 0x0B)
	{
		_calcNumber = 517;

		if (number != 117)
			_calcNumber = number;

		if (number == 121)
			_calcNumber = 521;
	}

	if (YS::REGION::Get() && YS::REGION::Get() != 0x07 && (_calcNumber <= 3 || _calcNumber == 113))
		_calcNumber += 400;

	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	auto _fetchMusic = (_fetchConfig & 0x0080) == 0x0080 ? 0x0080 : ((_fetchConfig & 0x0100) == 0x0100 ? 0x0100 : 0x0000);

	string _constructPath = _fetchConfig & 0x0080 ? "bgm_2nd/music%03d.win32.scd" : (_fetchConfig & 0x0100 ? "bgm_3rd/music%03d.win32.scd" : "bgm/music%03d.win32.scd");

	sprintf(BGM_WRITE_BUFFER, _constructPath.c_str(), _calcNumber);

	if (YS::FILE::GetSize(BGM_WRITE_BUFFER) == 0x00)
		sprintf(BGM_WRITE_BUFFER, "bgm/music%03d.win32.scd", _calcNumber);

	return BGM_WRITE_BUFFER;
}

char* ReFined::Continuous::ConstructMDLX(char* objentryEntry, char* buff)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.mdlx" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mdlx" : "obj/%s.mdlx");

	char* _mdlxName = objentryEntry + 0x08;
	char* _useBuff = !buff ? MDLX_WRITE_BUFFER : buff;

	sprintf(_useBuff, _constructPath.c_str(), _mdlxName);
	
	if (!YS::FILE::GetSize(_useBuff))
		sprintf(_useBuff, "obj/%s.mdlx", _mdlxName);

	return _useBuff;
}

char* ReFined::Continuous::ConstructAPDX(char* objentryEntry, char* buff)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);

	char* _apdxName = objentryEntry + 0x08;
	char* _useBuff = !buff ? APDX_WRITE_BUFFER : buff;

	const char* _regionStr = !YS::REGION::Get() || YS::REGION::Get() == 0x07 ? "fm" : reinterpret_cast<char*>(*YS::REGION::pint_region);

	string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.a.%s" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.a.%s" : "obj/%s.a.%s");

	if ((*(objentryEntry + 0x48) & 0x01) != 0x00)
		return nullptr;

	sprintf(_useBuff, _constructPath.c_str(), _apdxName, _regionStr);

	if (!YS::FILE::GetSize(_useBuff))
		sprintf(_useBuff, _constructPath.c_str(), _apdxName, "us");

	if (!YS::FILE::GetSize(_useBuff))
	{
		sprintf(_useBuff, "obj/%s.a.%s", _apdxName, _regionStr);

		if (!YS::FILE::GetSize(_useBuff))
			sprintf(_useBuff, "obj/%s.a.us", _apdxName);
	}

	return _useBuff;
}

char* ReFined::Continuous::ConstructMSET(char* objentryEntry, uint32_t objectID, char* buff)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);

	char* _mdlxName = objentryEntry + 0x08;
	char* _msetName = objentryEntry + 0x28;

	char* _useBuff = !buff ? MSET_WRITE_BUFFER : buff;

	string _fetchMSET = _fetchConfig & 0x0200 ? "obj_2nd/%s.mset" :		 (_fetchConfig & 0x0400 ? "obj_3rd/%s.mset" : "obj/%s.mset");
	string _fetchMEMO = _fetchConfig & 0x0200 ? "obj_2nd/%s_MEMO.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s_MEMO.mset" : "obj/%s_MEMO.mset");

	if (!*_msetName)
	{
		if ((objectID & 0x10000000) == 0x00)
			return nullptr;

		sprintf(_useBuff, _fetchMEMO.c_str(), _mdlxName);

		if (!YS::FILE::GetSize(_useBuff))
			sprintf(_useBuff, "obj/%s_MEMO.mset", _mdlxName);

		return _useBuff;
	}

	auto _fetchNameMSET = string(_msetName);
	_fetchNameMSET.resize(_fetchNameMSET.size() - 0x05);

	sprintf(_useBuff, _fetchMSET.c_str(), _fetchNameMSET.c_str());

	if (!YS::FILE::GetSize(_useBuff))
		sprintf(_useBuff, "obj/%s.mset", _fetchNameMSET.c_str());

	if ((objectID & 0x10000000) != 0x00)
	{
		LABEL_12:
		sprintf(_useBuff, _fetchMEMO.c_str(), _fetchNameMSET.c_str());

		if (!YS::FILE::GetSize(_useBuff))
			sprintf(_useBuff, "obj/%s_MEMO.mset", _fetchNameMSET.c_str());

		return _useBuff;
	}

	if (objectID <= 0x31A)
	{
		if (objectID > 0x318)
			goto LABEL_10;

		LABEL_9:
		if ((objectID & 0x20000000) == 0x00)
			return _useBuff;

		goto LABEL_10;
	}

	if (objectID != 0x03EE)
		goto LABEL_9;

	LABEL_10:
	if (*(objentryEntry + 0x04) != 0x00 && YS::CACHE_BUFF::GetStatus(_useBuff) < 2)
		goto LABEL_12;

	return _useBuff;
}

char* ReFined::Continuous::ConstructMENU(char* buff, char* fileName)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	auto _regionPointer = (!YS::REGION::Get() || YS::REGION::Get() == 7) ? reinterpret_cast<char*>(*YS::REGION::pint_region_default) : reinterpret_cast<char*>(*YS::REGION::pint_region);

	if (!strcmp(MENU_FNAME_BUFFER, fileName))
	{
		string _constructFile = _fetchConfig & 0x0200 ? "file_2nd/%s/%s" : (_fetchConfig & 0x0400 ? "file_3rd/%s/%s" : "file/%s/%s");

		sprintf(buff, _constructFile.c_str(), _regionPointer, fileName);

		if (!YS::FILE::GetSize(buff))
		{
			_constructFile.resize(_constructFile.size() - 0x03);
			sprintf(buff, _constructFile.c_str(), fileName);

			if (!YS::FILE::GetSize(buff))
			{
				sprintf(buff, "file/%s/%s", _regionPointer, fileName);

				if (!YS::FILE::GetSize(buff))
					sprintf(buff, "file/%s", fileName);
			}
		}

		return buff;
	}

	else
	{
		string _constructMenu = _fetchConfig & 0x0200 ? "menu_2nd/%s/%s" : (_fetchConfig & 0x0400 ? "menu_3rd/%s/%s" : "menu/%s/%s");

		auto _checkPhoto = strstr(fileName, "jm_photo/");
		sprintf(buff, _constructMenu.c_str(), _regionPointer, fileName);

		if (!YS::FILE::GetSize(buff))
		{
			if (_checkPhoto)
				goto SKIP_PHOTO;

			_constructMenu.resize(_constructMenu.size() - 0x03);
			sprintf(buff, _constructMenu.c_str(), fileName);

			if (!YS::FILE::GetSize(buff))
			{
				SKIP_PHOTO:
				sprintf(buff, "menu/%s/%s", _regionPointer, fileName);

				if (!YS::FILE::GetSize(buff) && !_checkPhoto)
					sprintf(buff, "menu/%s", fileName);
			}
		}

		return buff;
	}
}

void ReFined::Continuous::ConstructFAC(uint16_t id)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	auto _writeBuff = *reinterpret_cast<char**>(FAC_WRITE_BUFFER);

	string _fetchFilePath(_writeBuff);

	_fetchFilePath = _fetchFilePath.substr(0x04, _fetchFilePath.size() - 0x04);
	_fetchFilePath = _fetchFilePath.substr(0x00, _fetchFilePath.size() - 0x04);

	string _constructPath = _fetchConfig & 0x0200 ? "menu_2nd" : (_fetchConfig & 0x0400 ? "menu_3rd" : "menu");

	_constructPath = _constructPath.append(_fetchFilePath);
	_constructPath = _constructPath.append("%d.imd");

	sprintf(_writeBuff, _constructPath.c_str(), id);

	if (!YS::FILE::GetSize(_writeBuff))
	{
		_constructPath = "menu";

		_constructPath = _constructPath.append(_fetchFilePath);
		_constructPath = _constructPath.append("%d.imd");

		sprintf(_writeBuff, _constructPath.c_str(), id);
	}
}

void ReFined::Continuous::ConstructITEMPIC(char* buff, uint16_t id)
{
	auto _fetchPicturePtr = *YS::ITEMPIC::LoadedId;
	auto _fetchPictureID = 0x00;

	if (_fetchPicturePtr != nullptr)
		_fetchPictureID = *_fetchPicturePtr;

	else
		_fetchPictureID = id;

	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
	string _constructPath = _fetchConfig & 0x0200 ? "itempic_2nd/item-%03d.imd" : (_fetchConfig & 0x0400 ? "itempic_3rd/item-%03d.imd" : "itempic/item-%03d.imd");

	sprintf(buff, _constructPath.c_str(), _fetchPictureID);

	if (!YS::FILE::GetSize(buff))
		sprintf(buff, "itempic/item-%03d.imd", _fetchPictureID);
}

void ReFined::Continuous::EnforceControls()
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A4);
	auto _fetchControl = (_fetchConfig & 0x4000) == 0x4000 ? 0x00 : ((_fetchConfig & 0x8000) == 0x8000 ? 0x01 : 0x02);

	auto _fetchChange = *(YS::HARDPAD::Input - 0x02);
	auto _fetchTarget = *LOCKON_TARGET;

	if (SCHEME_VANILLA.size() == 0x00)
	{
		SCHEME_EDITED =
		{
			0x41, 0x8B, 0x06, // mov eax, [r14]
			0x66, 0xA9, 0x00, 0x01, // test ax, 0x0100
			0x74, 0x0A, // je 0x0A
			0xF3, 0x0F, 0x10, 0x1D, 0x00, 0x00, 0x00, 0x00, // movss xmm3, [_floatCalcFirst]
			0xEB, 0x19, // jmp 0x19
			0x66, 0xA9, 0x00, 0x02, // test ax, 0x0200
			0x75, 0x0B, // jne 0x0B
			0x41, 0xC7, 0x46, 0xFC, 0x00, 0x00, 0x00, 0x00, // mov [r14 - 0x04], 0x00
			0xEB, 0x59, // jmp 0x59
			0x90, // nop
			0xF3, 0x0F, 0x10, 0x1D, 0x00, 0x00, 0x00, 0x00, // movss xmm3, [_floatCalcSecond]
			0x41, 0x8B, 0x4E, 0xFC, // mov ecx, [r14 - 0x04]
			0x85, 0xC9, //test ecx, ecx
			0x75, 0x48, // jne 0x48
			0x41, 0xFF, 0x46, 0xFC // inc [r14 - 0x04]
		};

		SCHEME_VANILLA.resize(SCHEME_EDITED.size());
		memcpy(SCHEME_VANILLA.data(), LOCKON_FUNCTION + 0xF3, SCHEME_EDITED.size());
	}

	if (_fetchControl != CONTROL_SCHEME)
	{
		if (_fetchControl == 0x02)
			memcpy(LOCKON_FUNCTION + 0xF3, SCHEME_VANILLA.data(), SCHEME_VANILLA.size());

		else
		{
			uint32_t _floatCalcFirst = LOCKON_FLOATS - (LOCKON_FUNCTION + 0x104) - (_fetchControl == 0x00 ? 0x00 : 0x1C8);
			uint32_t _floatCalcSecond = (LOCKON_FLOATS - 0x1C8) - (LOCKON_FUNCTION + 0x11F) + (_fetchControl == 0x00 ? 0x00 : 0x1C8);

			memcpy(LOCKON_FUNCTION + 0xF3, SCHEME_EDITED.data(), SCHEME_EDITED.size());

			memcpy(LOCKON_FUNCTION + 0x100, &_floatCalcFirst, 0x04);
			memcpy(LOCKON_FUNCTION + 0x11B, &_floatCalcSecond, 0x04);
		}

		CONTROL_SCHEME = _fetchControl;
	}

	if (PAST_LOCKON != _fetchTarget && !LOCKON_PLAY)
	{
		YS::SOUND::PlaySFX(7);
		LOCKON_PLAY = true;
	}

	else if (LOCKON_PLAY)
		LOCKON_PLAY = false;

	PAST_LOCKON = _fetchTarget;
}