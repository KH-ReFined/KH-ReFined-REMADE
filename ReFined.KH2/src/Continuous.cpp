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
#include "party.h"
#include <chrono>

#include "Continuous.h"
#include "Critical.h"
#include "SigScan.h"
#include <hardpad.h>

using namespace std;

bool IS_SHOWN = false;
bool SYSTEM_LOADED = false;
bool SAVE_INITIATE = false;
bool SYSTEM_WRITTEN = false;

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
bool RPC_ENABLED = true;

char* SAVE_OFFSET = SignatureScan<char*>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\xC8\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x3F\x48\x8B\xD9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????");

uint8_t* COMMAND_TYPE = ResolveRelativeAddress<uint8_t*>("\x48\x83\xEC\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9\x00\x00\x00\x00\xCC\xCC\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxxx????x????xxx????xxx????xxx????xxxxx????xxxxx????xxx????x", 0x1A);

char* PROMPT_INSTRUCTION = SignatureScan<char*>("\xC7\x05\x00\x00\x00\x00\x01\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00", "xx????xxxxx????xx????");
bool* PROMPT_MODE = ResolveRelativeAddress<bool*>("\x40\x57\x48\x83\xEC\x20\x4C\x8B\x0D\x00\x00\x00\x00\x33\xD2\x4D\x85\xC9\x49\x8D\x81\xA0\x12\x00\x00\x48\x0F\x45\xD0\x4D\x8D\x81\x3C\x02\x00\x00\x48\x85\xD2\x0F\x84\x2B\x01\x00\x00\x33\xC0\x4D\x85\xC9\x49\x0F\x45\xC0\x48\x85\xC0\x74\x16\x48\x63\x82\x00\x02\x00\x00\x48\xC1\xE0\x08\x80\x7C\x10\x3B\x00\x0F\x85\x00\x00\x00\x00\x48\x85\xD2\x0F\x84\xFE\x00\x00\x00\x33\xC0\x4D\x85\xC9\x49\x0F\x45\xC0\x48\x85\xC0\x74\x16", "xxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxx", IS_STEAM ? 0x969 : 0x959);
	
char* VSYNC_SETLIMIT_FUNCTION = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8D\x79\x01\x8B\xD9\x8B\xCF\xE8\x00\x00\x00\x00\xF3\x0F\x10\x0D\x00\x00\x00\x00\x33\xC0\x00\x00\x00\x00\x00\x00\x0F\x57\xC0\x89\x05\x00\x00\x00\x00\x0F\x57\xD2\x83\xFF\x08", "xxxxxxxxxxxxxxxxxx????xxxx????xx??????xxxxx????xxxxxx");

