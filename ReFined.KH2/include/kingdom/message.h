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
		using GetSize_t = const size_t(*)(const char* message);

		static GetData_t GetData;
		static GetSize_t GetSize;

		static string DecodeKHSCII(const char* Input);
		static vector<char> EncodeKHSCII(string Input);
	};
}
