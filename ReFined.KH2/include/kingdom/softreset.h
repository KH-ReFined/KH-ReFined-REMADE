#pragma once

#include <cstdint>

namespace dk
{
	class SOFTRESET
	{
	public:
		using SoftResetThread_t = void(*)();
		static SoftResetThread_t SoftResetThread;

		static bool* RESET;
	};
}
