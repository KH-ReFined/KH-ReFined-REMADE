#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT PARTY
		{
		public:
			using SetWeapon_t = void(*)(char* party_ptr, int entry_id, int hand);
			static SetWeapon_t SetWeapon;

			static char* KeybladePAX;

			// In all honesty, reimplementing this seemed like the far more stable option.
			static void ChangeWeapon(int part, bool hand_secondary, int item);
		};
	}
}