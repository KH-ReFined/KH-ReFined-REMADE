#include "intro_menu.h"
#include "SigScan.h"

vector<ReFined::IntroMenu::Entry> ReFined::IntroMenu::Children = vector<ReFined::IntroMenu::Entry>
{
	ReFined::IntroMenu::Entry(0x04, 0xC330, 0xC380, vector<uint32_t>{ 0xC331, 0xC332, 0xC333, 0xCE33 }, vector<uint32_t>{ 0xC334, 0xC335, 0xC336, 0xCE34 }),
	ReFined::IntroMenu::Entry(0x02, 0xC337, 0xC381, vector<uint32_t>{ 0xC338, 0xC339 }, vector<uint32_t>{ 0xC33A, 0xC33B }),
	ReFined::IntroMenu::Entry(0x03, 0x5733, 0xFFFF, vector<uint32_t>{ 0x5705, 0x5707, 0x5709 }, vector<uint32_t>{ 0x5706, 0x5708, 0x570A }),
	ReFined::IntroMenu::Entry(0x02, 0x5737, 0xFFFF, vector<uint32_t>{ 0x5723, 0x5725 }, vector<uint32_t>{ 0x5724, 0x5726 }),
	ReFined::IntroMenu::Entry(0x02, 0x5730, 0xFFFF, vector<uint32_t>{ 0x5738, 0x5739 }, vector<uint32_t>{ 0x5731, 0x5732 })
};

vector<char*> ReFined::IntroMenu::INTRO_OFFSETS = vector<char*>
{
	SignatureScan<char*>("\x48\x89\x5C\x24\x18\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx????"),
	SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\x0D\x00\x00\x00\x00\x48\x81\xC1\x40\x04\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxx????xxxxxxxx????"),
	SignatureScan<char*>("\x48\x83\xEC\x38\x48\x8B\x0D\x00\x00\x00\x00\x48\x89\x5C\x24\x40\x48\x85\xC9\x74\x27\xE8\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x80\x78\x0C\x00\x74\x18\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxx????xxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x20\x56\x57\x41\x56\x48\x83\xEC\x30\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxx????xxxx????"),
	SignatureScan<char*>("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00", "xxxxx????xx????xxxx????")
};

ReFined::IntroMenu::Entry::Entry(uint32_t count, uint32_t flairID, uint32_t titleID, vector<uint32_t> buttonIDs, vector<uint32_t> descriptionIDs)
{
	this->Count = count;
	this->FlairID = flairID;
	this->TitleID = titleID;
	this->ButtonIDs = buttonIDs;
	this->DescriptionIDs = descriptionIDs;
};

void ReFined::IntroMenu::Submit()
{
	for (int i = 0; i < Children.size(); i++)
	{
		auto _currentEntry = Children[i];

		memcpy(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + (0x2C * i), &_currentEntry.Count, 0x04);
		memcpy(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + (0x2C * i) + 0x04, &_currentEntry.FlairID, 0x04);
		memcpy(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + (0x2C * i) + 0x08, &_currentEntry.TitleID, 0x04);

		for (int z = 0; z < _currentEntry.ButtonIDs.size(); z++)
			memcpy(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + (0x2C * i) + (0x04 * z) + 0x0C, &_currentEntry.ButtonIDs[z], 0x04);

		for (int z = 0; z < _currentEntry.DescriptionIDs.size(); z++)
			memcpy(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + (0x2C * i) + (0x04 * z) + 0x1C, &_currentEntry.DescriptionIDs[z], 0x04);
	}

	vector<uint8_t> _firstInit(0x10);

	uint8_t _childCount = Children.size();
	uint8_t _lastIndex = Children.size() - 0x01;

	fill(_firstInit.begin(), _firstInit.begin() + 0x10, 0x00);
	memcpy(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x200, _firstInit.data(), 0x10);

	uint64_t _menuAddress = reinterpret_cast<uint64_t>(ReFined::MemoryManager::Fetch("INTRO_MEMORY"));
	uint32_t _menuOffset = ReFined::MemoryManager::Fetch("INTRO_MEMORY") - moduleInfo.startAddr;

	memcpy(INTRO_OFFSETS[0x03] + 0x097, &_childCount, 0x01);
	memcpy(INTRO_OFFSETS[0x03] + 0x1F5, &_childCount, 0x01);
	memcpy(INTRO_OFFSETS[0x03] + 0x531, &_childCount, 0x01);
	memcpy(INTRO_OFFSETS[0x04] + 0x1EF, &_childCount, 0x01);

	memcpy(INTRO_OFFSETS[0x00] + 0x3F7, &_lastIndex, 0x01);
	memcpy(INTRO_OFFSETS[0x01] + 0x3CB, &_lastIndex, 0x01);
	memcpy(INTRO_OFFSETS[0x02] + 0x031, &_lastIndex, 0x01);
	memcpy(INTRO_OFFSETS[0x03] + 0x08E, &_lastIndex, 0x01);

	memcpy(INTRO_OFFSETS[0x00] + 0x253, &_menuOffset, 0x04);
	_menuOffset += 0x04; memcpy(INTRO_OFFSETS[0x00] + 0x233, &_menuOffset, 0x04);
	_menuOffset += 0x08; memcpy(INTRO_OFFSETS[0x00] + 0x276, &_menuOffset, 0x04);
	_menuOffset += 0x10; memcpy(INTRO_OFFSETS[0x00] + 0x406, &_menuOffset, 0x04);

	RedirectLEA(INTRO_OFFSETS[0x01] + 0x1DA, ReFined::MemoryManager::Fetch("INTRO_MEMORY"));
	RedirectLEA(INTRO_OFFSETS[0x01] + 0x3D7, ReFined::MemoryManager::Fetch("INTRO_MEMORY"));
	RedirectLEA(INTRO_OFFSETS[0x01] + 0x0AF, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x08);
	RedirectLEA(INTRO_OFFSETS[0x02] + 0x03D, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x1C);

	_menuOffset += 0x1E4; memcpy(INTRO_OFFSETS[0x00] + 0x3B5, &_menuOffset, 0x04);

	RedirectLEA(INTRO_OFFSETS[0x01] + 0x0A8, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x200);
	RedirectLEA(INTRO_OFFSETS[0x04] + 0x1F2, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x200);
	RedirectMOV(INTRO_OFFSETS[0x05] + 0x2BF, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x200);
	RedirectMOV(INTRO_OFFSETS[0x06] + 0x009, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x200);
	RedirectCMP(INTRO_OFFSETS[0x06] + 0x017, ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x204);
}

void ReFined::IntroMenu::Add(int Index, Entry Input)
{
	Children.insert(Children.begin() + Index, Input);
	Submit();
}

void ReFined::IntroMenu::Remove(int Index)
{
	Children.erase(Children.begin() + Index);
	Submit();
}