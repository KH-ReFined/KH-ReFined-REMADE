#pragma once

#include <cstdint>
#include "SigScan.h"

namespace Tz
{
	class CmConfig
	{
	public:
		using UpdateList_t = void (*)();
		static UpdateList_t UpdateList;

		using UpdateActive_t = void (*)();
		static UpdateActive_t UpdateActive;
	};
}