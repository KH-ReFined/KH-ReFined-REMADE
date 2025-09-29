#pragma once

#include <cstdint>
#include "SigScan.h"

namespace YS
{
	class CACHE_BUFF
	{
	public:
		using SearchByName_t = char* (*)(const char* fileName, int pri);
		static SearchByName_t SearchByName;
	};
}