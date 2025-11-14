#include "Critical.h"
#include <memory_manager.h>
#include <sstream>
#include <iomanip>

using namespace std;

bool IS_DEAD;
bool FACE_APPLIED;

char* SORA_PORTRAIT;

uint8_t PAST_FORM;
uint8_t PAST_CROWN;

uint32_t MAGIC_FIRST;
uint16_t MAGIC_SECOND;

map<uint32_t, char*> MAGIC_FILES;

vector<uint16_t> ABILITY_ARRAY;

map<uint8_t, string> SORA_FORMS
{
	{0x00, "P_EX100"},
	{0x01, "P_EX100_BTLF"},
	{0x02, "P_EX100_MAGF"},
	{0x03, "P_EX100_KH1F"},
	{0x04, "P_EX100_TRIF"},
	{0x05, "P_EX100_ULTF"},
	{0x06, "P_EX100_HTLF"}
};

vector<uint8_t> INST_MAPJUMPTASK;
vector<uint8_t> INST_CONTINUELOAD;

vector<uint8_t> RETRY_STATE;

bool HADES_ESCAPE;
bool HADES_CHANGED;
uint8_t HADES_ITERATOR = 0xFF;
bool RETRY_BLACKLIST;
uint8_t RETRY_MODE;

bool INTRO_APPLIED;
bool ENFORCE_INTRO;

bool CONFIG_INIT;
bool CONFIG_FETCH;
bool CONFIG_FIRST_INIT;
uint16_t SETTING_MEMORY;

char ReFined::Critical::SAVE_MODE = 0x00;
bool ReFined::Critical::CONTROLLER_MODE = false;

char* COMMAND_MENU_ADDR = ResolveRelativeAddress<char*>("\x40\x56\x48\x83\xEC\x30\x8B\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x89\x5C\x24\x40\x48\x89\x7C\x24\x48", "xxxxxxxx????x????xxxx????x????xxxx????xxxxxxxxxx", 0x50);
bool* COMMAND_FLAG_ADDR = ResolveRelativeAddress<bool*>("\x40\x56\x48\x83\xEC\x30\x8B\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x89\x5C\x24\x40\x48\x89\x7C\x24\x48", "xxxxxxxx????x????xxxx????x????xxxx????xxxxxxxxxx", 0x5D) + 0x04;
uint8_t* COMMAND_TYPE_ADDR = ResolveRelativeAddress<uint8_t*>("\x48\x83\xEC\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9\x00\x00\x00\x00\xCC\xCC\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxxx????x????xxx????xxx????xxx????xxxxx????xxxxx????xxx????x", 0x1A);

