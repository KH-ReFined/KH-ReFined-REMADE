#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SOUND
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
}