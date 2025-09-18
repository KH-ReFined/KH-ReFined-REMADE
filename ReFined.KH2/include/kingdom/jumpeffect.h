#pragma once

#include <stdint.h>

namespace dk
{
	class JUMPEFFECT
	{
		public:
			using Out_t = void(*)(uint64_t effectType);
			static Out_t Out;

			static char* FadeStatus;
	};
}