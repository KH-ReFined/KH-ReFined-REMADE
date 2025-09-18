#pragma once

#include <cstdint>
#include <Windows.h>

namespace YS
{
	class WORLD
	{
	public:
		using GetName_t = char*(*)(int world);
		static GetName_t GetName;
	};
}