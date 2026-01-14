#pragma once

namespace ReFined
{
	class Continuous
	{
		public:
			static uint8_t SAVE_SLOT_OFFSET;
			static uint8_t ROOM_AMOUNT;

			static bool FIRST_MUSIC_FOUND;
			static bool SECOND_MUSIC_FOUND;

			static bool FIRST_RESOURCE_FOUND;
			static bool SECOND_RESOURCE_FOUND;

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

			static char* ConstructBGM(int number);
			static char* ConstructMENU(char* buff, char* fileName);
			static char* ConstructMDLX(char* objentryEntry, char* buff);
			static char* ConstructAPDX(char* objentryEntry, char* buff);
			static char* ConstructMSET(char* objentryEntry, uint32_t objectID, char* buff);

			static void ConstructFAC(uint16_t id);
			static void ConstructITEMPIC(char* buff, uint16_t id);
	};
}