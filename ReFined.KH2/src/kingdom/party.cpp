#include "party.h"
#include "sora.h"
#include "friend.h"
#include "converter.h"
#include "member.h"
#include "weapon_entry.h"
#include "weapon_mset.h"
#include "weapon.h"
#include "objentry.h"
#include "cache_buff.h"
#include "file.h"
#include "pax.h"
#include "area.h"
#include "item.h"

#include <thread>
#include <cassert>

bool THREAD_RUNNING = false;

YS::PARTY::SetWeapon_t YS::PARTY::SetWeapon = SignatureScan<YS::PARTY::SetWeapon_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x8B\x81\xC8\x06\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
											 
char* KEYBLADE_PAX = ResolveRelativeAddress<char*>("\x48\x8B\xD1\x4C\x8D\x05\x00\x00\x00\x00\x4C\x8D\x0D\x00\x00\x00\x00\x49\x8B\x00\x48\x85\xC0\x74\x18\x0F\x1F\x80\x00\x00\x00\x00\x48\x39\x10\x74\x1B\x48\x8B\x48\x20\x48\x8B\xC1\x48\x85\xC9", "xxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x10A);

void YS::PARTY::ChangeWeapon(int part, bool hand_secondary, int item)
{
	thread _fetchThread([part, hand_secondary, item]
	{
		while (THREAD_RUNNING) {}

		THREAD_RUNNING = true;

		char* _charPtr = nullptr;
		bool is_hide = false;

		int _wpnBank = 0;
		int _wpnPart = 0;
		int _wpnPriority = 0;

		bool _wpnHide = false;

		// For every possible slot that *may* or *may not* contain a party member;
		for (int i = 0; i <= 3; i++)
		{
			// Fetch the pointer in which the party member is initialized.
			_charPtr = i == 0x00 ? *reinterpret_cast<char**>(YS::SORA::pint_sora) :
				*reinterpret_cast<char**>(YS::FRIEND::pint_friend + 0x08 * (i - 1));

			// If the pointer does not exist, kindly continue.
			if (_charPtr == nullptr)
				continue;

			// Fetch the WEAPON_PART variable.
			_wpnPart = *reinterpret_cast<int8_t*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_charPtr + 0x08)) + 0x4C);

			// If the WEAPON_PART parsed matches the WEAPON_PART requested, we have our target party member.
			if (_wpnPart == part)
				break;

			// Otherwise, if we are at the end of the loop, and the part requested is 0x01, give out Sora's pointer.
			// I have no clue why this is necessary or if it even is. But it was in the ASM, so it is here.

			if (i == 0x03 && part == 0x01)
				_charPtr = *reinterpret_cast<char**>(YS::SORA::pint_sora);

			// Else, break out. We will parse the character we need another way.
			else if (i == 0x03)
				break;
		}

		// If we have a hit on the party member;
		if (_charPtr != nullptr)
		{
			// Fetch the pointers for the weapon and its function space.

			auto _weaponPtr = *reinterpret_cast<char**>(_charPtr + 0x08 * hand_secondary + 0x0D60);
			int* _weaponInt = reinterpret_cast<int*>(_weaponPtr);

			uint64_t _weaponAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*_weaponInt);
			uint64_t* _weaponAddrPtr = reinterpret_cast<uint64_t*>(_weaponAddr);

			if (_weaponPtr == nullptr || reinterpret_cast<int64_t>(_weaponPtr) == -1 || _weaponInt == nullptr)
				return;

			// Fetch the priority and see if it should be hidden or not.
			_wpnPriority = *reinterpret_cast<uint32_t*>(_weaponPtr + 0x0C0C);
			_wpnHide = (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x124) & 0x180) != 0x00 || (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x6C8) & 0x44) == 0x00 || (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x120) & 0x400) != 0x00;

			// Fetch the weapon bank, set to -1 if it is not defined or otherwise can't be accessed.

			auto _wpnBankAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_weaponPtr + 0x0BB0));

			if (_wpnBankAddr != 0x00)
				_wpnBank = *reinterpret_cast<uint16_t*>(_wpnBankAddr);

			else
				_wpnBank = 0xFFFF;

			// Force-Destroy the current keyblade.

			*reinterpret_cast<uint64_t*>(_weaponPtr + 0x124) |= 0x700; // Hides the Keyblade.
			reinterpret_cast<void(*)(uint64_t, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x48))(_weaponAddr, _weaponInt); // Compresses the VIF packages(?).

			// This essentially reimplements YS::OBJ::destroy(). Why? Why not.

			if ((*reinterpret_cast<uint64_t*>(_weaponPtr + 0x9B8) & 0x40) == 0x00)
			{
				reinterpret_cast<void(*)(uint64_t, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x30))(_weaponAddr, _weaponInt);
				*reinterpret_cast<uint64_t*>(_weaponPtr + 0x9B8) |= 0x40;
			}

			// Erase the weapon pointer.
			*reinterpret_cast<uint64_t*>(_charPtr + 0x08 * hand_secondary + 0x0D60) = 0x00;

			// Parse the current weapon part from the parsed character.
			_wpnPart = *reinterpret_cast<int8_t*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_charPtr + 0x08)) + 0x4E);

			// Check if the weapon targeted is Sora's MAIN weapon;
			if (_wpnPart != 0x35 && !hand_secondary && (*reinterpret_cast<uint8_t*>(_charPtr + 0x06CB) & 0x01) != 0x00)
			{
				// Parse Sora's current costume from the objentry, then parse the WEAPON_PART accordingly.
				auto _object = YS::SORA::GetEntryID(0x00);
				_wpnPart = *reinterpret_cast<uint16_t*>(YS::OBJENTRY::Get(_object) + 0x4E);
			}
		}

		// Otherwise;
		else
		{
			// Fetch the current object from WEAPON_PART and its priority.
			auto _object = YS::MEMBER::PartToEntryID(part);
			auto _priority = YS::OBJENTRY::GetFriendPriority(_object);

			// If the priority is larger than 0x096A;
			if (_priority > 0x096A)
			{
				// If it is *specifically* 0x0974, we return as it is not a correct parse.
				if (_priority != 0x0974)
					return;

				// Otherwise, Weapon Bank is 0x06.
				_wpnBank = 0x06;
			}

			// Otherwise;
			else
			{
				// If it is *smaller* than 0x0969, we return as it is not a correct parse.
				if (_priority <= 0x0969)
					return;

				// Otherwise, Weapon Bank is 0x05.
				_wpnBank = 0x05;
			}

			// Apply the WEAPON_PART and Priority accordingly. A Party member's priority is always their own + 1.
			_wpnPriority = _priority + 1;
			_wpnPart = *reinterpret_cast<uint16_t*>(YS::OBJENTRY::Get(_object) + 0x4E);
		}

		// Destroy all files with the given priority from the CACHE_BUFF.
		YS::CACHE_BUFF::DestroyPriority(_wpnPriority);

		auto _fetchWeaponID = YS::WEAPON_ENTRY::Get(_wpnPart, item);

		// Request reading the files necessary for the current weapon. SINCE this is not a synchronous function, nor can it be, we will have to have an update function running.
		// Now I *can* use Task Managers within the game and their thread management. But do I want to? No. No I do not.
		// And before you ask, all C/C++ threads either block execution or crash Panacea.

		YS::OBJENTRY::ReadRequestWeapon(_wpnPart, hand_secondary, _fetchWeaponID, _wpnPriority, _wpnBank);
		YS::CACHE_BUFF::Flush(nullptr);


		while (YS::CACHE_BUFF::IsFlushing()) {}

		// Get the target weapon's objentry and initialize swap.

		auto _wpnEntry = YS::WEAPON_ENTRY::Get(_wpnPart, item);
		YS::PARTY::SetWeapon(_charPtr, _wpnEntry, hand_secondary);

		// Fetch the necessary pointers.

		auto _weaponPtr = *reinterpret_cast<char**>(_charPtr + 0x08 * hand_secondary + 0x0D60);
		int* _weaponInt = reinterpret_cast<int*>(_weaponPtr);

		uint64_t _weaponAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*_weaponInt);
		uint64_t* _weaponAddrPtr = reinterpret_cast<uint64_t*>(_weaponAddr);

		// If the weapon needs to be shown, set the parameter.
		if (!_wpnHide)
		{
			reinterpret_cast<void(*)(uint64_t, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x88))(_weaponAddr, _weaponInt);
		
			if (_wpnPart <= 0x02)
			{
				auto _targetPAX = CalculatePointer(reinterpret_cast<uint64_t>(KEYBLADE_PAX), { 0x5B0 + 0x08 * hand_secondary, 0x00 });
				ryj::PAX::Start(_targetPAX, 0x00, 0x01, 0x00, 0x00);
			}
		}

		// If the WEAPON_PART is 0x01 [Meaning the character is Sora]:
		if (_wpnPart == 0x01)
		{
			// Calculate the address of the Weapon Item according to Sora's form.
			auto _wpnAddr = reinterpret_cast<uint16_t*>(*(YS::AREA::SaveData + 0x3524) == 0x01 ? YS::AREA::SaveData + 0x32F4 :
				(*(YS::AREA::SaveData + 0x3524) == 0x04 ? YS::AREA::SaveData + 0x339C :
					(*(YS::AREA::SaveData + 0x3524) == 0x05 ? YS::AREA::SaveData + 0x33D4 : YS::AREA::SaveData + 0x24F0)));

			// Read the weapon, just in case.
			auto _fetchWeapon = *_wpnAddr;

			// Attach the target item as the weapon, removing it from the backyard.
			YS::ITEM::ReduceBackyard(item, 0x01);
			*_wpnAddr = item;

			// Add the previous weapon to the backyard.
			YS::ITEM::GetBackyard(_fetchWeapon, 0x01);
		}

		// If it ain't Sora:
		else
		{
			auto _wpnAddr = YS::AREA::SaveData + 0x24F0 + (0x114 * (_wpnPart - 0x01));

			// Read the weapon, just in case.
			auto _fetchWeapon = *_wpnAddr;

			// Attach the target item as the weapon, removing it from the backyard.
			YS::ITEM::ReduceBackyard(item, 0x01);
			*_wpnAddr = item;

			// Add the previous weapon to the backyard.
			YS::ITEM::GetBackyard(_fetchWeapon, 0x01);
		}

		THREAD_RUNNING = false;
	});

	_fetchThread.detach();
}