char* CAMP_OFFSET = SignatureScan<char*>("\x48\x8B\xC4\x48\x81\xEC\x88\x00\x00\x00\x48\x89\x58\x18\xBA\x02\x00\x00\x00\x48\x89\x68\xF8\x48\x89\x70\xF0", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* CAMPINIT_OFFSET = SignatureScan<char*>("\x66\x44\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x44\x88\x35\x00\x00\x00\x00\x0F\x95\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\xC7\x44\x24\x30\x8C\x00\x00\x00", "xxxx????x????xxxxx????xxx????x????xxx????xxxxxxxx");

vector <uint8_t> INST_CAMPBITWISE;
vector <uint8_t> INST_CAMPINIT;

char* MENUSELECT_OFFSET = SignatureScan<char*>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\x98\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx0000");
char* INFORMATION_OFFSET = SignatureScan<char*>("\x41\xB8\x40\x00\x00\x00\xB9\xAA\x00\x00\x00\x66\x2B\xC1\x66\x44\x89\x44\x24\x20\x44\x0F\xB7\x43\x2C\x48\x8D\x8B\x60\x02\x00\x00\x44\x0F\xB7\xC8\x33\xD2", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

bool SET_ADJUSTMENT;

ReFined::Continue::Entry RETRY_ENTRY(0x0002, 0x8AB1);
ReFined::Continue::Entry PREPARE_ENTRY(0x0002, 0x5727);

vector<char*> POSITIVE_ASPECT_SHORT;
vector<char*> NEGATIVE_ASPECT_SHORT = MultiSignatureScan("\xC7\x00\x00\x00\xAB\xFF\xFF\xFF", "x???xxxx");
vector<char*> POSITIVE_ASPECT_LONG = MultiSignatureScan("\xC7\x00\x00\x00\x00\x00\x55\x00\x00\x00", "x?????xxxx");
vector<char*> NEGATIVE_ASPECT_LONG = MultiSignatureScan("\xC7\x00\x00\x00\x00\x00\xAB\xFF\xFF\xFF", "x?????xxxx");

vector<char*> POSITIVE_ASPECT_BYTE;
vector<char*> NEGATIVE_ASPECT_BYTE;
												
char* VIEWPORT3D_ADDR = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

auto _fetchSimpleCont = MultiSignatureScan("\x41\x8D\x46\xAB\x41\x89\x84\x3F\x8C\x0D\x00\x00", "xxxxxxxxxxxx");

// Handles the registration and activation of Magic outside of load zones.
// Used *generally* with Rando, but can be used for other purposes by other mods.
void ReFined::Critical::RegisterMagic()
{
	// Fetch the current levels or magic.

	uint32_t _magicFirst = *reinterpret_cast<const uint32_t*>(YS::AREA::SaveData + 0x3594);
	uint16_t _magicSecond = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x35CF);

	// If the game is loaded, and if the Tier 1 Magic or the Tier 2 Magic levels do not match what is previously recorded.

	if (*YS::AREA::IsInMap && (_magicFirst != MAGIC_FIRST || _magicSecond != MAGIC_SECOND))
	{
		// Initialize the array we will be using for the commands.
		vector<uint16_t> _commandArray;

		// For 6 Magic (Fire, Blizzard, Thunder, Cure, Magnet, Reflect):
		for (uint32_t i = 0; i < 0x06; i++)
		{
			// Get the current table and denote the pointer.

			auto _currentTable = YS::MAGIC::GetTable(i);
			auto _currentTablePtr = reinterpret_cast<uint64_t>(_currentTable);

			// If the current table exists (Meaning we have the Magic):

			if (_currentTable != 0x00)
			{
				// Fetch the previously denoted level of the current Magic, and fetch the current Magic table pointer.

				char _fetchMagicLevel = i <= 0x03 ? *(&MAGIC_FIRST + i) : *(&MAGIC_SECOND + (i - 0x04));
				auto _currentPointer = *reinterpret_cast<const uint64_t*>(YS::MAGIC::MagicInfo + 0x48 + 0x50 * i);

				// If the current Magic table is the one we fetched and the current Magic level is not zero (Meaning we have the Magic and it was processed), or the denoted level equals to the current level:
				if ((_currentPointer == _currentTablePtr && _fetchMagicLevel != 0x00) || _fetchMagicLevel == _currentTable->Level)
				{
					// Denote the command of the current Magic to the array, and skip to the next entry.
					_commandArray.push_back(_currentTable->Command);
					continue;
				}

				// If a previous of the Magic was processed by this function, free it and remove it from the process list.
				if (MAGIC_FILES.count(i) != 0x00)
				{
					free(MAGIC_FILES.at(i));
					MAGIC_FILES.erase(i);
				}

				// Get the size of the .mag file and allocate memory for it.
				size_t _fetchSize = YS::FILE::GetSize(_currentTable->Filename);
				auto _loadBAR = (char*)malloc(_fetchSize);

				// If the magic file can load (Meaning it exists and we have allocated the memory successfully):
				if (YS::FILE::LoadBAR(_currentTable->Filename, _loadBAR) != 0x00)
				{
					// Denote the address of the loaded file for future handling.
					MAGIC_FILES[i] = _loadBAR;

					// Get the address of the BAR as a uint64_t for calculations, and get the 32-bit offset of the BAR.
					uint64_t _barAddress = reinterpret_cast<uint64_t>(_loadBAR);
					uint32_t _barFileOffset = *reinterpret_cast<const uint32_t*>(_loadBAR + 0x08);

					// Calculate the absolute addresses of PAX and BDX files.
					uint64_t _paxAddress = _barAddress + *reinterpret_cast<const uint32_t*>(_loadBAR + 0x18) - _barFileOffset;
					uint64_t _bdxAddress = _barAddress + *reinterpret_cast<const uint32_t*>(_loadBAR + 0x28) - _barFileOffset;

					// As well as the address of which PAX actually starts in.
					uint64_t _paxStartAddress = _barAddress + *reinterpret_cast<const uint32_t*>(_loadBAR + 0x18) + 0x10 - _barFileOffset;

					// Write all of the info needed for the Magic to be parsed, processed, and executed.
					memcpy(YS::MAGIC::MagicInfo + 0x50 * i, &_barAddress, 0x08);
					memcpy(YS::MAGIC::MagicInfo + 0x08 + 0x50 * i, &_bdxAddress, 0x08);
					memcpy(YS::MAGIC::MagicInfo + 0x10 + 0x50 * i, &_bdxAddress, 0x08);
					memcpy(YS::MAGIC::MagicInfo + 0x18 + 0x50 * i, &_paxAddress, 0x08);
					memcpy(YS::MAGIC::MagicInfo + 0x20 + 0x50 * i, &_paxStartAddress, 0x08);

					// Overwrite the pointer to the current Magic table.
					memcpy(YS::MAGIC::MagicInfo + 0x48 + 0x50 * i, &_currentTablePtr, 0x08);

					// Initialize the current Magic PAX.
					ryj::PAX::Init(YS::MAGIC::MagicInfo + 0x18 + 0x50 * i, reinterpret_cast<char*>(_paxAddress));

					// Denote the command of the current Magic to the array.
					_commandArray.push_back(_currentTable->Command);
				};
			}
		}

		// Copy the commands array to where the Magic Commands are stored.
		_commandArray.resize(0x06);
		memcpy(YS::MAGIC::MagicCommands, _commandArray.data(), 0x0C);

		// Denote the current Magic levels for both tiers of Magic.
		MAGIC_FIRST = _magicFirst;
		MAGIC_SECOND = _magicSecond;
	}

	// If the game isn't loaded, free every single magic that we have processed ourselves.
	else if (!*YS::AREA::IsInMap)
	{
		for (auto _magicEntry : MAGIC_FILES)
			free(_magicEntry.second);

		MAGIC_FILES.clear();
	}
}

// Handles the registration and activation of movement outside of menus.
// Used *generally* with Rando, but can be used for other purposes by other mods.
void ReFined::Critical::RegisterMovement()
{
	// Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
	auto _soraGauge = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

	// See if there is specifically a Cutscene playing.
	auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);

	auto _fetchEvent = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });
	bool _isCutscene = _fetchEvent != 0x00 && *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xCAFEEFAC &&
		*reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xEFACCAFE;

	auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

	// If the game is loaded:
	if (*YS::AREA::IsInMap && _commandPointer != 0x00 && _soraGauge != 0x00 && !_isCutscene && _eventPointer == 0x00)
	{
		// If  the ability denotation is not initialized:
		if (ABILITY_ARRAY.size() == 0x00)
		{
			// Resize the denotation to be 0x60 elements.
			ABILITY_ARRAY.resize(0x60);

			// Denote all of Sora's current abilities and sort them.
			memcpy(ABILITY_ARRAY.data(), YS::AREA::SaveData + 0x2544, 0xC0);
			sort(ABILITY_ARRAY.begin(), ABILITY_ARRAY.end());
		}

		// Create the vectors for current ability calculations.
		vector<uint16_t> _abilityDiff;
		vector<uint16_t> _currentAbility(0x60);

		// Denote all of Sora's current abilities and sort them.
		memcpy(_currentAbility.data(), YS::AREA::SaveData + 0x2544, 0xC0);
		sort(_currentAbility.begin(), _currentAbility.end());

		// Get all the abilities that differ between the old and current denotation.
		set_difference(_currentAbility.begin(), _currentAbility.end(), ABILITY_ARRAY.begin(), ABILITY_ARRAY.end(), inserter(_abilityDiff, _abilityDiff.begin()));
		
		// Check if any of the different abilities contain movement.
		bool _fetchMovement = any_of(_abilityDiff.begin(), _abilityDiff.end(), [](int x) {
			return (x >= 0x805E && x <= 0x806D) || (x >= 0x8234 && x <= 0x8237) || (x == 0x0194 || x == 0x8194);
			});

		// If they do:
		if (_fetchMovement)
		{
			// Refresh all of Sora's stats, which in turn, will commit all movement changes.
			YS::SORA::RefreshAbilities(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x1D0);
		}

		// Copy over the current ability list to the denotation array.
		ABILITY_ARRAY.assign(_currentAbility.begin(), _currentAbility.end());
	}
}

