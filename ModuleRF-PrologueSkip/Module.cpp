#include <cstdint>

#include "area.h"
#include "event.h"
#include "title.h"
#include "file.h"
#include "member_table.h"

#include "SigScan.h"

using namespace std;

uint8_t ROXAS_SKIP_STAGE;
bool SKIPPING_ROXAS;

extern "C"
{
	__declspec(dllexport) bool* INTRO_SEEK;

	__declspec(dllexport) uint32_t* RF_CheckIntro()
	{
		char* _allocMemory = (char*)malloc(0x07 * 0x04);
		uint32_t _tempInt[] = { 0x02, 0x5730, 0xFFFF, 0x5738, 0x5739, 0x5731, 0x5732 };

		memcpy(_allocMemory, _tempInt, 0x07 * 0x04);
		return reinterpret_cast<uint32_t*>(_allocMemory);
	}

	__declspec(dllexport) void RF_ModuleExecute()
	{
		if (*YS::TITLE::IsTitle)
		{
			if (*YS::TITLE::IntroSelect != 0x00 && ROXAS_SKIP_STAGE == 0x00)
				ROXAS_SKIP_STAGE = 0x02;

			else if (*YS::TITLE::IntroSelect == 0x00 && ROXAS_SKIP_STAGE != 0x00)
				ROXAS_SKIP_STAGE = 0x00;
		}

		if (!*YS::TITLE::IsTitle)
		{
			YS::AREA::INFO _constArea;
			const char* _eventPointer = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });

			if (YS::AREA::Current->World == 0x02 && YS::AREA::Current->Room == 0x01 && YS::AREA::Current->Set.Map == 0x38 && ROXAS_SKIP_STAGE == 0x00)
			{
				if (*INTRO_SEEK)
				{
					_constArea.World = 0x02;
					_constArea.Room = 0x20;
					_constArea.Entrance = 0x32;

					_constArea.Set.Map = 0x01;
					_constArea.Set.Event = 0x01;

					uint32_t _writeFlag = 0x00;
					memcpy(YS::AREA::SaveData + 0x1CD4, &_writeFlag, 0x04);

					_writeFlag = 0x1FF00001;
					memcpy(YS::AREA::SaveData + 0x1CD0, &_writeFlag, 0x04);

					YS::AREA::MapJump(&_constArea, 0x01, 0, false);
					ROXAS_SKIP_STAGE = 0x01;
				}

				else
					ROXAS_SKIP_STAGE = 0x02;
			}

			if (YS::AREA::Current->World == 0x02 && YS::AREA::Current->Room == 0x20 && YS::AREA::Current->Set.Map == 0x01 && ROXAS_SKIP_STAGE == 0x01 && _eventPointer != 0x00)
			{
				size_t _fetchSize = YS::FILE::GetSize("00prologue.bin");
				auto _loadBAR = (char*)malloc(_fetchSize);

				uint8_t _fetchWeapon = *reinterpret_cast<const uint8_t*>(YS::AREA::SaveData + 0x24FE);
				uint32_t _fetchConfig = *reinterpret_cast<const uint32_t*>(YS::AREA::SaveData + 0x41A4);

				uint8_t _fetchDifficulty = *reinterpret_cast<const uint8_t*>(YS::AREA::SaveData + 0x2498);

				if (YS::FILE::LoadBAR("00prologue.bin", _loadBAR) != 0x00)
				{
					memcpy(YS::AREA::SaveData + 0x031C, _loadBAR, 0x4D);
					memcpy(YS::AREA::SaveData + 0x03E8, _loadBAR + 0x4D, 0x01);
					memcpy(YS::AREA::SaveData + 0x03EE, _loadBAR + 0x4E, 0x01);
					memcpy(YS::AREA::SaveData + 0x1CE2, _loadBAR + 0x4F, 0x01);
					memcpy(YS::AREA::SaveData + 0x1CD0, _loadBAR + 0x70, 0x20);
					memcpy(YS::AREA::SaveData + 0x20E4, _loadBAR + 0x90, _fetchSize - 0x80);

					if (_fetchDifficulty == 0x03)
					{
						memcpy(YS::AREA::SaveData + 0x2544, _loadBAR + 0x50, 0x14);
						memcpy(YS::AREA::SaveData + 0x24F8, _loadBAR + 0x66, 0x01);

						memcpy(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C, _loadBAR + 0x64, 0x01);
					}

					else
					{
						memcpy(YS::AREA::SaveData + 0x2544, _loadBAR + 0x5E, 0x06);
						memcpy(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C, _loadBAR + 0x68, 0x01);
					}

					memcpy(YS::AREA::SaveData + 0x24FE, &_fetchWeapon, 0x01);
					memcpy(YS::AREA::SaveData + 0x2498, &_fetchDifficulty, 0x01);

					memcpy(YS::AREA::SaveData + 0x41A4, &_fetchConfig, 0x02);
					memcpy(YS::AREA::SaveData + 0x4270, _loadBAR + 0x6E, 0x02);

					YS::AREA::Current->Room = 0x0E;
					YS::AREA::Current->Set.Map = 0x02;
					YS::AREA::Current->Set.Event = 0x12;

					YS::AREA::MapJump(YS::AREA::Current, 0x01, 0, false);
					ROXAS_SKIP_STAGE = 0x02;

					free(_loadBAR);
				}
			}
		}
	}
}