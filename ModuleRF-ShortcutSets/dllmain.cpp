// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <algorithm>

#include <Windows.h>
#include <Psapi.h>
#include <thread>

using namespace std;

bool SUBMIT_SHORTCUTS = false;
bool REVERTED_SHORTNAME = false;
bool DEBOUNCE_SHORTCUT = false;

bool EDITING_SHORTCUT_NAME = false;

string STARTING_DEFAULT = "";
uint8_t CURRENT_KEY_SHORTCUT = 0x30;

vector<uint8_t> KEY_DEBOUNCE(256);
uint8_t KEY_DEBOUNCE_TIMEOUT = 0x00;
uint16_t KEY_ENTER_TIMEOUT = 0x00;

uint8_t CURRENT_SHORTCUT_SET = 0x80;
string DEFAULT_SHORTCUT;
vector<vector<char>> SHORTCUT_NAMES;

HMODULE MAIN_HANDLE;

struct ModuleInfo
{
	const char* startAddr;
	const char* endAddr;

	ModuleInfo()
	{
		HMODULE hModule = GetModuleHandle(NULL);
		startAddr = reinterpret_cast<const char*>(hModule);

		MODULEINFO info = {};
		GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(info));
		endAddr = startAddr + info.SizeOfImage;
	}
};

static const ModuleInfo moduleInfo;

