#pragma once
#pragma once

#include <cstdint>
#include <Windows.h>

namespace PC
{
	class STEAM
	{
	public:

		using CheckSaveFile_t = void(*)();
		static CheckSaveFile_t CheckSaveFile;

		using FetchMareConfig_t = void(*)();
		static FetchMareConfig_t FetchMareConfig;

		static char* MareConfig;
		static uint64_t pint_saveinformation;
	};
}