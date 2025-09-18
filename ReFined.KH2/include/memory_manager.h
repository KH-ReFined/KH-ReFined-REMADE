#pragma once

#include <cstdint>
#include <map>
#include <string>
#include "SigScan.h"

using namespace std;

namespace ReFined
{
	class MemoryManager
	{
    public:
        static map<string, char*> MEMORY_BLOCK;

        using alloc_t = char* (*)(char* instance, size_t size);
        static alloc_t alloc;

        using free_t = void(*)(char* instance, char* address);
        static free_t free;

        static char* _allocInstance;
        static uint64_t _functionSpace;

        static char* Fetch(string Input);
        static void Allocate(string  Key, size_t Size);
        static void Free(string Key);
	};
}