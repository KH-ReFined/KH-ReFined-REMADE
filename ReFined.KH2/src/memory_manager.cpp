#include "memory_manager.h"

using createAllocator_t = char* (*)(const char* _baseAddr, size_t size);
createAllocator_t createAllocator = SignatureScan<createAllocator_t>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8D\x59\x0F\x33\xFF\x48\x83\xE3\xF0\x4C\x8D\x0C\x11\x4C\x8D\x83\x80\x00\x00\x00\x49\x8D\x40\x30\x4C\x3B\xC8\x0F\x86\x8E\x00\x00\x00\x48\x39\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");

map<string, char*> ReFined::MemoryManager::MEMORY_BLOCK = map<string, char*>();

char* ReFined::MemoryManager::_allocInstance = createAllocator(moduleInfo.startAddr + 0x7B0000, 0x1000);
uint64_t ReFined::MemoryManager::_functionSpace = *reinterpret_cast<const uint64_t*>(_allocInstance);

ReFined::MemoryManager::alloc_t ReFined::MemoryManager::alloc = *reinterpret_cast<const alloc_t*>(_functionSpace + 0x08);
ReFined::MemoryManager::free_t ReFined::MemoryManager::free = *reinterpret_cast<const free_t*>(_functionSpace + 0x10);

char* ReFined::MemoryManager::Fetch(string Input)
{
    auto tagCheck = MEMORY_BLOCK.find(Input) != MEMORY_BLOCK.end();

    if (!tagCheck)
        return 0x00;

    uint64_t tagValue = *reinterpret_cast<const uint64_t*>(MEMORY_BLOCK.at(Input));

    bool tagValid = tagValue != 0xCAFEEFACCAFEEFAC &&
        tagValue != 0xEFACCAFEEFACCAFE;

    return tagValid ? MEMORY_BLOCK.at(Input) : 0x00;
}

void ReFined::MemoryManager::Allocate(string Key, size_t Size)
{
    MEMORY_BLOCK[Key] = alloc(_allocInstance, Size);
}

void ReFined::MemoryManager::Free(string Key)
{
    auto _checkMemory = MEMORY_BLOCK.find(Key) != MEMORY_BLOCK.end();

    if (_checkMemory)
    {
        free(_allocInstance, MEMORY_BLOCK[Key]);
        MEMORY_BLOCK.erase(Key);
    }
}