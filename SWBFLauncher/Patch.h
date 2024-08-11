#pragma once
#include <vector>
#include "PatchVariant.h"
class Patch
{
public:
	std::vector<PatchVariant*> sptest;
	std::vector<PatchVariant*> gog;
	std::vector<PatchVariant*> steam;
	Patch(std::vector<PatchVariant*>& sptest, std::vector<PatchVariant*>& gog, std::vector<PatchVariant*>& steam);
	bool runPatch(GameVersion version);
private:
	Patch();
};

