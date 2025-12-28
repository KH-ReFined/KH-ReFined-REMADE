#pragma once

#include <cstdint>
#include <vector>

#include "information.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "party.h"
#include "menu.h"
#include "message.h"
#include "softreset.h"
#include "sequence.h"
#include "next_form.h"
#include "panacea_alloc.h"

using namespace std;

namespace Tz
{
	class HookConfig
	{

	public:
		static struct Child
		{
			public:
				uint16_t Count;
				uint16_t TitleID;
				vector<uint16_t> ButtonIDs;
				vector<uint16_t> DescriptionIDs;

				Child(uint16_t count, uint16_t titleID, vector<uint16_t> buttonIDs, vector<uint16_t> descriptionIDs);
		};

		static vector<Child> Children;
		static vector<char*> CONFIG_OFFSETS;

		static void Submit();
		static void Handle();

		static void Remove(int Index);
		static void Add(int Index, Child Input);
		static uint8_t Fetch(uint16_t TitleID);
	};
}