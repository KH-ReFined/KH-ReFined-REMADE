#pragma once

#include "hardpad.h"

namespace ReFined
{
	class Demand
	{
		public:
			static uint16_t RESET_COMBO;

			static void TriggerReset();
			static void ShortcutSets();
			static void FormKeyLogic();
			static void EncounterPlus();
	};
}