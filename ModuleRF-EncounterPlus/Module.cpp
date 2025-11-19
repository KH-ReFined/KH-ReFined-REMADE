#include "SigScan.h"

#include "area.h"
#include "title.h"

#include <algorithm>
#include <vector>
#include <random>

using namespace std;

bool ENCOUNTER_ACTIVE;

extern "C"
{
	__declspec(dllexport) void RF_ModuleExecute()
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
				random_device dev;
				mt19937 rng(dev());
				uniform_int_distribution<mt19937::result_type> randDist(1, 100);

				auto _randomChance = randDist(rng);

				if (_randomChance >= 80)
					*(YS::AREA::IsVendor + _vendorOffset) = 0x02;
			}

			ENCOUNTER_ACTIVE = true;
		}

		else if (*YS::AREA::IsInMap && ENCOUNTER_ACTIVE)
			ENCOUNTER_ACTIVE = false;
	}
}