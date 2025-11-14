#include "SigScan.h"

#include "area.h"
#include "title.h"
#include "hardpad.h"
#include "egs.h"
#include "steam.h"
#include "command_draw.h"
#include "command_elem.h"

#include <vector>

using namespace std;

extern "C"
{
	__declspec(dllexport) void RF_ModuleExecute()
	{
		auto _pointAction = CalculatePointer(YS::COMMAND_ELEM::CommandElem, { 0x0A });

		auto _currAction = *reinterpret_cast<const uint32_t*>(_pointAction);
		auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);
		auto _currChildMenu = *reinterpret_cast<const uint64_t*>(YS::COMMAND_DRAW::pint_commandmenu + 0x08);

		auto _confirmConfig = *(FindModule("steam_api64.dll") ? PC::STEAM::MareConfig + 0x1E : PC::EGS::MareConfig + 0x1E);
		auto _confirmButton = _confirmConfig == 0x00 ? 0x4000 : 0x2000;

		if (!*YS::TITLE::IsTitle && _commandPointer != 0x00)
		{
			auto _currCommand = *reinterpret_cast<const uint8_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x74 }));
			auto _mainMenuType = *reinterpret_cast<const uint8_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x00 }));
			auto _firstCommand = *reinterpret_cast<const uint16_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x16 }));
			auto _commandFlag = *reinterpret_cast<const uint8_t*>(CalculatePointer(YS::COMMAND_DRAW::pint_commandmenu, { 0x19 }));

			bool _isActionGood = _currChildMenu == 0x00 && _currCommand == 0x00;
			bool _isCommandGood = (_firstCommand == 0x0001 || _firstCommand == 0x016D || _firstCommand == 0x0088) && (_commandFlag == 0x00) && (_mainMenuType == 0x00 || _mainMenuType == 0x06);

			auto _fetchButtons = *YS::HARDPAD::Input;

			if (*YS::AREA::BattleStatus != 0x00 && _isCommandGood && _isActionGood && (_fetchButtons & _confirmButton) == _confirmButton && _currAction == 0x00)
			{
				_currAction = 0x01;
				memcpy(const_cast<char*>(_pointAction), &_currAction, 0x04);
			}

			else if (_currAction == 0x01)
			{
				_currAction = 0x00;
				memcpy(const_cast<char*>(_pointAction), &_currAction, 0x04);
			}
		}
	}
}