// Allows for manual pop-ups of the Information and Prize windows.
// ENABLE LINE => 0x800000, TEXT INFO => 0x800004[0x100], TEXT PRIZE => 0x800104[0x20].
// Used *generally* with Archipelago, but can be used for other purposes by other mods.
void ReFined::Critical::ShowInformation()
{
	// Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
	auto _soraGauge = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

	auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

	// See if there is specifically a Cutscene playing.
	auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);

	auto _fetchEvent = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });
	bool _isCutscene = _eventPointer != nullptr && _fetchEvent != 0x00 && *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xCAFEEFAC &&
																		  *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xEFACCAFE;

	// If the game is loaded, and there isn't a menu present, and it's not a cutscene:
	if (*YS::AREA::IsInMap && _commandPointer != 0x00 && !*YS::MENU::IsMenu && !_isCutscene && _soraGauge != 0x00 && _eventPointer == 0x00)
	{
		// Fetch the fade status and the enable line.
		auto _fetchFade = *(dk::JUMPEFFECT::FadeStatus + 0x108);
		auto _fetchEnable = moduleInfo.startAddr[0x800000];

		// If there is no fade, and the enable line is set:
		if (_fetchFade == 0x00 && _fetchEnable != 0x00)
		{
			// Reset the enable line.
			*const_cast<char*>(moduleInfo.startAddr + 0x800000) = 0x00;

			// If the enable line is 0x01, summon INFORMATION. If it's 0x02, summon PRIZE.
			switch (_fetchEnable)
			{
				case 0x01:
					dk::INFORMATION::openInformationWindow(moduleInfo.startAddr + 0x800004);
					break;

				case 0x02:
					dk::TREASURE_INFO::openPrizeWindow(moduleInfo.startAddr + 0x800104);
					break;

				case 0x03:
					dk::TREASURE_INFO::openBoxWindow(moduleInfo.startAddr + 0x800154, *reinterpret_cast<const uint16_t*>(moduleInfo.startAddr + 0x800150));
			}
		}
	}
}

// Processes Sora's/Roxas' death when his HP is 0.
void ReFined::Critical::ProcessDeath()
{
	// Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
	auto _soraGauge = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

	// Fetch Sora's pointer as well as his UCM.
	auto _soraSelf = *reinterpret_cast<const uint64_t*>(YS::SORA::pint_sora);
	auto _fetchSora = *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable);

	// If Sora's HP is 0, and he isn't Mermaid Sora, and his gauge is present, and he isn't dead:
	if (*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308) == 0x00 && *YS::AREA::IsInMap && !*YS::MENU::IsMenu && (_fetchSora != 0x03BE && _fetchSora != 0x0656) && _soraGauge != 0x00 && !IS_DEAD)
	{
		// Process his death and mark it.
		YS::SORA::AddHP(reinterpret_cast<char*>(_soraSelf), 0x00, 0x00, false);
		IS_DEAD = true;
	}

	// If Sora's HP is NOT 0 but he is dead, mark him as not.
	else if (*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308) != 0x00 && IS_DEAD)
		IS_DEAD = false;
}

