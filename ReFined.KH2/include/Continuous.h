#pragma once

namespace ReFined
{
	class Continuous
	{
		public:
			static uint8_t SAVE_SLOT_OFFSET;
			static uint8_t ROOM_AMOUNT;

			static void FixSummonBGM();
			static void AutosaveLogic();
			static void EnforcePrompts();
			static void DiscordRPC();
			static void ActivateWarpGOA();
			static void HandleFrameLimiter();
			static void HandleSaveGlow();
			static void ShowFormEXP();
			static void HandleShake();
	};
}