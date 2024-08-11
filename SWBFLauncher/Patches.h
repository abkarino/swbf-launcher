#pragma once
#include "Patch.h"
#include <utility>
#include <map>

class Patches {
public:
	std::map<const char*, Patch*> patches;

	Patches();
};