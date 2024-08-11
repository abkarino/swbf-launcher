#pragma once
#include <string>
#include <functional>
#include "Utils.h"

class PatchVariant
{
protected:
	GameVersion gameVersion;
public:
	virtual bool patch() = 0;
};

class InPlacePatchVariant : public PatchVariant {
protected:
	int RVA;
	std::string pattern;
	const byte *replacement;
	int replacementSize;
public:
	InPlacePatchVariant(GameVersion gameVersion, int RVA, const byte *replacement, int replacementSize);
	virtual bool patch();
};

class NOPInPlacePatchVariant : public InPlacePatchVariant {
public:
	NOPInPlacePatchVariant(GameVersion gameVersion, int RVA, int replacementSize);
};

class AllocatorPatch : public PatchVariant {
protected:
	int memSize;
	int protection;
	std::function<bool(LPVOID)> callback;
public:
	AllocatorPatch(GameVersion gameVersion, int memSize, int protection, std::function<bool(LPVOID)>);
	virtual bool patch();
private:
	AllocatorPatch();
};

class ReadOnlyInPlacePatchVariant : public InPlacePatchVariant {
public:
	ReadOnlyInPlacePatchVariant(GameVersion gameVersion, int RVA, const byte* replacement, int replacementSize);
	virtual bool patch();
};

class CallbackPatch : public PatchVariant {
protected:
	std::function<bool(VOID)> callback;
public:
	CallbackPatch(GameVersion gameVersion, std::function<bool(VOID)>);
	virtual bool patch();
private:
	CallbackPatch();
};