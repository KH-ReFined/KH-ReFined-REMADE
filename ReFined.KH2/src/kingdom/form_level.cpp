#include "form_level.h"
#include "SigScan.h"

YS::FORM_LEVEL::Search_t YS::FORM_LEVEL::Search = SignatureScan<YS::FORM_LEVEL::Search_t>("\x48\x83\xEC\x38\x48\x63\xC2\x41\xB9\x08\x00\x00\x00", "xxxxxxxxxxxxx");