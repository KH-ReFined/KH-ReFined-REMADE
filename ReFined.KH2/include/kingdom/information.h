#pragma once

#define DLL_EXPORT __declspec(dllexport)
extern "C"
{
	namespace dk {

		class DLL_EXPORT INFORMATION {
		public:
			using openInformationWindow_t = void(*)(const char* message);
			static openInformationWindow_t openInformationWindow;
		};
	}
}