// Handles Sora's Crown on the HUD.
// Has unforeseen side effects that I will have to work around.
// TODO: Multi-Audio implementation.
void ReFined::Critical::HandleCrown()
{
	// Fetch Sora's UCM.
	auto _fetchCharacter = *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable);

	// Fetch the inventory slots for the Proofs.
	auto _fetchCrown = new char[0x03];
	memcpy(_fetchCrown, YS::AREA::SaveData + 0x36B2, 0x03);

	// Add the Proof Counts together and see the result.
	uint8_t _calculateCrown = _fetchCrown[0] + _fetchCrown[1] + _fetchCrown[2];

	// Check if we are in a place that needs crown application.
	auto _areaCheck = YS::AREA::Current->World >= 0x02 && YS::AREA::Current->World != 0x0F;

	// Check if Sora's gauge exists.
	auto _gaugeSora = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

	// If in the title, or the game is loaded, or there was a Drive Form change, or there was a crown change: Reset everything.
	if (*YS::TITLE::IsTitle || !*YS::AREA::IsInMap || PAST_FORM != *(YS::AREA::SaveData + 0x3524) || PAST_CROWN != _calculateCrown)
	{
		FACE_APPLIED = false;
		SORA_PORTRAIT = 0x00;
		PAST_CROWN = _calculateCrown;
		PAST_FORM = *(YS::AREA::SaveData + 0x3524);
	}

	// If not in title, and game's loaded, and the area check passes, and Sora's Portrait is not parsed or reset, and Sora is regular KH2 Sora, and Sora's gauge exists, and the patch is not applied:
	if (!*YS::TITLE::IsTitle && *YS::AREA::IsInMap && _areaCheck && SORA_PORTRAIT == 0x00 && _calculateCrown > 0x00 && _fetchCharacter == 0x0054 && _gaugeSora != 0x00 && !FACE_APPLIED)
	{
		// Fetch Sora's APDX file from the Cache Buffer.
		auto _fileName = "obj/" + SORA_FORMS.at(*(YS::AREA::SaveData + 0x3524)) + ".a.us";
		SORA_PORTRAIT = YS::CACHE_BUFF::SearchByName(const_cast<char*>(_fileName.c_str()), -1);

		// If Sora's APDX does not exist, exit out.
		if (SORA_PORTRAIT == 0x00)
			return;

		// Fetch Sora's PAX Pointer from the APDX file.
		auto _paxPointer = reinterpret_cast<char*>(*reinterpret_cast<const uint64_t*>(SORA_PORTRAIT + 0x58));

		// If it does not exist, exit out.
		if (_paxPointer == 0x00)
			return;

		// Prepare for some calculations.
		uint64_t _readSize = 0x40;

		// Read the BAR offset and the face size.
		auto _barOffset = *reinterpret_cast<const uint32_t*>(_paxPointer + 0x08);
		auto _checkFaceSizes = *reinterpret_cast<const uint32_t*>(_paxPointer + 0x3C);

		// Read all the sizes needed and add them in.
		for (uint32_t i = 0x00; i < 0x02; i++)
			_readSize += *reinterpret_cast<const uint32_t*>(_paxPointer + 0x1CU + (0x10 * i));

		// If the face isn't the size we expect, assume a non-compatible APDX is being used and consider the patch applied.
		if (_checkFaceSizes != 0x04EC && _checkFaceSizes != 0x0000)
		{
			FACE_APPLIED = true;
			return;
		}

		// If the BAR is not initialized, exit out.
		if (_barOffset == 0x00 || _readSize == 0x40)
			return;

		// Fetch the name of a sub-file.
		string _fetchFace(_paxPointer + 0x24, 0x04);

		// If that sub file is not a "face" file, exit out.
		if (_fetchFace != "face")
			return;

		// Calculate the Y coordinates for the SEQD entries based on the current crown.
		short _topValue = 0x00 + _calculateCrown * 0x5A;
		short _bottomValue = 0x5D + _calculateCrown * 0x5A;

		char* _topAddress = _paxPointer + _readSize + 0x38;
		char* _bottomAddress = _paxPointer + _readSize + 0x40;

		auto _readBottom = *reinterpret_cast<const short*>(_bottomAddress);

		// If the bottom address did not read, exit out.
		if (_readBottom == 0x00)
			return;

		// For every face (REGULAR, HURT, CRITICAL), apply the Y coords.
		for (uint32_t i = 0x00; i < 0x03; i++)
		{
			memcpy(_topAddress + 0x2C * i, &_topValue, 0x02);
			memcpy(_bottomAddress + 0x2C * i, &_bottomValue, 0x02);
		}

		// Denote the patch was applied.
		FACE_APPLIED = true;
	}
}

