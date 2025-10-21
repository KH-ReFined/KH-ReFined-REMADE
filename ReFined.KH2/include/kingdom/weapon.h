#pragma once

#include <cstdint>
#include "SigScan.h"

namespace YS
{
	class WEAPON
	{
	public:
		using Init_t = char*(*)(char* addr, char* objentry, char* partyObject, int part, int hand, int priority);
		static Init_t Init;
	};
}