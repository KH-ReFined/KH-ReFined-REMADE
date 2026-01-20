#include "field.h"

YS::FIELD::CreateThread_t YS::FIELD::CreateThread = ResolveFunctionFromCall<YS::FIELD::CreateThread_t>("\x40\x53\x48\x83\xEC\x20\x0F\xB6\xD9\xE8\x00\x00\x00\x00\x84\xC0", "xxxxxxxxxx????xx", 0xA2);

char* YS::FIELD::TaskManager = ResolveRelativeAddress<char*>(reinterpret_cast<char*>(YS::FIELD::CreateThread), 0x28);