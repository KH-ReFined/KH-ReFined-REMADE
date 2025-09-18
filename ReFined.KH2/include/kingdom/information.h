#pragma once
namespace dk {

	class INFORMATION {
	public:
		using openInformationWindow_t = void(*)(const char* message);
		static openInformationWindow_t openInformationWindow;
	};
}