#pragma once

namespace PC
{
	class AXA
	{
	public:
		using DebugPrint_t = void(*)(const char* string);
		static DebugPrint_t DebugPrint;
	};
}