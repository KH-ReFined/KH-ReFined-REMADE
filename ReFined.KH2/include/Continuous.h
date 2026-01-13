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
			static void ShowSummonEXP();
			static void HandleShake();
			static void HotswapMusic();
			static void HotswapObjects();
			static void EnforceControls();

			static char* VerifyMDLX(char* objentryEntry, char* buff);
			static char* VerifyAPDX(char* objentryEntry, char* buff);

			static char* VerifyMSET(char* objentryEntry, uint32_t objectID, char* buff);

			static void VerifyITEMPIC(char* buff, uint16_t id);
	};
}