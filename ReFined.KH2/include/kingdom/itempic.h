#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ITEMPIC
		{
		public:
			using ReadImage_t = void(__fastcall*)(int _image);
			static ReadImage_t ReadImage;
			using FreeImageData_t = void(__fastcall*)(char* _imageData);
			static FreeImageData_t FreeImageData;

			static char* ImageBuff;
		};
	}
}