// Adds the ability to Retry battles in the game.
void ReFined::Critical::RetryBattles()
{
	// Declare the worlds and rooms in which Retry **cannot** execute.
	auto _checkBlacklist = (YS::AREA::Current->World == 0x04 && YS::AREA::Current->Room >= 0x15 && YS::AREA::Current->Room <= 0x1A) ||
						   (YS::AREA::Current->World == 0x12 && ((YS::AREA::Current->Room >= 0x16 && YS::AREA::Current->Room <= 0x1C) || YS::AREA::Current->Room == 0x14));

	// If the arrays are not yet initialize, initialize them.
	if (INST_MAPJUMPTASK.size() == 0x00)
	{
		INST_MAPJUMPTASK.resize(0x05);
		INST_CONTINUELOAD.resize(0x05);

		INST_CAMPBITWISE.resize(0x07);
		INST_CAMPINIT.resize(0x08);

		memcpy(INST_MAPJUMPTASK.data(), reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, 0x05);
		memcpy(INST_CONTINUELOAD.data(), reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, 0x05);

		memcpy(INST_CAMPBITWISE.data(), reinterpret_cast<char*>(CAMP_OFFSET) + 0x1A7, 0x07);
		memcpy(INST_CAMPINIT.data(), reinterpret_cast<char*>(CAMPINIT_OFFSET), 0x08);
	}

	// If the room we are in is not blacklisted:
	if (!_checkBlacklist)
	{
		// Check if we are in a cutscene.
		const char* _eventPointer = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });
		bool _isCutscene = _eventPointer != 0x00 && *reinterpret_cast<const uint32_t*>(_eventPointer) != 0xCAFEEFAC && *reinterpret_cast<const uint32_t*>(_eventPointer) != 0xEFACCAFE;

		// If not in a cutscene, and is in a Boss Battle, and Retry State is not denoted:
		if (!_isCutscene && *YS::AREA::BattleStatus == 0x02 && RETRY_STATE.size() == 0x00)
		{
			// Check if on Hades Escape.
			HADES_ESCAPE = YS::AREA::Current->World == 0x06 && YS::AREA::Current->Room == 0x05 && YS::AREA::Current->Set.Event == 0x6F;

			// If on Hades Escape, denote and initialize it.
			if (HADES_ESCAPE && HADES_ITERATOR == 0xFF)
				HADES_ITERATOR = 0x00;

			// Read the current state of the room for Retry.
			RETRY_STATE.resize(0x10FC0);
			memcpy(RETRY_STATE.data(), YS::AREA::SaveData, 0x10FC0);

			// Add the entries for Retry as well as Prepare.
			ReFined::Continue::Add(0x00, PREPARE_ENTRY);
			ReFined::Continue::Add(0x00, RETRY_ENTRY);
		}

		// If not retrying, and is Hades Escape:
		if (RETRY_MODE == 0x00 && HADES_ESCAPE && HADES_ITERATOR != 0xFF)
		{
			// If Hades Escape hit an intermission, note it down.
			if (*YS::AREA::BattleStatus == 0x01 && !HADES_CHANGED)
			{
				HADES_ITERATOR++;
				HADES_CHANGED = true;
			}

			// If Hades Escape is in battle once again, note it down.
			if (*YS::AREA::BattleStatus != 0x01 && HADES_CHANGED)
				HADES_CHANGED = false;

			// If there have been 3 intermissions:
			if (HADES_ITERATOR == 0x03)
			{
				// Reset Hades Escape variables.
				HADES_ITERATOR = 0xFF;
				HADES_ESCAPE = false;

				// Reinstate Area Initializers to allow for game progression.
				memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
				memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);
			}
		}

		// If on title or the battle isn't a Boss Battle:
		else if (*YS::TITLE::IsTitle || *YS::AREA::BattleStatus != 0x02)
		{
			// If retrying, not on title, and Retry State has been noted:
			if (RETRY_MODE != 0x00 && RETRY_STATE.size() != 0x00 && !*YS::TITLE::IsTitle)
			{
				// Restore the Retry State.
				memcpy(const_cast<char*>(YS::AREA::SaveData), RETRY_STATE.data(), 0x10FC0);

				// If Retry Mode is 0x02, meaning a Prepare Menu has been requested:
				if (RETRY_MODE == 0x02)
				{
					// Prepare the NOP state.
					vector<uint8_t> _nopArrayCamp(0x08);
					fill(_nopArrayCamp.begin(), _nopArrayCamp.begin() + 0x08, 0x90);

					// Write the camp menu options bitwise (Items, Abilities, Customize, Party)
					uint8_t _campBitwise = 0x0F;
					memcpy(YS::MENU::CampOptions, &_campBitwise, 0x01);

					// NOP the instructions that set and refresh the camp menu options bitwise.
					memcpy(CAMPINIT_OFFSET, _nopArrayCamp.data(), 0x08);
					memcpy(CAMP_OFFSET + 0x1A7, _nopArrayCamp.data(), 0x07);

					// Summon the camp menu.
					YS::MENU::CampStart(0x00, 0x00);
				}

				// Denote we are no longer retrying.
				RETRY_MODE = 0x00;
			}

			// If we are NOT retrying and not in a menu, but the Retry State has been denoted:
			if (RETRY_MODE == 0x00 && RETRY_STATE.size() != 0x00 && !*YS::MENU::IsMenu)
			{
				// Restore the Area Init functions so we can progress.
				memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
				memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);

				// Restore all Camp initialization instructions.
				memcpy(CAMPINIT_OFFSET, INST_CAMPINIT.data(), 0x08);
				memcpy(CAMP_OFFSET + 0x1A7, INST_CAMPBITWISE.data(), 0x07);

				// If the continue menu has been edited, remove all Retry elements.
				if (ReFined::Continue::Children.size() > 0x02)
				{
					ReFined::Continue::Remove(0x01);
					ReFined::Continue::Remove(0x00);
				}

				// Clear the Retry State.
				RETRY_STATE.clear();
			}
		}

		// Fetch the current dialog state (Necessary for Game Over screen.)
		auto _fetchDialog = CalculatePointer(YS::MENU::pint_dialogbase, { 0xD48 });

		// Fetch Menu selection and the pointer to the Game Over screen.
		uint8_t _fetchSelectMenu = _fetchDialog != 0x00 ? *_fetchDialog : 0x80;
		uint64_t _continuePoint = *reinterpret_cast<const uint64_t*>(YS::MENU::pint_gameover);

		// If the Game Over menu exists and the Retry State has been noted:
		if (_continuePoint != 0x00 && RETRY_STATE.size() != 0x00)
		{
			// If on Hades Escape, reset all Hades Escape variables.
			if (HADES_ESCAPE && HADES_ITERATOR != 0x00)
			{
				HADES_ITERATOR = 0x00;
				HADES_CHANGED = false;
			}

			// If the menu selection is valid:
			if (_fetchSelectMenu <= 0x04)
			{
				// Determine the Retry Mode based on selection.
				RETRY_MODE = _fetchSelectMenu == 0x00 ? 0x01 : (_fetchSelectMenu == 0x01 ? 0x02 : 0x00);
				RETRY_MODE = *reinterpret_cast<const uint8_t*>(YS::MENU::SubMenuType) == 0xFF ? RETRY_MODE : 0x00;

				// If we are retrying, NOP all Area Init instructions to not change the room state.
				if (RETRY_MODE != 0x00)
				{
					char* _nopArray = new char[0x05];
					fill(_nopArray, _nopArray + 0x05, 0x90);

					memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, _nopArray, 0x05);
					memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, _nopArray, 0x05);
				}

				// If we are not, restore the prior instructions.
				else
				{
					memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
					memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);
				}
			}
		}

		// If we are retrying, we are still in a boss battle, and the command menu type is "MICKEY"
		else if (RETRY_MODE > 0x00 && *YS::AREA::BattleStatus == 0x02 && *COMMAND_TYPE_ADDR == 0x01)
		{
			// Restore the Area Init functions so we don't loop after Mickey leaves.
			memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
			memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);

			// If the continue menu has been edited, remove all Retry elements.
			if (ReFined::Continue::Children.size() > 0x02)
			{
				ReFined::Continue::Remove(0x01);
				ReFined::Continue::Remove(0x00);
			}

			// Denote we are no longer retrying.
			RETRY_MODE = 0x00;
		}
	}
}

