#pragma once

#include <cstdint>
#include "SigScan.h"

namespace YS
{
	class WEAPON_ENTRY
	{
	public:
		using Get_t = uint16_t(*)(int part, uint16_t item);
		static Get_t Get;
	};
}