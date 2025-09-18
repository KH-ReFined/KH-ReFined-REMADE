#pragma once

#include <stdint.h>
#include "SigScan.h"

namespace YS
{
	class SORA
	{
	public:
		using AddHP_t = void(*)(char* playerObject, int value, int parts, bool is_voice);
		static AddHP_t AddHP;

		using RefreshAbilities_t = void(*)(char* playerStats);
		static RefreshAbilities_t RefreshAbilities;

		static uint64_t pint_sora;
	};
}