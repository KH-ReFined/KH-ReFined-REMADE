#include "objentry.h"

YS::OBJENTRY::GetFriendPriority_t YS::OBJENTRY::GetFriendPriority = ResolveFunctionFromCall<YS::OBJENTRY::GetFriendPriority_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x111);
YS::OBJENTRY::Get_t YS::OBJENTRY::Get = ResolveFunctionFromCall<YS::OBJENTRY::Get_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x1E2);
YS::OBJENTRY::ReadRequestWeapon_t YS::OBJENTRY::ReadRequestWeapon = ResolveFunctionFromCall<YS::OBJENTRY::ReadRequestWeapon_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x169);