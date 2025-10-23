#pragma once

#include <cstdint>

namespace ryj
{
	class PAX
	{
	public:
		using Start_t = void(*)(char* PAX, int EffectID, int Flags, int Type, int Priority);
		static Start_t Start;

		static void Init(char* PAX, char* Data);

		static char* PaxList;
	};
}