#pragma once

#include <cstdint>
#include <vector>

#include "memory_manager.h"

using namespace std;

namespace ReFined
{
	class ConfigMenu
	{

		public:
			static struct Entry
			{
			public:
				uint16_t Count;
				uint16_t TitleID;
				vector<uint16_t> ButtonIDs;
				vector<uint16_t> DescriptionIDs;

				Entry(uint16_t count, uint16_t titleID, vector<uint16_t> buttonIDs, vector<uint16_t> descriptionIDs);
			};

			static vector<char*> CONFIG_OFFSETS;

			static vector<Entry> Children;

			static void Submit();

			static void Add(int Index, Entry Input);
			static void Remove(int Index);
	};
}