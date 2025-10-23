#pragma once

#include <cstdint>
#include <Windows.h>
#include "SigScan.h"

namespace YS
{
	class PARTY
	{
	public:
		using SetWeapon_t = void(*)(char* party_ptr, int entry_id, int hand);
		static SetWeapon_t SetWeapon;

		// In all honesty, reimplementing this seemed like the far more stable option.
		static void ChangeWeapon(int part, bool hand_secondary, int item);
	};
}