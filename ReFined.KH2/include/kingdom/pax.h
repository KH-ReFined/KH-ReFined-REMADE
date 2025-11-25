#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace ryj
	{
		class DLL_EXPORT PAX
		{
		public:
			using Start_t = void(*)(char* PAX, int EffectID, int Flags, int Type, int Priority);
			static Start_t Start;

			static void Init(char* PAX, char* Data);

			static char* PaxList;
		};
	}
}