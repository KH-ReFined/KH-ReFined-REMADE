#include "party.h"
#include "sora.h"
#include "friend.h"
#include "converter.h"
#include "member.h"
#include "weapon_entry.h"
#include "weapon_mset.h"
#include "weapon.h"
#include "objentry.h"
#include "panacea_alloc.h"
#include "cache_buff.h"
#include "file.h"
#include "pax.h"
#include "area.h"
#include "field.h"
#include "item.h"

#include <thread>
#include <cassert>

bool THREAD_RUNNING = false;

YS::PARTY::SetWeapon_t YS::PARTY::SetWeapon = SignatureScan<YS::PARTY::SetWeapon_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x8B\x81\xC8\x06\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
YS::PARTY::ExecuteCommand_t YS::PARTY::ExecuteCommand = SignatureScan<YS::PARTY::ExecuteCommand_t>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x30\x81\xA1\x24\x01\x00\x00", "xxxxxxxxxxxxxxxx");

YS::PARTY::ChgWeapon_t YS::PARTY::ChgWeapon = SignatureScan<YS::PARTY::ChgWeapon_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57", "xxxxxxxxxxxxxxxxx");

char* YS::PARTY::KeybladePAX = ResolveRelativeAddress<char*>("\x48\x8B\xD1\x4C\x8D\x05\x00\x00\x00\x00\x4C\x8D\x0D\x00\x00\x00\x00\x49\x8B\x00\x48\x85\xC0\x74\x18\x0F\x1F\x80\x00\x00\x00\x00\x48\x39\x10\x74\x1B\x48\x8B\x48\x20\x48\x8B\xC1\x48\x85\xC9", "xxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x10A);

void ACTIVATE_CHGWEAPON(char* TASK)
{
	auto _fetchParams = reinterpret_cast<int*>(YS::PANACEA_ALLOC::Get("CHGWEAPON_PARAMS"));
	YS::PARTY::ChgWeapon(TASK, _fetchParams[0], _fetchParams[1], _fetchParams[2]);
}

void YS::PARTY::ChangeWeapon(int part, bool hand_secondary, int item)
{
	uint64_t* _funcItr = nullptr;

	while (true)
	{
		_funcItr = _funcItr ? *reinterpret_cast<uint64_t**>(_funcItr + 0x78) : *reinterpret_cast<uint64_t**>(YS::FIELD::TaskManager + 16);

		if (!_funcItr || reinterpret_cast<char*>(_funcItr) < moduleInfo.startAddr)
			break;

		if (*_funcItr == reinterpret_cast<uint64_t>(&ACTIVATE_CHGWEAPON))
			return;
	}

	auto _fetchParams = reinterpret_cast<int*>(YS::PANACEA_ALLOC::Get("CHGWEAPON_PARAMS"));

	_fetchParams[0] = part;
	_fetchParams[1] = hand_secondary;
	_fetchParams[2] = item;

	YS::FIELD::CreateThread(0, 140000, (YS::FIELD::TASK_FUNC)&ACTIVATE_CHGWEAPON, 4096);

	auto _wpnAddr = reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x24F0 + (0x114 * (part - 0x01)));

	if (hand_secondary)
	{
		auto _formID = *(YS::AREA::SaveData + 0x3524);
		_wpnAddr = reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x32F4 + (0x38 * (_formID - 1)));
	}

	// Read the weapon, just in case.
	auto _fetchWeapon = *_wpnAddr;

	if (_fetchWeapon != item)
	{
		// Attach the target item as the weapon, removing it from the backyard.
		YS::ITEM::ReduceBackyard(item, 0x01);
		*_wpnAddr = item;

		// Add the previous weapon to the backyard.
		YS::ITEM::GetBackyard(_fetchWeapon, 0x01);
	}
}