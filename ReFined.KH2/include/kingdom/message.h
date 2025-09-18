#pragma once

#include <cstdint>
#include <map>
#include <sstream>

using namespace std;

namespace YS
{
	class MESSAGE
	{
	public:
		using GetData_t = const char*(*)(int id);
		static GetData_t GetData;

		static string DecodeKHSCII(const char* Input);
	};
}