void ReFined::Critical::HandleConfiguration()
{
	auto _configMemory = *reinterpret_cast<const uint64_t*>(YS::MENU::pint_config);
	auto _configBitwise = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A4);

	auto _menuOK = *YS::MENU::MenuType == 0x08 && (*YS::MENU::SubMenuType == 0x24 || *(YS::MENU::SubMenuType + 0x04) == 0x24);

	if (!*YS::TITLE::IsTitle)
	{
		if (!CONFIG_FIRST_INIT)
		{
			ReFined::Critical::CONTROLLER_MODE = (_configBitwise & 0x2000) == 0x2000;
			ReFined::Critical::SAVE_MODE = (_configBitwise & 0x0004) == 0x0004 ? 0x00 : ((_configBitwise & 0x0002) == 0x0002 ? 0x01 : 0x02);

			bool _isQuadratum = *(YS::AREA::SaveData + 0x41A7);
			const char* _fileName = _isQuadratum ? "field2d/%s/qd0command.2dd" : "field2d/%s/zz0command.2dd";

			memcpy(COMMAND_MENU_ADDR, _fileName, 0x19);
			memcpy(COMMAND_MENU_ADDR + 0x20, _fileName + 0x0B, 0x0E);

			CONFIG_FIRST_INIT = true;
		}

		if (_menuOK)
		{
			if (!CONFIG_INIT)
			{
				if (!CONFIG_FETCH)
				{
					SETTING_MEMORY = _configBitwise;
					CONFIG_FETCH = true;
				}

				bool _isQuadratum = *(YS::AREA::SaveData + 0x41A7);
				uint8_t _configSize = 0x0B;

				vector<uint8_t> _configArray
				{
					static_cast<uint8_t>((_configBitwise & 0x0010) == 0x0010 ? 0x01 : 0x00), // Field Camera
					static_cast<uint8_t>((_configBitwise & 0x0020) == 0x0020 ? 0x01 : 0x00), // Right Stick Mode
					static_cast<uint8_t>((_configBitwise & 0x0100) == 0x0100 ? 0x01 : 0x00), // Horizontal Camera Mode
					static_cast<uint8_t>((_configBitwise & 0x0080) == 0x0080 ? 0x01 : 0x00), // Vertical Camera Mode
					static_cast<uint8_t>((_configBitwise & 0x0200) == 0x0200 ? 0x00 : ((_configBitwise & 0x0400) == 0x0400 ? 0x01 : 0x02)), // Summon Animations
					static_cast<uint8_t>((_configBitwise & 0x0008) == 0x0008 ? 0x00 : 0x01), // Navigational Map
					static_cast<uint8_t>((_configBitwise & 0x0004) == 0x0004 ? 0x00 : ((_configBitwise & 0x0002) == 0x0002 ? 0x01 : 0x02)), // Autosave Mode
					static_cast<uint8_t>((_configBitwise & 0x2000) == 0x2000 ? 0x00 : 0x01), // Controller Prompt Style
					static_cast<uint8_t>((_configBitwise & 0x0001) == 0x0001 ? 0x00 : 0x01), // Vibration
					static_cast<uint8_t>(_isQuadratum ? 0x02 : ((_configBitwise & 0x0040) == 0x0040 ? 0x01 : 0x00)), // Command Menu
					static_cast<uint8_t>(*(YS::AREA::SaveData + 0x2498)) // Difficulty
				};

				if (_configMemory == 0x00)
					return;

				memcpy(reinterpret_cast<char*>(_configMemory), _configArray.data(), _configArray.size());

				if (*reinterpret_cast<const int*>(YS::MENU::MenuType + 0x5C) == 0xAF20)
				{
					char _nopArray[0x04];
					char _menuSelectInst[0x04];

					fill(_nopArray, _nopArray + 0x04, 0x90);

					memcpy(_menuSelectInst, MENUSELECT_OFFSET + 0x46, 0x04);
					memcpy(MENUSELECT_OFFSET + 0x46, _nopArray, 0x04);

					auto _pointPage = CalculatePointer(YS::MENU::pint_suboptionselect, { 0x12 });
					auto _pointCurrent = CalculatePointer(YS::MENU::pint_suboptionselect, { 0x00 });
					auto _pointMaximum = CalculatePointer(YS::MENU::pint_suboptionselect, { 0x16 });

					memcpy(const_cast<char*>(_pointMaximum), &_configSize, 0x01);

					uint8_t _pageCurrent = *_pointPage;
					uint8_t _indexCurrent = *_pointCurrent;
					uint8_t _pageAmount = _configSize - 0x09;

					if (_pageCurrent >= _pageAmount)
					{
						uint8_t _indexCalcuation = _indexCurrent + (_pageCurrent - _pageAmount);

						memcpy(const_cast<char*>(_pointPage), &_pageAmount, 0x01);
						memcpy(const_cast<char*>(_pointCurrent), &_indexCalcuation, 0x01);
					}

					Tz::CmConfig::UpdateList();
					Tz::CmConfig::UpdateActive();

					memcpy(MENUSELECT_OFFSET + 0x46, _menuSelectInst, 0x04);
				}

				CONFIG_INIT = true;
			}

			if (*reinterpret_cast<const int*>(YS::MENU::MenuType + 0x5C) == 0xAF20)
			{
				uint8_t _fetchConfig[0x0B];
				memcpy(_fetchConfig, reinterpret_cast<char*>(_configMemory), 0x0B);

				uint16_t _constructBitwise = (_fetchConfig[0x00] == 0x01 ? 0x0010 : 0x0000) |
					(_fetchConfig[0x01] == 0x01 ? 0x0020 : 0x0000) |
					(_fetchConfig[0x02] == 0x01 ? 0x0100 : 0x0000) |
					(_fetchConfig[0x03] == 0x01 ? 0x0080 : 0x0000) |
					(_fetchConfig[0x04] == 0x00 ? 0x0200 : (_fetchConfig[0x04] == 0x01 ? 0x0400 : 0x0000)) |
					(_fetchConfig[0x05] == 0x00 ? 0x0008 : 0x0000) |
					(_fetchConfig[0x06] == 0x00 ? 0x0004 : ((_fetchConfig[0x06] == 0x01 ? 0x0002 : 0x0000))) |
					(_fetchConfig[0x07] == 0x00 ? 0x2000 : 0x0000) |
					(_fetchConfig[0x08] == 0x00 ? 0x0001 : 0x0000) |
					(_fetchConfig[0x09] >= 0x01 ? 0x0040 : 0x0000);

				bool _switchFlag = true;
				bool _isQuadratum = _fetchConfig[0x09] == 0x02;

				const char* _fileName = _isQuadratum ? "field2d/%s/qd0command.2dd" : "field2d/%s/zz0command.2dd";

				ReFined::Critical::CONTROLLER_MODE = (_constructBitwise & 0x2000) == 0x2000;
				ReFined::Critical::SAVE_MODE = (_constructBitwise & 0x0004) == 0x0004 ? 0x00 : ((_constructBitwise & 0x0002) == 0x0002 ? 0x01 : 0x02);

				memcpy(YS::AREA::SaveData + 0x41A7, &_isQuadratum, 0x01);

				memcpy(COMMAND_MENU_ADDR, _fileName, 0x19);
				memcpy(COMMAND_MENU_ADDR + 0x20, _fileName + 0x0B, 0x0E);

				memcpy(YS::AREA::SaveData + 0x41A4, &_constructBitwise, 0x02);
				memcpy(COMMAND_FLAG_ADDR, &_switchFlag, 0x01);
			}

			auto _pointLayout = CalculatePointer(YS::MENU::pint_camp2ld, { 0x00 });
			auto _pointCurrent = CalculatePointer(YS::MENU::pint_suboptionselect, { 0x12 });

			if (_pointCurrent != nullptr)
			{
				uint32_t _barFileOffset = *reinterpret_cast<const uint32_t*>(_pointLayout + 0x08);
				uint32_t _campFileOffset = *reinterpret_cast<const uint32_t*>(_pointLayout + 0x28) - _barFileOffset;

				uint8_t _pageCount = 0x0B - 0x09;
				uint8_t _pageFactor = 0x18 * _pageCount;

				auto _pageOffset = (_pageFactor / _pageCount) * *_pointCurrent;

				auto _offsetCalc = 0x64 + _pageOffset;
				auto _floatCalc = (0xC0 - _pageFactor) * 0.01F;
				auto _factorCalc = 0x64 - (_pageFactor + 0x01) + _pageOffset;

				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x21498), &_offsetCalc, 0x04);
				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x2149C), &_offsetCalc, 0x04);
				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x21528), &_offsetCalc, 0x04);
				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x2152C), &_offsetCalc, 0x04);

				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x21568), &_floatCalc, 0x04);
				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x2156C), &_floatCalc, 0x04);

				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x215B8), &_factorCalc, 0x04);
				memcpy(const_cast<char*>(_pointLayout + _campFileOffset + 0x215BC), &_factorCalc, 0x04);
			}
		}

		else if (!_menuOK && CONFIG_INIT)
			CONFIG_INIT = false;
	}

	else if (*YS::TITLE::IsTitle && CONFIG_FIRST_INIT)
		CONFIG_FIRST_INIT = false;
}

