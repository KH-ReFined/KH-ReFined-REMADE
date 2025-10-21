#pragma once

#include <cstdint>
#include "SigScan.h"

namespace YS
{
	class OBJENTRY
	{
	public:
		using GetFriendPriority_t = uint32_t(*)(uint16_t id);
		static GetFriendPriority_t GetFriendPriority;

		using Get_t = char*(*)(uint32_t id);
		static Get_t Get;

		using ReadRequestWeapon_t = char*(*)(int part, int hand, uint16_t id, int pri, int bank);
		static ReadRequestWeapon_t ReadRequestWeapon;
	};
}