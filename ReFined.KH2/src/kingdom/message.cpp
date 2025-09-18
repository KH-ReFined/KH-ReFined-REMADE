#include "message.h"
#include "SigScan.h"
#include <Windows.h>

YS::MESSAGE::GetData_t YS::MESSAGE::GetData = SignatureScan<YS::MESSAGE::GetData_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x50\x45\x33\xF6\x48\x63\xE9\x33\xF6\x48\x8D\x3D\x00\x00\x00\x00\x4C\x8D\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????");

string YS::MESSAGE::DecodeKHSCII(const char* Input)
{
	map<uint8_t, char> _specialDict
	{
		{ 0x01, ' ' },
		{ 0x02, '\n' },
		{ 0x54, '-' },
		{ 0x2C, '-' },
		{ 0x66, '-' },
		{ 0x55, 'ー' },
		{ 0x48, '!' },
		{ 0x49, '?' },
		{ 0x4A, '%' },
		{ 0x4B, '/' },
		{ 0x4F, '.' },
		{ 0x50, ',' },
		{ 0x51, ';' },
		{ 0x52, ':' },
		{ 0x57, '\'' },
		{ 0x5A, '('},
		{ 0x5B, ')'},
		{ 0x62, '['},
		{ 0x63, ']'},
		{ 0xB7, 'à'},
		{ 0xB8, 'á'},
		{ 0xB9, 'â'},
		{ 0xBA, 'ä'},
		{ 0xBB, 'è'},
		{ 0xBC, 'é'},
		{ 0xBD, 'ê'},
		{ 0xBE, 'ë'},
		{ 0xBF, 'ì'},
		{ 0xC0, 'í'},
		{ 0xC1, 'î'},
		{ 0xC2, 'ï'},
		{ 0xC3, 'ñ'},
		{ 0xC4, 'ò'},
		{ 0xC5, 'ó'},
		{ 0xC6, 'ô'},
		{ 0xC7, 'ö'},
		{ 0xC8, 'ù'},
		{ 0xC9, 'ú'},
		{ 0xCA, 'û'},
		{ 0xCB, 'ü'},
		{ 0xE8, 'ç'},
		{ 0xD0, 'À'},
		{ 0xD1, 'Á'},
		{ 0xD2, 'Â'},
		{ 0xD3, 'Ä'},
		{ 0xD4, 'È'},
		{ 0xD5, 'É'},
		{ 0xD6, 'Ê'},
		{ 0xD7, 'Ë'},
		{ 0xD8, 'Ì'},
		{ 0xD9, 'Í'},
		{ 0xDA, 'Î'},
		{ 0xDB, 'Ï'},
		{ 0xDC, 'Ñ'},
		{ 0xDD, 'Ò'},
		{ 0xDE, 'Ó'},
		{ 0xDF, 'Ô'},
		{ 0xE0, 'Ö'},
		{ 0xE1, 'Ù'},
		{ 0xE2, 'Ú'},
		{ 0xE3, 'Û'},
		{ 0xE4, 'Ü'},
		{ 0xE5, '¡'},
		{ 0xE6, '¿'},
		{ 0xE7, 'Ç'}
	};

	vector<char> _outList;

	auto _charCount = 0;
	auto _currentChar = *reinterpret_cast<const uint8_t*>(Input + _charCount);

	// Throughout the text, do:
	while (_currentChar != 0x00)
	{
		// Simple character conversion through mathematics.
		if (_currentChar >= 0x9A && _currentChar <= 0xB3)
		{
			_outList.push_back(_currentChar - 0x39);
			_charCount++;
		}

		else if (_currentChar >= 0x2E && _currentChar <= 0x47)
		{
			_outList.push_back(_currentChar + 0x13);
			_charCount++;
		}

		else if (_currentChar >= 0x90 && _currentChar <= 0x99)
		{
			_outList.push_back(_currentChar - 0x60);
			_charCount++;
		}

		else
		{
			if (_specialDict.find(_currentChar) != _specialDict.end())
				_outList.push_back(_specialDict.at(_currentChar));

			else
			{
				stringstream _charToHex;
				_charToHex << "{0x" << std::hex << _currentChar << "}";

				for (auto _chr : _charToHex.str())
					_outList.push_back(_chr);
			}
			_charCount++;
		}

		_currentChar = *reinterpret_cast<const uint8_t*>(Input + _charCount);
	}

	return string(_outList.begin(), _outList.end());
}