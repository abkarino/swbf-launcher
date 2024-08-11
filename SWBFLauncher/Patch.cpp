#include "pch.h"
#include "Patch.h"

Patch::Patch(std::vector<PatchVariant*>& sptest, std::vector<PatchVariant*>& gog, std::vector<PatchVariant*>& steam) : sptest{ sptest }, gog{ gog }, steam{ steam }
{
}

bool Patch::runPatch(GameVersion version)
{
	bool success = true;
	if (version == SPTEST && this->sptest.size()) {
		for each (PatchVariant* p in this->sptest)
		{
			success &= p->patch();
		}
	}
	else if (version == GOG && this->gog.size()) {
		for each (PatchVariant* p in this->gog)
		{
			success &= p->patch();
		}
	}
	else if (version == STEAM && this->steam.size()) {
		for each (PatchVariant* p in this->steam)
		{
			success &= p->patch();
		}
	}
	else {
		// throw new std::exception("Wrong game version or patch not supported on the selected version.");
		success = false;
	}
	return success;
}
