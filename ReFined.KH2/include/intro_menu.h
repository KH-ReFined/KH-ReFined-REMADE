#pragma once

#include <cstdint>
#include <vector>

#include "memory_manager.h"

using namespace std;

namespace ReFined
{
	class IntroMenu
	{

	public:
		static struct Entry
		{
		public:
			uint32_t Count;
			uint32_t FlairID;
			uint32_t TitleID;
			vector<uint32_t> ButtonIDs;
			vector<uint32_t> DescriptionIDs;

			Entry(uint32_t count, uint32_t flairID, uint32_t titleID, vector<uint32_t> buttonIDs, vector<uint32_t> descriptionIDs);
		};

		static vector<char*> INTRO_OFFSETS;

		static vector<Entry> Children;

		static void Submit();

		static void Add(int Index, Entry Input);
		static void Remove(int Index);
	};
}