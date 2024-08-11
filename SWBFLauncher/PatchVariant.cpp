#include "pch.h"
#include "PatchVariant.h"
#include "InstanceSingleton.h"

InPlacePatchVariant::InPlacePatchVariant(GameVersion gameVersion, int RVA, const byte* replacement, int replacementSize)
{
	this->gameVersion = gameVersion;
	this->RVA = RVA;
	this->replacement = replacement;
	this->replacementSize = replacementSize;
}

bool InPlacePatchVariant::patch() {
	LPPROCESS_INFORMATION pinfo = InstanceSingleton::getInstance();
	if (pinfo == NULL || pinfo->hProcess == NULL) return false;
	PVOID base_address = InstanceSingleton::getBase();

	if (this->RVA)
		return WriteProcessMemory(pinfo->hProcess, (BYTE *)base_address + this->RVA, this->replacement, this->replacementSize, nullptr);

	return false;
}


AllocatorPatch::AllocatorPatch(GameVersion gameVersion, int memSize, int protection, std::function<bool(LPVOID)> callback) {
	this->gameVersion = gameVersion;
	this->memSize = memSize;
	this->callback = std::move(callback);
	this->protection = protection;
}

bool AllocatorPatch::patch() {
	LPPROCESS_INFORMATION pinfo = InstanceSingleton::getInstance();
	if (pinfo == NULL || pinfo->hProcess == NULL) return false;

	LPVOID ptr = VirtualAllocEx(pinfo->hProcess, NULL, this->memSize, MEM_COMMIT, this->protection);
	
	if (!ptr)
		::MessageBox(NULL, wstring(L"Failed to create memory.\n").append(GetLastErrorAsString()).c_str(), TEXT("Error"), MB_ICONERROR);

	if (ptr && this->callback)
		return this->callback(ptr);

	return false;
}

NOPInPlacePatchVariant::NOPInPlacePatchVariant(GameVersion gameVersion, int RVA, int replacementSize) : InPlacePatchVariant(gameVersion, RVA, new byte[replacementSize], replacementSize)
{
	memset((void *)this->replacement, 0x90, replacementSize);
}

ReadOnlyInPlacePatchVariant::ReadOnlyInPlacePatchVariant(GameVersion gameVersion, int RVA, const byte* replacement, int replacementSize) : InPlacePatchVariant(gameVersion, RVA, replacement, replacementSize)
{
}

bool ReadOnlyInPlacePatchVariant::patch() {
	LPPROCESS_INFORMATION pinfo = InstanceSingleton::getInstance();
	if (pinfo == NULL || pinfo->hProcess == NULL) return false;
	PVOID base_address = InstanceSingleton::getBase();

	DWORD oldProtect;
	bool result = VirtualProtectEx(pinfo->hProcess, (BYTE*)base_address + this->RVA, this->replacementSize, PAGE_READWRITE, &oldProtect);

	if (result) {
		if (this->RVA)
			result = WriteProcessMemory(pinfo->hProcess, (BYTE*)base_address + this->RVA, this->replacement, this->replacementSize, nullptr);
		else result = false;
		VirtualProtectEx(pinfo->hProcess, (BYTE*)base_address + this->RVA, this->replacementSize, oldProtect, &oldProtect);
	}
	
	return result;
}


CallbackPatch::CallbackPatch(GameVersion gameVersion, std::function<bool(VOID)> callback) {
	this->gameVersion = gameVersion;
	this->callback = std::move(callback);
}

bool CallbackPatch::patch() {
	if (this->callback)
		return this->callback();

	return false;
}