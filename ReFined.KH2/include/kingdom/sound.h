#pragma once

#include <stdint.h>
#include "SigScan.h"

namespace YS
{
	class SOUND
	{
	public:
		using BGMFadeOut_t = void(*)(uint32_t fadeTime, uint32_t mode);
		static BGMFadeOut_t BGMFadeOut;

		using KillBGM_t = void(*)(uint32_t mode);
		static KillBGM_t KillBGM;

		using PlaySFX_t = void(*)(uint32_t soundID);
		static PlaySFX_t PlaySFX;
	};
}