#pragma once

#include <cstdint>
#include "SigScan.h"

namespace YS
{
	class MEMBER
	{
	public:
		using PartToEntryID_t = uint32_t(*)(uint32_t);
		static PartToEntryID_t PartToEntryID;
	};
}