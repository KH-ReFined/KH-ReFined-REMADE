#pragma once

#include "SigScan.h"

namespace YS
{
	class ITEMPIC
	{
		public:
			using ReadImage_t = void(__fastcall*)(int _image);
			static ReadImage_t ReadImage;
			using FreeImageData_t = void(__fastcall*)(char* _imageData);
			static FreeImageData_t FreeImageData;

			static char* ImageBuff;
	};
}