void ReFined::Critical::HandleIntro()
{
	if (*YS::TITLE::IsTitle)
	{
		if (INTRO_APPLIED)
			INTRO_APPLIED = false;

		ENFORCE_INTRO = *YS::TITLE::IntroSelect == 0x00;

		SETTING_MEMORY = 0x0408 | 
			(*(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x204) == 0x00 ? 0x0001 : 0x0000) |
			(*(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x208) == 0x00 ? 0x0004 : (*(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x208) == 0x01 ? 0x0002 : 0x0000)) |
			(*(ReFined::MemoryManager::Fetch("INTRO_MEMORY") + 0x20C) == 0x00 ? 0x2000 : 0x0000);
	}

	else if (!*YS::TITLE::IsTitle && !INTRO_APPLIED && ENFORCE_INTRO && YS::AREA::Current->World == 0x02 && (YS::AREA::Current->Room == 0x01 || YS::AREA::Current->Room == 0x20))
	{
		memcpy(YS::AREA::SaveData + 0x41A4, &SETTING_MEMORY, 0x02);

		ReFined::Critical::SAVE_MODE = (SETTING_MEMORY & 0x0004) ? 0x00 : ((SETTING_MEMORY & 0x0002) == 0x02 ? 0x01 : 0x02);
		ReFined::Critical::CONTROLLER_MODE = (SETTING_MEMORY & 0x2000) == 0x2000;

		ENFORCE_INTRO = false;
		INTRO_APPLIED = true;
		CONFIG_FIRST_INIT = true;
	}
}

