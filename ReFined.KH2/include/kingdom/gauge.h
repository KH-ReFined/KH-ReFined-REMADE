#pragma once

#include <cstdint>

namespace dk
{
	class GAUGE
	{
		public:
			using Init_t = void(*)();
			static Init_t Init;

			static uint64_t pint_playergauge;
			static uint64_t pint_friendgauge;
	};
}