char* ADJUST_GLOW_FUNCTION = SignatureScan<char*>("\x4C\x8B\xDC\x49\x89\x5B\x20\x55\x56\x57\x41\x54\x41\x56\x49\x8D\xAB\x18\xF2\xFF\xFF\x48\x81\xEC\xC0\x0E\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
char* INIT_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x38\xE8\x00\x00\x00\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x0F\x10\x54\x24\x20\xF3\x0F\x10\x48\x10\xF3\x0F\x10\x40\x14\x0F\xC6\xD2\xD2\xF3\x0F\x10\xD1\x0F\xC6\xD2\x27\xF3\x0F\x10\xD0\x0F\xC6\xD2\x39\x0F\x11\x90\x5C\x01\x00\x00\x48\x83\xC4\x38\xC3", "xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* ADJUST_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x78\x0F\x29\x74\x24\x60\x0F\x28\xF1\x0F\x29\x7C\x24\x50\x0F\x28\xFA\x44\x0F\x29\x44\x24\x40\x44\x0F\x28\xC3\x44\x0F\x29\x4C\x24\x30\x44\x0F\x28\xC8\xE8\x00\x00\x00\x00\x45\x0F\xC6\xC9\xE1\xF3\x44\x0F\x10\xCE\x0F\x28\x74\x24\x60\x45\x0F\xC6\xC9\xC6\xF3\x44\x0F\x10\xCF\x0F\x28\x7C\x24\x50\x45\x0F\xC6\xC9\x27\xF3\x45\x0F\x10\xC8\x44\x0F\x28\x44\x24\x40\x45\x0F\xC6\xC9\x39\x44\x0F\x11\x88\x5C\x01\x00\x00\x44\x0F\x28\x4C\x24\x30\x48\x83\xC4\x78\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

vector<char> ADJUST_GLOW_ARRAY;
vector<char> INIT_VIEWPORT_ARRAY;
vector<char> ADJUST_VIEWPORT_ARRAY;

chrono::system_clock::time_point _currentTime = chrono::system_clock::time_point();

bool SHAKE_WRITTEN;

char* VIEWPORT3D = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

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

			bool _checkStatus = !*YS::MENU::IsMenu && _commandPointer != 0x00 && *YS::AREA::IsInMap && _mainPointer == 0x00 && *YS::AREA::BattleStatus == 0x00 && SAVE_AREA.World >= 0x02 && SYSTEM_LOADED && ReFined::Critical::SAVE_MODE != 0x02 && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x00;

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

				if (SAVE_ITERATOR == 0x03)
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
		const char* _saveName = "BISLPM-66675FM-98";
		const char* _saveHeader = "KH2J";

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

		const char* _saveInfoStartRAM = CalculatePointer(_savePointer, { 0x10, 0x168 });
		const char* _saveDataStartRAM = CalculatePointer(_savePointer, { 0x10, 0x19630 });

		memcpy(YS::AREA::SaveData + 0x10, &_autoSaveTag, 0x04);

		const char* _saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);

		while (_saveSlotRAM[0] != 0x00 && strcmp(_saveSlotRAM, _saveName) != 0x00)
		{
			_saveSlot++;
			_saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
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

		char* _saveInfoAddrRAM = const_cast<char*>(_saveInfoStartRAM) + (_saveInfoLength * _saveSlot);
		char* _saveDataAddrRAM = const_cast<char*>(_saveDataStartRAM) + (_saveDataLength * _saveSlot);

		memcpy(_saveInfoAddrRAM, _saveName, 0x11);

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
		_stream.write(_saveName, 0x11);

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

		if (ReFined::Critical::SAVE_MODE == 0x00)
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

	if (_isEnforced != ReFined::Critical::CONTROLLER_MODE)
	{
		bool _targetMode = ReFined::Critical::CONTROLLER_MODE ? 0x00 : 0x01;

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

void ReFined::Continuous::Autoattack()
{
	auto _pointAction = CalculatePointer(YS::COMMAND_ELEM::CommandElem, { 0x0A });

	auto _currAction = *reinterpret_cast<const uint32_t*>(_pointAction);
	auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);
	auto _currChildMenu = *reinterpret_cast<const uint64_t*>(YS::COMMAND_DRAW::pint_commandmenu + 0x08);

	auto _confirmConfig = *(IS_STEAM ? PC::STEAM::MareConfig + 0x1E : PC::EGS::MareConfig + 0x1E);
	auto _confirmButton = _confirmConfig == 0x00 ? 0x4000 : 0x2000;

	if (!*YS::TITLE::IsTitle && _commandPointer != 0x00)
	{
		auto _currCommand = *reinterpret_cast<const uint8_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x74 }));
		auto _mainMenuType = *reinterpret_cast<const uint8_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x00 }));
		auto _firstCommand = *reinterpret_cast<const uint16_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x16 }));
		auto _commandFlag = *reinterpret_cast<const uint8_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x19 }));

		bool _isActionGood = _currChildMenu == 0x00 && _currCommand == 0x00;
		bool _isCommandGood = (_firstCommand == 0x0001 || _firstCommand == 0x016D || _firstCommand == 0x0088) && (_commandFlag == 0x00) && (_mainMenuType == 0x00 || _mainMenuType == 0x06);

		auto _fetchButtons = *YS::HARDPAD::Input;

		if (*YS::AREA::BattleStatus != 0x00 && _isCommandGood && _isActionGood && (_fetchButtons & _confirmButton) == _confirmButton && _currAction == 0x00)
		{
			_currAction = 0x01;
			memcpy(const_cast<char*>(_pointAction), &_currAction, 0x04);
		}

		else if (_currAction == 0x01)
		{
			_currAction = 0x00;
			memcpy(const_cast<char*>(_pointAction), &_currAction, 0x04);
		}
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
