#include "party.h"
#include "sora.h"
#include "friend.h"
#include "converter.h"
#include "member.h"
#include "weapon_entry.h"
#include "weapon.h"
#include "objentry.h"
#include "cache_buff.h"
#include "file.h"

#include <cassert>

YS::PARTY::SetWeapon_t YS::PARTY::SetWeapon = SignatureScan<YS::PARTY::SetWeapon_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x8B\x81\xC8\x06\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

char* _wpnUser = nullptr;
char* _wpnCache = nullptr;

int _wpnPart = 0x00;
bool _wpnHand = false;
bool _wpnHide = false;
uint16_t _wpnItem = 0x00;

void YS::PARTY::WeaponUpdate()
{
	if (_wpnUser != nullptr)
	{
		auto _checkFlush = reinterpret_cast<bool(*)()>(moduleInfo.startAddr + 0x39DCC0)();

		if (!_checkFlush)
		{
			auto _wpnEntry = YS::WEAPON_ENTRY::Get(_wpnPart, _wpnItem);
			YS::PARTY::SetWeapon(_wpnUser, _wpnEntry, _wpnHand);

			auto _weaponPtr = *reinterpret_cast<char**>(_wpnUser + 0x08 * _wpnHand + 0x0D60);
			int* _weaponInt = reinterpret_cast<int*>(_weaponPtr);

			uint64_t _weaponAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*_weaponInt);
			uint64_t* _weaponAddrPtr = reinterpret_cast<uint64_t*>(_weaponAddr);

			if (!_wpnHide)
				reinterpret_cast<void(*)(uint64_t, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x88))(_weaponAddr, _weaponInt);

			_wpnUser = nullptr;

			_wpnPart = 0x00;
			_wpnItem = 0x00;
			_wpnHand = false;
		}
	}
}

void YS::PARTY::ChangeWeapon(int part, bool hand_secondary, int item)
{
	char* _charPtr = nullptr; 
	bool is_hide = false;

	int _wpnBank = 0;
	int _wpnPriority = 0;

	_wpnHand = hand_secondary;
	_wpnItem = item;

	char* _wpnLoaded = nullptr;

	for (int i = 0; i <= 3; i++)
	{
		_charPtr = i == 0x00 ? CalculatePointer(YS::SORA::pint_sora, { 0x00 }) : 
							   CalculatePointer(YS::FRIEND::pint_friend, { 0x08 * (i - 1)});

		if (_charPtr == nullptr)
			continue;

		_wpnPart = *reinterpret_cast<int8_t*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_charPtr + 0x08)) + 0x4C);

		if (_charPtr != nullptr && _wpnPart == part)
			break;

		if (i == 0x03 && part == 0x01)
			_charPtr = CalculatePointer(YS::SORA::pint_sora, { 0x00 });

		else if (i == 0x03)
			break;
	}

	if (_charPtr != nullptr)
	{
		auto _weaponPtr = *reinterpret_cast<char**>(_charPtr + 0x08 * _wpnHand + 0x0D60);
		int* _weaponInt = reinterpret_cast<int*>(_weaponPtr);

		uint64_t _weaponAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*_weaponInt);
		uint64_t* _weaponAddrPtr = reinterpret_cast<uint64_t*>(_weaponAddr);

		_wpnPriority = *reinterpret_cast<uint32_t*>(_weaponPtr + 0x0C0C);
		_wpnHide = (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x124) & 0x180) != 0x00 || (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x6C8) & 0x44) == 0x00 || (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x120) & 0x400) != 0x00;

		auto _wpnBankAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_weaponPtr + 0x0BB0));

		if (_wpnBankAddr != 0x00)
			_wpnBank = *(uint16_t*)_wpnBankAddr;
		
		else
			_wpnBank = 0xFFFF;

		*reinterpret_cast<uint64_t*>(_weaponPtr + 0x124) |= 0x700;
		reinterpret_cast<void(*)(uint64_t, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x48))(_weaponAddr, _weaponInt);

		if ((*reinterpret_cast<uint64_t*>(_weaponPtr + 0x9B8) & 0x40) == 0x00)
		{
			reinterpret_cast<void(*)(uint64_t, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x30))(_weaponAddr, _weaponInt);
			*reinterpret_cast<uint64_t*>(_weaponPtr + 0x9B8) |= 0x40;
		}

		*reinterpret_cast<uint64_t*>(_charPtr + 0x08 * _wpnHand + 0x0D60) = 0x00;

		_wpnPart = *reinterpret_cast<int8_t*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_charPtr + 0x08)) + 0x4E);

		if (_wpnPart != 0x35 && !_wpnHand && (*reinterpret_cast<uint8_t*>(_charPtr + 0x06CB) & 0x01) != 0x00)
		{
			auto _object = YS::SORA::GetEntryID(0x00);
			_wpnPart = *reinterpret_cast<uint16_t*>(YS::OBJENTRY::Get(_object) + 0x4E);
		}
	}

	else
	{
		auto _object = YS::MEMBER::PartToEntryID(part);
		auto _priority = YS::OBJENTRY::GetFriendPriority(_object);

		if (_priority > 0x096A)
		{
			if (_priority != 0x0974)
				return;

			_wpnBank = 0x06;
		}

		else
		{
			if (_priority <= 0x0969)
				return;

			_wpnBank = 0x05;
		}

		_wpnPriority = _priority + 1;
		_wpnPart = *reinterpret_cast<uint16_t*>(YS::OBJENTRY::Get(_object) + 0x4E);
	}
	
	reinterpret_cast<void(*)(int)>(moduleInfo.startAddr + 0x39D010)(_wpnPriority);
	YS::OBJENTRY::ReadRequestWeapon(_wpnPart, _wpnHand, YS::WEAPON_ENTRY::Get(_wpnPart, _wpnItem), _wpnPriority, _wpnBank);

	reinterpret_cast<void(*)(char*)>(moduleInfo.startAddr + 0x39D460)(nullptr);
	_wpnUser = _charPtr;


}