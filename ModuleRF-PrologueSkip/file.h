#pragma once

#include <cstdint>

namespace YS
{
	class FILE
	{
	public:
		using GetSize_t = size_t(*)(const char* fileName);
		static GetSize_t GetSize;

		using LoadBAR_t = size_t(*)(const char* fileName, char* address);
		static LoadBAR_t LoadBAR;
	};
}