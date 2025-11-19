#include <cstdint>
#include <map>

#include "area.h"
#include "gauge.h"
#include "title.h"
#include "cache_buff.h"
#include "member_table.h"

#include "SigScan.h"

using namespace std;

bool FACE_APPLIED;
uint8_t PAST_FORM;
uint8_t SORA_CROWN;
uint8_t PAST_CROWN;
char* SORA_PORTRAIT;


map<uint8_t, string> SORA_FORMS
{
	{0x00, "P_EX100"},
	{0x01, "P_EX100_BTLF"},
	{0x02, "P_EX100_MAGF"},
	{0x03, "P_EX100_KH1F"},
	{0x04, "P_EX100_TRIF"},
	{0x05, "P_EX100_ULTF"},
	{0x06, "P_EX100_HTLF"}
};

extern "C"
{
	__declspec(dllexport) void RF_ModuleExecute()
	{
		// Fetch Sora's UCM.
		auto _fetchCharacter = *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable);

		// Fetch the inventory slots for the Proofs.
		auto _fetchCrown = new char[0x03];
		memcpy(_fetchCrown, YS::AREA::SaveData + 0x36B2, 0x03);

		// Add the Proof Counts together and see the result.
		uint8_t _calculateCrown = _fetchCrown[0] + _fetchCrown[1] + _fetchCrown[2];

		// Check if we are in a place that needs crown application.
		auto _areaCheck = YS::AREA::Current->World >= 0x02 && YS::AREA::Current->World != 0x0F;

		// Check if Sora's gauge exists.
		auto _gaugeSora = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

		// If in the title, or the game is loaded, or there was a Drive Form change, or there was a crown change: Reset everything.
		if (*YS::TITLE::IsTitle || !*YS::AREA::IsInMap || PAST_FORM != *(YS::AREA::SaveData + 0x3524) || PAST_CROWN != _calculateCrown)
		{
			FACE_APPLIED = false;
			SORA_PORTRAIT = 0x00;
			PAST_CROWN = _calculateCrown;
			PAST_FORM = *(YS::AREA::SaveData + 0x3524);
		}

		// If not in title, and game's loaded, and the area check passes, and Sora's Portrait is not parsed or reset, and Sora is regular KH2 Sora, and Sora's gauge exists, and the patch is not applied:
		if (!*YS::TITLE::IsTitle && *YS::AREA::IsInMap && _areaCheck && SORA_PORTRAIT == 0x00 && _calculateCrown > 0x00 && _fetchCharacter == 0x0054 && _gaugeSora != 0x00 && !FACE_APPLIED)
		{
			// Fetch Sora's APDX file from the Cache Buffer.
			auto _fileName = "obj/" + SORA_FORMS.at(*(YS::AREA::SaveData + 0x3524)) + ".a.us";
			SORA_PORTRAIT = YS::CACHE_BUFF::SearchByName(const_cast<char*>(_fileName.c_str()), -1);

			// If Sora's APDX does not exist, exit out.
			if (SORA_PORTRAIT == 0x00)
				return;

			// Fetch Sora's PAX Pointer from the APDX file.
			auto _paxPointer = reinterpret_cast<char*>(*reinterpret_cast<const uint64_t*>(SORA_PORTRAIT + 0x58));

			// If it does not exist, exit out.
			if (_paxPointer == 0x00)
				return;

			// Prepare for some calculations.
			uint64_t _readSize = 0x40;

			// Read the BAR offset and the face size.
			auto _barOffset = *reinterpret_cast<const uint32_t*>(_paxPointer + 0x08);
			auto _checkFaceSizes = *reinterpret_cast<const uint32_t*>(_paxPointer + 0x3C);

			// Read all the sizes needed and add them in.
			for (uint32_t i = 0x00; i < 0x02; i++)
				_readSize += *reinterpret_cast<const uint32_t*>(_paxPointer + 0x1CU + (0x10 * i));

			// If the face isn't the size we expect, assume a non-compatible APDX is being used and consider the patch applied.
			if (_checkFaceSizes != 0x04EC && _checkFaceSizes != 0x0000)
			{
				FACE_APPLIED = true;
				return;
			}

			// If the BAR is not initialized, exit out.
			if (_barOffset == 0x00 || _readSize == 0x40)
				return;

			// Fetch the name of a sub-file.
			string _fetchFace(_paxPointer + 0x24, 0x04);

			// If that sub file is not a "face" file, exit out.
			if (_fetchFace != "face")
				return;

			// Calculate the Y coordinates for the SEQD entries based on the current crown.
			short _topValue = 0x00 + _calculateCrown * 0x5A;
			short _bottomValue = 0x5D + _calculateCrown * 0x5A;

			char* _topAddress = _paxPointer + _readSize + 0x38;
			char* _bottomAddress = _paxPointer + _readSize + 0x40;

			auto _readBottom = *reinterpret_cast<const short*>(_bottomAddress);

			// If the bottom address did not read, exit out.
			if (_readBottom == 0x00)
				return;

			// For every face (REGULAR, HURT, CRITICAL), apply the Y coords.
			for (uint32_t i = 0x00; i < 0x03; i++)
			{
				memcpy(_topAddress + 0x2C * i, &_topValue, 0x02);
				memcpy(_bottomAddress + 0x2C * i, &_bottomValue, 0x02);
			}

			// Denote the patch was applied.
			FACE_APPLIED = true;
		}
	}
}