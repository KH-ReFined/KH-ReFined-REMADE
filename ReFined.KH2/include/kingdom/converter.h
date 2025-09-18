#pragma once

#include <cstdint>

namespace PC
{
	class CONVERTER
	{
	public:
		using LONG_TO_INT_ADDRESS_t = uint32_t(*)(uint64_t);
		static LONG_TO_INT_ADDRESS_t LONG_TO_INT_ADDRESS;
	};
}