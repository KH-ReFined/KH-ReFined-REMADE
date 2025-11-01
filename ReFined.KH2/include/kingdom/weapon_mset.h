#pragma once

#include <cstdint>
#include "SigScan.h"

namespace YS
{
	class WEAPON_MSET
	{
	public:
		using GetFilename_t = char*(*)(int part, int hand, char* buff);
		static GetFilename_t GetFilename;
	};
}