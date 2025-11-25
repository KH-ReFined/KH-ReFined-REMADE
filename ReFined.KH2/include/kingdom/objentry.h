#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJENTRY
		{
		public:
			using GetFriendPriority_t = uint32_t(*)(uint16_t id);
			static GetFriendPriority_t GetFriendPriority;

			using Get_t = char* (*)(uint32_t id);
			static Get_t Get;

			using ReadRequestWeapon_t = char* (*)(int part, int hand, uint16_t id, int pri, int bank);
			static ReadRequestWeapon_t ReadRequestWeapon;
		};
	}
}