void ReFined::Critical::AspectCorrection()
{
	float _resolutionHorizontal = *reinterpret_cast<float*>(VIEWPORT3D_ADDR + 0x10); 
	float _resolutionVertical = *reinterpret_cast<float*>(VIEWPORT3D_ADDR + 0x14);

	float _commonDiv = 0x00F;

	if (_resolutionHorizontal != 0x00 && _resolutionVertical != 0x00)
	{
		uint32_t _offsetPositive = 0x55;
		uint32_t _offsetNegative = 0xFFFFFFAB;

		short _offsetInformation = -1;

		auto _tempWidth = _resolutionHorizontal;
		auto _tempHeight = _resolutionVertical;

		while (_tempWidth != 0 && _tempHeight != 0)
		{
			if (_tempWidth > _tempHeight)
				_tempWidth = fmodf(_tempWidth, _tempHeight);

			else
				_tempHeight = fmodf(_tempHeight, _tempWidth);
		}

		_commonDiv = _tempWidth == 0 ? _tempHeight : _tempWidth;

		auto _ratioNum = _resolutionHorizontal / _commonDiv;
		auto _checkRatio = _resolutionVertical / _commonDiv;

		if (_checkRatio != 9)
		{
			auto _commonMulp = 9 / _checkRatio;
			_ratioNum = _ratioNum * _commonMulp;
		}

		auto _ratioMultiplier = 0.0625F * _ratioNum;

		memcpy(VIEWPORT3D_ADDR + 0x20, &_ratioMultiplier, 0x04);

		auto _heightFactor = _resolutionVertical / 1080;
		auto _widthCalc = floorf(_resolutionHorizontal / _heightFactor);

		_offsetPositive = ceilf(0.177F * (_widthCalc - 1440));
		_offsetNegative = _offsetPositive * -1;

		if (_ratioNum != 16)
			_offsetInformation = floorf(-0.11 * (_widthCalc - 1440));

		if (POSITIVE_ASPECT_BYTE.size() == 0x00)
		{
			for (int i = 0xB8; i < 0xC0; i++)
			{
				stringstream stream;
				stream << char(i);

				auto _listPos = MultiSignatureScan(stream.str().append("\x55\x00\x00\x00").c_str(), "xxxxx");
				auto _listNeg = MultiSignatureScan(stream.str().append("\xAB\xFF\xFF\xFF").c_str(), "xxxxx");

				for (auto _ptr : _listPos)
				{
					if (_ptr > moduleInfo.startAddr + 0x1A0000)
						continue;

					POSITIVE_ASPECT_BYTE.push_back(_ptr);
				}

				for (auto _ptr : _listNeg)
				{
					if (_ptr > moduleInfo.startAddr + 0x1A0000)
						continue;

					NEGATIVE_ASPECT_BYTE.push_back(_ptr);
				}
			}
		}

		if (POSITIVE_ASPECT_BYTE.size() == 0x00)
		{
			for (int i = 0xB8; i < 0xC0; i++)
			{
				stringstream stream;
				stream << char(i);

				auto _listPos = MultiSignatureScan("\xC7\x00\x00\x00\x55\x00\x00\x00", "x???xxxx");

				for (auto _ptr : _listPos)
				{
					if (_ptr > moduleInfo.startAddr + 0x185000)
						continue;

					POSITIVE_ASPECT_SHORT.push_back(_ptr);
				}
			}
		}

		for (int i = 0; i < NEGATIVE_ASPECT_SHORT.size(); i++)
			memcpy(NEGATIVE_ASPECT_SHORT[i] + 0x04, &_offsetNegative, 0x04);

		for (int i = 0; i < POSITIVE_ASPECT_LONG.size(); i++)
			memcpy(POSITIVE_ASPECT_LONG[i] + 0x06, &_offsetPositive, 0x04);
		for (int i = 0; i < NEGATIVE_ASPECT_LONG.size(); i++)
			memcpy(NEGATIVE_ASPECT_LONG[i] + 0x06, &_offsetNegative, 0x04);

		for (int i = 0; i < POSITIVE_ASPECT_BYTE.size(); i++)
			memcpy(POSITIVE_ASPECT_BYTE[i] + 0x01, &_offsetPositive, 0x04);
		for (int i = 0; i < NEGATIVE_ASPECT_BYTE.size(); i++)
			memcpy(NEGATIVE_ASPECT_BYTE[i] + 0x01, &_offsetNegative, 0x04);

		for (int i = 0; i < _fetchSimpleCont.size(); i++)
			memcpy(_fetchSimpleCont[i] + 0x03, &_offsetNegative, 0x04);

		memcpy(INFORMATION_OFFSET + 0x0B, &_offsetInformation, 0x02);

		auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);
		auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

		if ((_commandPointer == 0x00 || _eventPointer != 0x00 || !*YS::AREA::IsInMap) && _offsetPositive != 0x55)
		{
			vector<char*> _fetchMission =
			{
				YS::CACHE_BUFF::SearchByName("msn/us/EH26_MS108.bar", -1),
				YS::CACHE_BUFF::SearchByName("msn/us/EH14_MS103.bar", -1),
				YS::CACHE_BUFF::SearchByName("msn/us/HB33_FM_LEX.bar", -1)
			};

			bool _isFetched = any_of(_fetchMission.begin(), _fetchMission.end(), [](char* x) {
				return (x != nullptr);
				});

			if (_isFetched)
			{
				uint32_t _positiveDefault = 0x55;
				uint32_t _negativeDefault = -0x55;

				memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x21, &_positiveDefault, 0x04);
				memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x29, &_negativeDefault, 0x04);
			}
		}

		else if (_commandPointer != 0x00 && _eventPointer == 0x00 && *YS::AREA::IsInMap)
		{
			memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x21, &_offsetPositive, 0x04);
			memcpy(ReFined::MemoryManager::Fetch("GAUGE_ASPECT_OVERRIDE") + 0x29, &_offsetNegative, 0x04);
		}
	}
}