#include "information.h"
#include "SigScan.h"
#include <Windows.h>

dk::INFORMATION::openInformationWindow_t dk::INFORMATION::openInformationWindow = SignatureScan<dk::INFORMATION::openInformationWindow_t>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xD3", "xxxxxxxxxxxx????x????xxx????xxx");