template <typename T>
T SignatureScan(const char* pattern, const char* mask)
{
	size_t patLen = std::strlen(mask);

	for (const char* addr = moduleInfo.startAddr; addr < moduleInfo.endAddr - patLen; ++addr)
	{
		size_t i = 0;
		for (; i < patLen; ++i)
		{
			if (mask[i] != '?' && pattern[i] != addr[i])
				break;
		}

		if (i == patLen)
			return reinterpret_cast<T>(const_cast<char*>(addr));
	}

	return nullptr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

char* OFFSET_INPUT_REG = SignatureScan<char*>("\x40\x53\x55\x41\x54\x41\x55\x41\x56\x48\x83\xEC\x30\x48\x8B\x2D", "xxxxxxxxxxxxxxxx");
vector<uint8_t> INPUT_REG;

extern "C"
{
	__declspec(dllexport) void RF_ModuleInit(const wchar_t* mod_path)
	{
		wchar_t filepath[MAX_PATH];

		wcscpy(filepath, mod_path);
		wcscat(filepath, L"\\dll\\ReFined.KH2.dll");

		MAIN_HANDLE = LoadLibraryW(filepath);
	}

	__declspec(dllexport) void RF_ModuleExecute(wchar_t* mod_path)
	{
		bool* _isMenu  = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsMenu@MENU@YS@@2PEA_NEA");
		bool* _isTitle = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsTitle@TITLE@YS@@2PEA_NEA");
		bool* _isInMap = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsInMap@AREA@YS@@2PEA_NEA");
		char* _menuType = *(char**)GetProcAddress(MAIN_HANDLE, "?MenuType@MENU@YS@@2PEADEA");
		char* _subMenuType = *(char**)GetProcAddress(MAIN_HANDLE, "?SubMenuType@MENU@YS@@2PEADEA");
		char* _memberTable = *(char**)GetProcAddress(MAIN_HANDLE, "?MemberTable@MEMBER_TABLE@YS@@2PEADEA");
		char* _saveData = *(char**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@YS@@2PEADEA");

		uint16_t* _hardpadInput = *(uint16_t**)GetProcAddress(MAIN_HANDLE, "?Input@HARDPAD@YS@@2PEAGEA");

		uint64_t _jiminyMenuPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_jiminymenu@MENU@YS@@2_KA");
		uint64_t _commandMenuPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_commandmenu@COMMAND_DRAW@YS@@2_KA");
		uint64_t _subOptionSelectPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_suboptionselect@MENU@YS@@2_KA");
		
		using PlaySFX_t = void(*)(uint32_t);
		using GetData_t = const char*(*)(int);
		using GetSize_t = size_t(*)(const char*);
		using DecodeKHSCII_t = string(*)(const char*);
		using EncodeKHSCII_t = vector<char>(*)(string);
		using UpdateListShortcut_t = void(*)(uint32_t);

		PlaySFX_t _playSFX = *(PlaySFX_t*)GetProcAddress(MAIN_HANDLE, "?PlaySFX@SOUND@YS@@2P6AXI@ZEA");
		GetData_t _getData = *(GetData_t*)GetProcAddress(MAIN_HANDLE, "?GetData@MESSAGE@YS@@2P6APEBDH@ZEA");
		GetSize_t _getSize = *(GetSize_t*)GetProcAddress(MAIN_HANDLE, "?GetSize@MESSAGE@YS@@2P6A?B_KPEBD@ZEA");
		UpdateListShortcut_t _updateListShortcut = *(UpdateListShortcut_t*)GetProcAddress(MAIN_HANDLE, "?UpdateListShortcut@MENU@YS@@2P6AXI@ZEA");

		DecodeKHSCII_t _decodeKHSCII = (DecodeKHSCII_t)GetProcAddress(MAIN_HANDLE, "?DecodeKHSCII@MESSAGE@YS@@SA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBD@Z");
		EncodeKHSCII_t _encodeKHSCII = (EncodeKHSCII_t)GetProcAddress(MAIN_HANDLE, "?EncodeKHSCII@MESSAGE@YS@@SA?AV?$vector@DV?$allocator@D@std@@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@4@@Z");

		if (DEFAULT_SHORTCUT.size() == 0x00)
			DEFAULT_SHORTCUT = _decodeKHSCII(_getData(0x051F));

		if (!*_isTitle && SHORTCUT_NAMES.size() == 0x00)
		{
			if (*(_saveData + 0x20) != 0x00)
			{
				auto _decodedStr = _decodeKHSCII(_saveData + 0x20);
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_decodedStr));
			}

			else
			{
				auto _fetchDefault = DEFAULT_SHORTCUT;
				replace(_fetchDefault.begin(), _fetchDefault.end(), 'X', 'A');
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_fetchDefault));
			}

			if (*(_saveData + 0x40) != 0x00)
			{
				auto _decodedStr = _decodeKHSCII(_saveData + 0x40);
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_decodedStr));
			}

			else
			{
				auto _fetchDefault = DEFAULT_SHORTCUT;
				replace(_fetchDefault.begin(), _fetchDefault.end(), 'X', 'B');
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_fetchDefault));

			}

			if (*(_saveData + 0x60) != 0x00)
			{
				auto _decodedStr = _decodeKHSCII(_saveData + 0x60);
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_decodedStr));
			}

			else
			{
				auto _fetchDefault = DEFAULT_SHORTCUT;
				replace(_fetchDefault.begin(), _fetchDefault.end(), 'X', 'C');
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_fetchDefault));
			}

			INPUT_REG.resize(4);
			memcpy(INPUT_REG.data(), OFFSET_INPUT_REG + 0x109, 0x04);
		}

		else if (*_isTitle && SHORTCUT_NAMES.size() != 0x00)
			SHORTCUT_NAMES.clear();

		if (!*_isInMap && CURRENT_SHORTCUT_SET != *(_saveData + 0xE600))
			CURRENT_SHORTCUT_SET = *(_saveData + 0xE600);

		bool IS_CUSTOMIZE = *_isMenu && *_menuType == 0x08 && *_subMenuType == 0x19 && *reinterpret_cast<uint64_t*>(_jiminyMenuPtr) == 0x00;
		bool IS_SHORTEDIT = *_isMenu && *_menuType == 0x08 && (*_subMenuType == 0x1A || *_subMenuType == 0x1D || *_subMenuType == 0x1E || *_subMenuType == 0x1F) && *reinterpret_cast<uint64_t*>(_jiminyMenuPtr) == 0x00;

		bool IS_ROXAS = (*reinterpret_cast<const uint16_t*>(_memberTable) == 0x5A || *reinterpret_cast<const uint16_t*>(_memberTable) == 0x0323);

		auto _helpBitwise = *reinterpret_cast<uint16_t*>(_saveData + 0x4270);

		if ((_helpBitwise & 0x0600) == 0x0600)
		{
			_helpBitwise += 0x0F00;
			memcpy(_saveData + 0x4270, &_helpBitwise, 0x02);
		}

		if (!*_isTitle && !IS_ROXAS)
		{
			if (IS_SHORTEDIT && !SUBMIT_SHORTCUTS)
				SUBMIT_SHORTCUTS = true;

			else if (!IS_SHORTEDIT && SUBMIT_SHORTCUTS)
			{
				memcpy(_saveData + 0xE700 + (0x08 * CURRENT_SHORTCUT_SET), _saveData + 0x36F8, 0x08);
				SUBMIT_SHORTCUTS = false;
			}

			auto _currentTextPtr = _getData(0x051F);
			auto _currentTextSize = _getSize(_currentTextPtr);

			vector<char> _currentText(_currentTextSize + 0x01);
			memcpy(_currentText.data(), _currentTextPtr, _currentTextSize + 0x01);

			if (!EDITING_SHORTCUT_NAME)
			{
				if (!IS_CUSTOMIZE && !IS_SHORTEDIT && !REVERTED_SHORTNAME)
				{
					auto _soraText = _getData(0x572E);
					auto _soraSize = _getSize(_soraText);

					memcpy(const_cast<char*>(_currentTextPtr), _soraText, _soraSize + 0x01);
					REVERTED_SHORTNAME = true;
				}

				else if ((IS_CUSTOMIZE || IS_SHORTEDIT))
				{
					bool _fetchEqual = false;

					if (SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size() > _currentText.size())
						_fetchEqual = equal(_currentText.begin(), _currentText.end(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].begin());

					else
						_fetchEqual = equal(SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].begin(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].end(), _currentText.begin());

					if (!_fetchEqual || REVERTED_SHORTNAME)
					{
						memcpy(const_cast<char*>(_currentTextPtr), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].data(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size());
						REVERTED_SHORTNAME = false;
					}
				}
			}

			bool IS_INPUT_MENU = (*_hardpadInput & 0x0400) == 0x0400 || (*_hardpadInput & 0x0800) == 0x0800;
			bool IS_INPUT_GAME = (*_hardpadInput & 0x0010) == 0x0010 || (*_hardpadInput & 0x0040) == 0x0040;

			auto _commandType = *reinterpret_cast<uint8_t**>(_commandMenuPtr);
			auto _subOptionSelect = *reinterpret_cast<uint8_t**>(_subOptionSelectPtr);

			if (_commandType != nullptr)
			{
				if (((IS_CUSTOMIZE || IS_SHORTEDIT) && !IS_INPUT_MENU) || (*_commandType == 0x05 && !IS_INPUT_GAME))
					DEBOUNCE_SHORTCUT = false;

				if (!DEBOUNCE_SHORTCUT)
				{
					if (*_commandType == 0x05)
					{
						auto _flowDirection = (*_hardpadInput & 0x0010) == 0x0010 ? -1 : ((*_hardpadInput & 0x0040) == 0x0040 ? 1 : 0);

						if (*(_saveData + 0x3524) != 0x03 && *(_saveData + 0x3524) != 0x06)
						{
							if (_flowDirection != 0x00)
							{
								_playSFX(0x14);

								CURRENT_SHORTCUT_SET += _flowDirection;
								DEBOUNCE_SHORTCUT = true;
							}
						}

						else if (_flowDirection != 0x00)
						{
							_playSFX(0x05);
							DEBOUNCE_SHORTCUT = true;
						}
					}

					else if (IS_CUSTOMIZE && _subOptionSelect != nullptr)
					{
						if (EDITING_SHORTCUT_NAME)
						{
							if (GetAsyncKeyState(VK_RETURN) & 0x8000)
							{
								_playSFX(0x02);
								memcpy(OFFSET_INPUT_REG + 0x109, INPUT_REG.data(), 0x04);

								auto _encodedEnd = _encodeKHSCII(STARTING_DEFAULT);

								_encodedEnd.insert(_encodedEnd.begin(), (char)(STARTING_DEFAULT.size() <= 13 ? 0x64 : 90 - (4 * (STARTING_DEFAULT.size() - 13))));
								_encodedEnd.insert(_encodedEnd.begin(), 0x0B);

								SHORTCUT_NAMES[CURRENT_SHORTCUT_SET] = _encodedEnd;
								memcpy(_saveData + 0x20 + (0x20 * CURRENT_SHORTCUT_SET), _encodedEnd.data(), _encodedEnd.size());

								memcpy(const_cast<char*>(_currentTextPtr), _encodedEnd.data(), _encodedEnd.size());
								EDITING_SHORTCUT_NAME = false;
								KEY_ENTER_TIMEOUT = 0;
							}

							else if (GetAsyncKeyState(VK_BACK) & 0x8000)
							{
								if (STARTING_DEFAULT.size() != 0x00 && KEY_DEBOUNCE.at(VK_BACK) == 0x00)
								{
									_playSFX(0x04);

									STARTING_DEFAULT = STARTING_DEFAULT.erase(STARTING_DEFAULT.size() - 1);

									auto _encodedEnd = _encodeKHSCII(STARTING_DEFAULT);

									char* _textWidth = new char[0x02] { 0x0B, (char)(STARTING_DEFAULT.size() <= 13 ? 0x64 : 90 - (4 * (STARTING_DEFAULT.size() - 13))) };
									memcpy(const_cast<char*>(_currentTextPtr + 0x02), _textWidth, 0x02);
									memcpy(const_cast<char*>(_currentTextPtr + 0x04), _encodedEnd.data(), _encodedEnd.size());

									KEY_DEBOUNCE.at(VK_BACK) = 0x01;
									KEY_DEBOUNCE_TIMEOUT = 0x00;
									KEY_ENTER_TIMEOUT = 0;
								}
							}

							else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && STARTING_DEFAULT.size() < 32)
							{
								if (KEY_DEBOUNCE.at(VK_SPACE) == 0x00)
								{
									_playSFX(0x01);
									STARTING_DEFAULT.push_back(' ');

									auto _encodedEnd = _encodeKHSCII(STARTING_DEFAULT);

									char* _textWidth = new char[0x02] { 0x0B, (char)(STARTING_DEFAULT.size() <= 13 ? 0x64 : 90 - (4 * (STARTING_DEFAULT.size() - 13))) };
									memcpy(const_cast<char*>(_currentTextPtr + 0x02), _textWidth, 0x02);
									memcpy(const_cast<char*>(_currentTextPtr + 0x04), _encodedEnd.data(), _encodedEnd.size());

									KEY_DEBOUNCE.at(VK_SPACE) = 0x01;
									KEY_DEBOUNCE_TIMEOUT = 0x00;
									KEY_ENTER_TIMEOUT = 0;
								}
							}

							else
							{
								if (KEY_DEBOUNCE.at(VK_SPACE) != 0x00 || KEY_DEBOUNCE.at(VK_BACK) != 0x00)
									KEY_DEBOUNCE.at(VK_SPACE) = 0; KEY_DEBOUNCE.at(VK_BACK) = 0; KEY_DEBOUNCE_TIMEOUT = 0x00;

								for (int k = 0x30; k <= 0x5A; k++)
								{
									if (k >= 0x3A && k <= 0x40)
										k = 0x41;

									if (GetAsyncKeyState(k) & 0x8000)
									{
										if (KEY_DEBOUNCE.at(k) == 0x00)
										{
											if (STARTING_DEFAULT.size() < 32)
											{
												_playSFX(0x01);

												if (k >= 0x41)
													STARTING_DEFAULT.push_back(k + ((GetKeyState(VK_CAPITAL) & 0x01) == 0x01 ? 0x00 : 0x20));

												else
													STARTING_DEFAULT.push_back(k);

												auto _encodedEnd = _encodeKHSCII(STARTING_DEFAULT);

												char* _textWidth = new char[0x02] { 0x0B, (char)(STARTING_DEFAULT.size() <= 13 ? 0x64 : 90 - (4 * (STARTING_DEFAULT.size() - 13))) };
												memcpy(const_cast<char*>(_currentTextPtr + 0x02), _textWidth, 0x02);
												memcpy(const_cast<char*>(_currentTextPtr + 0x04), _encodedEnd.data(), _encodedEnd.size());

												KEY_DEBOUNCE.at(k) = 0x01;
												KEY_DEBOUNCE_TIMEOUT = 0x00;

												KEY_ENTER_TIMEOUT = 0;

												break;
											}

											else
											{
												_playSFX(0x05);
												KEY_ENTER_TIMEOUT = 0;
												break;
											}
										}
									}

									else if (KEY_DEBOUNCE.at(k) != 0x00)
										KEY_DEBOUNCE.at(k) = 0x00;
								}
							}

							KEY_DEBOUNCE_TIMEOUT++;
							KEY_ENTER_TIMEOUT++;

							if (KEY_DEBOUNCE_TIMEOUT >= 10)
							{
								KEY_DEBOUNCE.at(VK_SPACE) = 0; KEY_DEBOUNCE.at(VK_BACK) = 0;
								KEY_DEBOUNCE_TIMEOUT = 0x00;
							}

							if (KEY_ENTER_TIMEOUT >= 300)
							{
								_playSFX(0x04);
								memcpy(OFFSET_INPUT_REG + 0x109, INPUT_REG.data(), 0x04);

								memcpy(const_cast<char*>(_currentTextPtr), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].data(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size());
								EDITING_SHORTCUT_NAME = false;

								KEY_DEBOUNCE.at(VK_SPACE) = 0; KEY_DEBOUNCE.at(VK_BACK) = 0;
								KEY_DEBOUNCE_TIMEOUT = 0x00;
								KEY_ENTER_TIMEOUT = 0;
							}
						}

						if (*_subOptionSelect == 0x00)
						{
							// If TRIANGLE input received:
							if ((*_hardpadInput & 0x1000) == 0x1000)
							{
								_playSFX(0x02);

								EDITING_SHORTCUT_NAME = true;

								char* _nopArray = new char[0x04];
								fill(_nopArray, _nopArray + 0x04, 0x90);

								memcpy(OFFSET_INPUT_REG + 0x109, _nopArray, 0x04);

								char* _textColor = new char[0x02] { 0x04, 0x01 };
								memcpy(const_cast<char*>(_currentTextPtr), _textColor, 0x02);

								memcpy(const_cast<char*>(_currentTextPtr + 0x02), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].data(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size());

								STARTING_DEFAULT = _decodeKHSCII(SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].data() + 0x02);

								*_hardpadInput = 0x00;
							}

							else
							{
								auto _flowDirection = (*_hardpadInput & 0x0400) == 0x0400 ? -1 : ((*_hardpadInput & 0x0800) == 0x0800 ? 1 : 0);

								if (_flowDirection != 0x00)
								{
									_playSFX(0x02);

									CURRENT_SHORTCUT_SET += _flowDirection;
									DEBOUNCE_SHORTCUT = true;
								}
							}
						}
					}
				}
			}

			if (CURRENT_SHORTCUT_SET == 0x80)
				CURRENT_SHORTCUT_SET = *(_saveData + 0xE600);

			if (CURRENT_SHORTCUT_SET >= 0x81)
				CURRENT_SHORTCUT_SET = 0x02;

			if (CURRENT_SHORTCUT_SET >= 0x03)
				CURRENT_SHORTCUT_SET = 0x00;

			if (*_isInMap && CURRENT_SHORTCUT_SET != *(_saveData + 0xE600) && DEBOUNCE_SHORTCUT)
			{
				memcpy(_saveData + 0x36F8, _saveData + 0xE700 + (0x08 * CURRENT_SHORTCUT_SET), 0x08);
				*(_saveData + 0xE600) = CURRENT_SHORTCUT_SET;

				if (IS_CUSTOMIZE)
					_updateListShortcut(0x00);
			}
		}
	}
}


