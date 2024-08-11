
#include "pch.h"
#include "Patches.h"
#include "InstanceSingleton.h"

Patches::Patches()
{
	patches.insert(std::pair<const char*, Patch*>(
		"HighRes",
		new Patch(
			std::vector<PatchVariant*>{
				// allow higher penality limit for low res Vehicle
				new InPlacePatchVariant(SPTEST, 0x592D41 + 1, (BYTE*)"\x00\xF0\x55\x00", 4),	//0x100*22000
				new InPlacePatchVariant(SPTEST, 0x192d46 + 1, (BYTE*)"\x00\xF0\x55\x00", 4),	//0x100*22000
				// allow higher penality limit for high res Vehicle
				new InPlacePatchVariant(SPTEST, 0x192d4b + 1, (BYTE*)"\x00\xA0\x86\x01", 4),	//0x100*100000
				// allow higher penality limit for low res soldier
				new InPlacePatchVariant(SPTEST, 0x192D17, (BYTE*)"\x68\x00\xE8\x03\x00", 5),	//0x100*1000
				// allow higher penality limit for high res soldier
				new InPlacePatchVariant(SPTEST, 0x192D1F, (BYTE*)"\x68\x00\x10\x27\x00", 5),	//0x100*10000
				// allow more renderable items
				// requires Trampoline, hardwareBreakpoint or some other form of patching out of place due to the low size of the instruction
				new InPlacePatchVariant(SPTEST, 0x192CF7, (BYTE*)"\x68\x00\x02\x00\x00\xEB\x28", 7),	//push 200; jmp sptest.592D26
				new InPlacePatchVariant(SPTEST, 0x192D24, (BYTE*)"\xEB\xD1", 2),						//jmp sptest.592CF7
				// allow more than 10 high res human animation
				new InPlacePatchVariant(SPTEST, 0x7E12E, (BYTE*)"\x68\x10\x40\x40\x00", 5),		//100*0x2020+16
				new InPlacePatchVariant(SPTEST, 0x7E142, (BYTE*)"\x6A\x64", 2),					//100
				new InPlacePatchVariant(SPTEST, 0x7E164, (BYTE*)"\xBF\x00\x40\x40\x00", 5),		//100*0x2020
			},
			std::vector<PatchVariant*>{
				// allow higher penality limit for low res Vehicle
				new InPlacePatchVariant(GOG, 0x279ccc + 1, (BYTE*)"\x00\xF0\x55\x00", 4),	//0x100*22000
				new InPlacePatchVariant(GOG, 0x279cd1 + 1, (BYTE*)"\x00\xF0\x55\x00", 4),	//0x100*22000
				// allow higher penality limit for high res Vehicle
				new InPlacePatchVariant(GOG, 0x279cd6 + 1, (BYTE*)"\x00\xA0\x86\x01", 4),	//0x100*100000
				// allow higher penality limit for low res soldier
				new InPlacePatchVariant(GOG, 0x279C99, (BYTE*)"\x68\x00\xE8\x03\x00", 5),								//0x100*1000
				// allow higher penality limit for high res soldier
				new InPlacePatchVariant(GOG, 0x279CA0, (BYTE*)"\x68\x00\x10\x27\x00", 5),								//0x100*10000
				// TODO: allow more renderable items
				//// requires Trampoline, hardwareBreakpoint or some other form of patching out of place due to the low size of the instruction
				new InPlacePatchVariant(GOG, 0x279C57, (BYTE*)"\x68\xC8\x00\x00\x00\xEB\x49", 7),						//push 200; jmp sptest.592D26
				new InPlacePatchVariant(GOG, 0x279CA5, (BYTE*)"\xEB\xB0", 2),											//jmp sptest.592CF7
				// allow more than 10 high res human animation
				new InPlacePatchVariant(GOG, 0x206006, (BYTE*)"\x68\x10\x40\x40\x00", 5),								//100*0x2020+16
				new InPlacePatchVariant(GOG, 0x206017, (BYTE*)"\xC7\x00\x00\x02\x00\x00\xBE\x00\x02\x00\x00", 11),		//100
				new InPlacePatchVariant(GOG, 0x206062, (BYTE*)"\x81\xFF\x00\x40\x40\x00", 6),							//100*0x2020
				// since the add to linked list is unrolled, we need to construct a loop.
				// ;preloge
				// push ebx
				// push edi
				// push esi
				// push 0
				// mov ebx, esi
				// mov esi, 0x2020
				// mov edi, 0x404000
				// loop:
				// mov     eax, [ebx]
				// mov     ecx, [esp]
				// add     eax, ecx
				// push    eax; obj
				// add     eax, 0xA0
				// push    eax; a3
				// lea     ecx, [ebx + 4]; this
				// push    ecx; a2
				// call    linkedListInsert
				// add[esp], esi
				// cmp[esp], edi
				// jl      loop
				// ; postloge
				// pop eax; remove the const count
				// pop esi
				// pop edi
				// pop ebx
				// ; skip the function we insert in the middle
				// jmp		fncont
				// 
				// 
				// ; define the function that we will call in the same place as inline
				// ; turns out that linkedlist and linkedlistitem fields order is different causing crashes
				// linkedListInsert:(old)
				// inc dword ptr ds : [ecx + 0x10]
				// mov eax, dword ptr ss : [esp + 8]
				// mov edx, dword ptr ss : [esp + 0xC]
				// mov dword ptr ds : [eax] , ecx
				// mov ecx, dword ptr ss : [esp + 4]
				// mov dword ptr ds : [eax + 4] , ecx
				// mov dword ptr ds : [eax + 0xC] , edx
				// mov edx, dword ptr ds : [ecx + 8]
				// mov dword ptr ds : [eax + 8] , edx
				// mov dword ptr ds : [ecx + 8] , eax
				// mov ecx, dword ptr ds : [eax + 8]
				// mov dword ptr ds : [ecx + 4] , eax
				// ret 0xC
				// linkedListInsert:(new)
				// inc dword ptr ds : [ecx + 0x10]
				// mov eax, dword ptr ss : [esp + 8]
				// mov edx, dword ptr ss : [esp + 0xC]
				// mov dword ptr ds : [eax + 8] , ecx
				// mov ecx, dword ptr ss : [esp + 4]
				// mov dword ptr ds : [eax] , ecx
				// mov dword ptr ds : [eax + 0xC] , edx
				// mov edx, dword ptr ds : [ecx + 4]
				// mov dword ptr ds : [eax + 4] , edx
				// mov dword ptr ds : [ecx + 4] , eax
				// mov ecx, dword ptr ds : [eax + 4]
				// mov dword ptr ds : [ecx] , eax
				// ret 0xC
				//new InPlacePatchVariant(GOG, 0x20606A, (BYTE*)"\x53\x57\x56\x6A\x00\x8B\xDE\xBE\x20\x20\x00\x00\xBF\x00\x40\x40\x00\x8B\x03\x8B\x0C\x24\x03\xC1\x50\x05\xA0\x00\x00\x00\x50\x8D\x4B\x04\x51\xE8\x1E\x00\x00\x00\x01\x34\x24\x39\x3C\x24\x7C\xE1\x58\x5E\x5F\x5B\xE9\x5D\x01\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xFF\x41\x10\x8B\x44\x24\x08\x8B\x54\x24\x0C\x89\x08\x8B\x4C\x24\x04\x89\x48\x04\x89\x50\x0C\x8B\x51\x08\x89\x50\x08\x89\x41\x08\x8B\x48\x08\x89\x41\x04\xC2\x0C\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC", 406),
				new InPlacePatchVariant(GOG, 0x20606A, (BYTE*)"\x53\x57\x56\x6A\x00\x8B\xDE\xBE\x20\x20\x00\x00\xBF\x00\x40\x40\x00\x8B\x03\x8B\x0C\x24\x03\xC1\x50\x05\xA0\x00\x00\x00\x50\x8D\x4B\x04\x51\xE8\x1E\x00\x00\x00\x01\x34\x24\x39\x3C\x24\x7C\xE1\x58\x5E\x5F\x5B\xE9\x5D\x01\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xFF\x41\x10\x8B\x44\x24\x08\x8B\x54\x24\x0C\x89\x48\x08\x8B\x4C\x24\x04\x89\x08\x89\x50\x0C\x8B\x51\x04\x89\x50\x04\x89\x41\x04\x8B\x48\x04\x89\x01\xC2\x0C\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC", 406),
			},
			std::vector<PatchVariant*>{
				// allow higher penality limit for low res Vehicle
				new InPlacePatchVariant(STEAM, 0x294886 + 1, (BYTE*)"\x00\xF0\x55\x00", 4),	//0x100*22000
				new InPlacePatchVariant(STEAM, 0x29488b + 1, (BYTE*)"\x00\xF0\x55\x00", 4),	//0x100*22000
				// allow higher penality limit for high res Vehicle
				new InPlacePatchVariant(STEAM, 0x294890 + 1, (BYTE*)"\x00\xA0\x86\x01", 4),	//0x100*100000
				// allow higher penality limit for low res soldier
				new InPlacePatchVariant(STEAM, 0x294853, (BYTE*)"\x68\x00\xE8\x03\x00", 5),	//0x100*1000
				// allow higher penality limit for high res soldier
				new InPlacePatchVariant(STEAM, 0x29485A, (BYTE*)"\x68\x00\x10\x27\x00", 5),	//0x100*10000
					//new InPlacePatchVariant(STEAM, 0x29485A, (BYTE*)"\x68\x40\x9c\x00\x00", 5),	//0x4*10000 test only
				// allow more renderable items
				// requires Trampoline, hardwareBreakpoint or some other form of patching out of place due to the low size of the instruction
				new InPlacePatchVariant(STEAM, 0x294817, (BYTE*)"\x68\x00\x02\x00\x00\xEB\x43", 7),	//push 200; jmp sptest.592D26
				new InPlacePatchVariant(STEAM, 0x29485F, (BYTE*)"\xEB\xB6", 2),						//jmp sptest.592CF7
				// allow more than 10 high res human animation
				new InPlacePatchVariant(STEAM, 0x21a5d0, (BYTE*)"\x68\x10\x40\x40\x00", 5),		//100*0x2020+16
				// requires Trampoline, hardwareBreakpoint or some other form of patching out of place due to the low size of the instruction
				new InPlacePatchVariant(STEAM, 0x21a5e6, (BYTE*)"\xE9\xEE\x01\x00\x00", 5),		//100
				new InPlacePatchVariant(STEAM, 0x21A7D9, (BYTE*)"\x68\x00\x02\x00\x00\x8D\x70\x10\xE9\x05\xFE\xFF\xFF", 13),
				new InPlacePatchVariant(STEAM, 0x21a5eb, (BYTE*)"\xC7\x00\x00\x02\x00\x00", 6),	//100
				new InPlacePatchVariant(STEAM, 0x21a616, (BYTE*)"\x81\xFE\x00\x40\x40\x00", 6),	//100*0x2020
				new InPlacePatchVariant(STEAM, 0x21a63c, (BYTE*)"\x81\xFE\x00\x40\x40\x00", 6),	//100*0x2020
				//new InPlacePatchVariant(STEAM, 0x21a5d0, (BYTE*)"\x68\x10\x40\x40\x00", 5),		//100*0x2020+16
				//new InPlacePatchVariant(STEAM, 0x21a5e6, (BYTE*)"\x6A\x64", 2),					//100
				//new InPlacePatchVariant(STEAM, 0x21a5eb, (BYTE*)"\xC7\x00\x64\x00\x00\x00", 6),	//100
				//new InPlacePatchVariant(STEAM, 0x21a616, (BYTE*)"\x81\xFE\x00\x40\x40\x00", 6),	//100*0x2020
				//new InPlacePatchVariant(STEAM, 0x21a63c, (BYTE*)"\x81\xFE\x00\x40\x40\x00", 6),	//100*0x2020
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"Heap",
		new Patch(
			std::vector<PatchVariant*>{
				// Increase HEAP size to 512MB
				new InPlacePatchVariant(SPTEST, 0x100FB2, (BYTE*)"\x68\x00\x00\x00\x20", 5),
				new InPlacePatchVariant(SPTEST, 0x100FBC, (BYTE*)"\x8D\x88\x00\x00\x00\x20", 6),
			},
			std::vector<PatchVariant*>{
				// Increase HEAP size to 512MB
				new InPlacePatchVariant(GOG, 0x1D6F10, (BYTE*)"\x68\x00\x00\x00\x20", 5),
				new InPlacePatchVariant(GOG, 0x1D6F1F, (BYTE*)"\x8D\x88\x00\x00\x00\x20", 6),
			},
			std::vector<PatchVariant*>{
				// Increase HEAP size to 512MB
				new InPlacePatchVariant(STEAM, 0x1ed780, (BYTE*)"\x68\x00\x00\x00\x20", 5),
				new InPlacePatchVariant(STEAM, 0x1ed78f, (BYTE*)"\x8D\x88\x00\x00\x00\x20", 6),
				// error when full
				new InPlacePatchVariant(STEAM, 0x28448E, (BYTE*)"\xEB\x70", 2),
				new CallbackPatch(STEAM, []() -> bool {
					LPPROCESS_INFORMATION pinfo = InstanceSingleton::getInstance();
					if (pinfo == NULL || pinfo->hProcess == NULL) return false;
					PVOID base_address = InstanceSingleton::getBase();

					BYTE patch[] = "\x6A\x10\x68\x20\x45\x29\x00\x68\x26\x45\x29\x00\x6A\x00\xFF\x15\x60\xC4\x1C\x02\x33\xC0\xE9\x75\xFF\xFF\xFF\x90\x90\x90\x90\x90"/*without the separation, the 'E' is considered as part of the hex*/"Error\x00MemoryFull\x00";
					*(byte**)((byte*)&patch + 3) = (byte*)base_address + 0x284520;
					*(byte**)((byte*)&patch + 8) = (byte*)base_address + 0x284526;

					return WriteProcessMemory(pinfo->hProcess, (BYTE*)base_address + 0x284500, &patch, 49, nullptr);
				}),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"MatrixPool",
		new Patch(
			std::vector<PatchVariant*>{
				// change the pointer
				// size = 0x40 sizeof(D3DMATRIX) * 0x4000 count = 0x100000
				// 0x18251F {81 C3 00 E0 AA 01}, 0x182593 {81 C6 00 E0 AA 01}
				new AllocatorPatch(SPTEST, 0x40 * 0x8000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					byte p1[6] = { 0x81 , 0xC3 },
						p2[6] = { 0x81 , 0xC6 },
						p3[5] = { 0x3D },
						p4[5] = { 0x3D };
					BYTE* endPtr = (BYTE*)ptr + 0x40 * 0x8000;
					memcpy(&p1[2], &ptr, 4);
					memcpy(&p2[2], &ptr, 4);
					memcpy(&p3[1], &ptr, 4);
					memcpy(&p4[1], &endPtr, 4);
					return (new InPlacePatchVariant(SPTEST, 0x18251F, (const BYTE*)&p1, 6))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x182593, (const BYTE*)&p2, 6))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x186b7d, (const BYTE*)&p3, 5))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x186b84, (const BYTE*)&p4, 5))->patch();
				}),
				// Increase Matrix count from 0xBF8 to 0x4000
				new InPlacePatchVariant(SPTEST, 0x182599, (BYTE*)"\x81\xF9\x00\x80\x00\x00", 6),
				new InPlacePatchVariant(SPTEST, 0x182525, (BYTE*)"\x81\xF9\x00\x80\x00\x00", 6),
				// RenderItemPool
				// change the pointer
				// size = 0x30 sizeof(RenderItem) * 0x6000 count = 0x120000
				new AllocatorPatch(SPTEST, 0x30 * 0x6000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
				byte p1[6] = { 0x81 , 0xC6 };
				memcpy(&p1[2], &ptr, 4);
				return (new InPlacePatchVariant(SPTEST, 0x186b0a, (const BYTE*)&p1, 6))->patch();
					}),
				// Increase RenderItemPoolCount count from 0x1228 to 0x6000
				new InPlacePatchVariant(SPTEST, 0x186ac8, (BYTE*)"\x3D\x00\x60\x00\x00", 5),
				// Increase sOPAQUE from 1800 to 7200 and respective size to 7200*8 + 8
				new InPlacePatchVariant(SPTEST, 0x1FCAA0, (BYTE*)"\x68\x08\xE1\x00\x00", 5),
				new InPlacePatchVariant(SPTEST, 0x1FCABC, (BYTE*)"\x20\x1C\x00\x00", 4),
				// Increase sShadow from 1024 to 4096 and respective size to 4096*8 + 8
				new InPlacePatchVariant(SPTEST, 0x1FCAE0, (BYTE*)"\x68\x08\x80\x00\x00", 5),
				new InPlacePatchVariant(SPTEST, 0x1FCAFC, (BYTE*)"\x00\x10\x00\x00", 4),
				// Increase sPostShadow from 2312 to 9248 and respective size to 9248*8 + 8
				new InPlacePatchVariant(SPTEST, 0x1FCB20, (BYTE*)"\x68\x08\x21\x01\x00", 5),
				new InPlacePatchVariant(SPTEST, 0x1FCB3C, (BYTE*)"\x20\x24\x00\x00", 4),
				// Increase sTRANSPARENT from 0x320 to 0x32000
				new InPlacePatchVariant(SPTEST, 0x1fcb60, (BYTE*)"\x68\x00\x20\x03\x00", 5),
				// Increase sPOSTTRANSPARENT from 0x200 to 0x20000
				new InPlacePatchVariant(SPTEST, 0x1fcb80, (BYTE*)"\x68\x00\x00\x02\x00", 5),
			},
			std::vector<PatchVariant*>{
				// change the pointer
				// size = 0x40 sizeof(D3DMATRIX) * 0x8000 count = 0x200000
				// 0x18251F {81 C3 00 E0 AA 01}, 0x182593 {81 C6 00 E0 AA 01}
				new AllocatorPatch(GOG, 0x40 * 0x8000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					byte p1[5] = { 0x05 },
						p2[6] = { 0x81 , 0xC6 },
						p3[6] = { 0x81, 0xFA }, 
						p4[6] = { 0x81, 0xFA };
					BYTE* endPtr = (BYTE*)ptr + 0x40 * 0x8000;
					memcpy(&p1[1], &ptr, 4);
					memcpy(&p2[2], &ptr, 4);
					memcpy(&p3[2], &ptr, 4);
					memcpy(&p4[2], &endPtr, 4);
					return (new InPlacePatchVariant(GOG, 0x25DD32, (const BYTE*)&p1, 5))->patch()
						&& (new InPlacePatchVariant(GOG, 0x25DDAF, (const BYTE*)&p2, 6))->patch()
						&& (new InPlacePatchVariant(GOG, 0x264FE2, (const BYTE*)&p3, 6))->patch()
						&& (new InPlacePatchVariant(GOG, 0x264FEA, (const BYTE*)&p4, 6))->patch();
				}),
				// Increase Matrix count from 0xBF8 to 0x4000
				new InPlacePatchVariant(GOG, 0x25DD3A, (BYTE*)"\x81\xF9\x00\x80\x00\x00", 6),
				new InPlacePatchVariant(GOG, 0x25DDB8, (BYTE*)"\x3D\x00\x80\x00\x00", 5),
				// RenderItemPool
				// change the pointer
				// size = 0x30 sizeof(RenderItem) * 0x6000 count = 0x120000
				new AllocatorPatch(GOG, 0x30 * 0x6000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
				byte p1[5] = { 0x68 },
					p2[6] = { 0x81 , 0xC6 },
					p3[5] = { 0x68 };
				memcpy(&p1[1], &ptr, 4);
				memcpy(&p2[2], &ptr, 4);
				memcpy(&p3[1], &ptr, 4);
				return (new InPlacePatchVariant(GOG, 0x3c097c, (const BYTE*)&p1, 5))->patch()
					&& (new InPlacePatchVariant(GOG, 0x264fab, (const BYTE*)&p2, 6))->patch()
					&& (new InPlacePatchVariant(GOG, 0x02cd51, (const BYTE*)&p3, 5))->patch();
					}),
				// Increase RenderItemPoolCount count from 0x1228 to 0x6000
				new InPlacePatchVariant(GOG, 0x264f92, (BYTE*)"\x3D\x00\x60\x00\x00", 5),
				new InPlacePatchVariant(GOG, 0x02cd4a, (BYTE*)"\x68\x00\x60\x00\x00", 5),
				new InPlacePatchVariant(GOG, 0x3c0975, (BYTE*)"\x68\x00\x60\x00\x00", 5),
				// Increase sOPAQUE from 1800 to 7200 and respective size to 7200*8 + 8
				new InPlacePatchVariant(GOG, 0x2CC70, (BYTE*)"\x68\x08\xE1\x00\x00", 5),
				new InPlacePatchVariant(GOG, 0x2CC8C, (BYTE*)"\x20\x1C\x00\x00", 4),
				// Increase sShadow from 1024 to 4096 and respective size to 4096*8 + 8
				new InPlacePatchVariant(GOG, 0x2CD70, (BYTE*)"\x68\x08\x80\x00\x00", 5),
				new InPlacePatchVariant(GOG, 0x2CD8C, (BYTE*)"\x00\x10\x00\x00", 4),
				// Increase sPostShadow from 2312 to 9248 and respective size to 9248*8 + 8
				new InPlacePatchVariant(GOG, 0x2CCC0, (BYTE*)"\x68\x08\x21\x01\x00", 5),
				new InPlacePatchVariant(GOG, 0x2CCDC, (BYTE*)"\x20\x24\x00\x00", 4),
				// Increase sTRANSPARENT from 0x320 to 0x32000
				new InPlacePatchVariant(GOG, 0x2cdc0, (BYTE*)"\x68\x00\x20\x03\x00", 5),
				// Increase sPOSTTRANSPARENT from 0x200 to 0x20000
				new InPlacePatchVariant(GOG, 0x2cd10, (BYTE*)"\x68\x00\x00\x02\x00", 5),
			},
			std::vector<PatchVariant*>{
				// change the pointer
				// size = 0x40 sizeof(D3DMATRIX) * 0x8000 count = 0x200000
				// 0x18251F {81 C3 00 E0 AA 01}, 0x182593 {81 C6 00 E0 AA 01}
				new AllocatorPatch(STEAM, 0x40 * 0x8000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					byte p1[5] = { 0x05 },
						p2[6] = { 0x81 , 0xC6 },
						p3[5] = { 0x3D },
						p4[5] = { 0x3D };
					BYTE* endPtr = (BYTE*)ptr + 0x40 * 0x8000;
					memcpy(&p1[1], &ptr, 4);
					memcpy(&p2[2], &ptr, 4);
					memcpy(&p3[1], &ptr, 4);
					memcpy(&p4[1], &endPtr, 4);
					return (new InPlacePatchVariant(STEAM, 0x2753A2, (const BYTE*)&p1, 5))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x27541f, (const BYTE*)&p2, 6))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x278e76, (const BYTE*)&p3, 5))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x278E7D, (const BYTE*)&p4, 5))->patch();
				}),
				// Increase Matrix count from 0xBF8 to 0x8000
				new InPlacePatchVariant(STEAM, 0x2753AA, (BYTE*)"\x81\xF9\x00\x80\x00\x00", 6),
				new InPlacePatchVariant(STEAM, 0x275428, (BYTE*)"\x3D\x00\x80\x00\x00", 5),
				new InPlacePatchVariant(STEAM, 0x47b15, (BYTE*)"\x68\x00\x80\x00\x00", 5),
				// Create a breakpoint if matrix is full
				new InPlacePatchVariant(STEAM, 0x2753B2, (BYTE*)"\xEB\x0C", 2),
				new InPlacePatchVariant(STEAM, 0x2753C0, (BYTE*)"\x83\xF9\x00\x75\x01\xCC\x23\xCE\xEB\xEA", 10),
				// RenderItemPool
				// change the pointer
				// size = 0x30 sizeof(RenderItem) * 0x6000 count = 0x120000
				new AllocatorPatch(STEAM, 0x30 * 0x6000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
				byte p1[5] = { 0x68 },
					p2[6] = { 0x81 , 0xC7 },
					p3[5] = { 0x68 };
				memcpy(&p1[1], &ptr, 4);
				memcpy(&p2[2], &ptr, 4);
				memcpy(&p3[1], &ptr, 4);
				return (new InPlacePatchVariant(STEAM, 0x40a0dc, (const BYTE*)&p1, 5))->patch()
					&& (new InPlacePatchVariant(STEAM, 0x27c9f9, (const BYTE*)&p2, 6))->patch()
					&& (new InPlacePatchVariant(STEAM, 0x047c71, (const BYTE*)&p3, 5))->patch();
					}),
				// Increase RenderItemPoolCount count from 0x1228 to 0x6000
				new InPlacePatchVariant(STEAM, 0x27c9d6, (BYTE*)"\x81\xFB\x00\x60\x00\x00", 6),
				new InPlacePatchVariant(STEAM, 0x40a0d5, (BYTE*)"\x68\x00\x60\x00\x00", 5),
				new InPlacePatchVariant(STEAM, 0x047c6a, (BYTE*)"\x68\x00\x60\x00\x00", 5),
				// Increase sOPAQUE from 1800 to 7200, here size is calculated automatically
				new InPlacePatchVariant(STEAM, 0x47BD4, (BYTE*)"\x68\x20\x1C\x00\x00", 5),
				// Increase sShadow from 1024 to 4096, here size is calculated automatically
				new InPlacePatchVariant(STEAM, 0x47C94, (BYTE*)"\x68\x00\x10\x00\x00", 5),
				// Increase sPostShadow from 2312 to 9248, here size is calculated automatically
				new InPlacePatchVariant(STEAM, 0x47C04, (BYTE*)"\x68\x20\x24\x00\x00", 5),
				// Increase sTRANSPARENT from 0x320 to 0x32000
				new InPlacePatchVariant(STEAM, 0x47cc0, (BYTE*)"\x68\x00\x20\x03\x00", 5),
				// Increase sPOSTTRANSPARENT from 0x200 to 0x20000
				new InPlacePatchVariant(STEAM, 0x47c30, (BYTE*)"\x68\x00\x00\x02\x00", 5),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"Console",
		new Patch(
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(SPTEST, 0x17B44F, (BYTE*)"\x00", 1), // make transparent
				new NOPInPlacePatchVariant(SPTEST, 0x178FB4, 6), // don't activate on write
				new NOPInPlacePatchVariant(SPTEST, 0x17B5D5, 9), // don't activate on error
				new AllocatorPatch(SPTEST, 272 /*size of the struct*/ * 100 /*number of maps*/, PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					LPVOID missionHash = (BYTE*)ptr + 4;
					LPVOID path = (BYTE*)ptr + 8;
					LPVOID lastPath = (BYTE*)ptr + 0x10B;
					LPVOID memoryModifier = (BYTE*)ptr + 0x10C;
					BYTE size = 100;
					return (new InPlacePatchVariant(SPTEST, 0xd7ae + 1, (const BYTE*)&ptr,			4))->patch()		// DownloadableContent::SetDCMap::MapLuaFile_hash
						&& (new InPlacePatchVariant(SPTEST, 0xd7e3 + 1, (const BYTE*)&missionHash,	4))->patch()		// DownloadableContent::SetDCMap::MissionScriptName_hash
						&& (new InPlacePatchVariant(SPTEST, 0xd821 + 1, (const BYTE*)&path,			4))->patch()		// DownloadableContent::getPath
						&& (new InPlacePatchVariant(SPTEST, 0xd839 + 2, (const BYTE*)&memoryModifier, 4))->patch()		// DownloadableContent::getMemoryModifier
						&& (new InPlacePatchVariant(SPTEST, 0xd880 + 6, (const BYTE*)&size,			1))->patch()		// AddDownloadableContent_closure
						&& (new InPlacePatchVariant(SPTEST, 0xd8a6 + 2, (const BYTE*)&ptr,			4))->patch()		// AddDownloadableContent_closure
						&& (new InPlacePatchVariant(SPTEST, 0xd8c8 + 2, (const BYTE*)&missionHash,	4))->patch()		// AddDownloadableContent_closure
						&& (new InPlacePatchVariant(SPTEST, 0xd8ce + 2, (const BYTE*)&path,			4))->patch()		// AddDownloadableContent_closure
						&& (new InPlacePatchVariant(SPTEST, 0xd8f3 + 2, (const BYTE*)&lastPath,		4))->patch()		// AddDownloadableContent_closure
						&& (new InPlacePatchVariant(SPTEST, 0xd8fe + 2, (const BYTE*)&memoryModifier, 4))->patch();		// AddDownloadableContent_closure
				}),
			},
			std::vector<PatchVariant*>{
				
			},
			std::vector<PatchVariant*>{
				//NOT used, it seems that it is not green in production and I doubt anything would write to console mid play
				new InPlacePatchVariant(STEAM, 0x485b7 + 1, (BYTE*)"\x00", 1), // make transparent
				new NOPInPlacePatchVariant(STEAM, 0x2c2665, 8), // don't activate on write
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"Crouch",
		new Patch(
			std::vector<PatchVariant*>{},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0xF1427, (BYTE*)"\xC6\x46\x41\x00\x5F\x5E\xC3", 7), // clear crouch after crouch
				new InPlacePatchVariant(GOG, 0xFC73A, (BYTE*)"\x85\xC0\x74\x07\xC6\x86\x91\x02\x00\x00\x00\x5E\x8B\xE5\x5D\xC3", 16), // clear crouch after standing
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x10E3b1, (BYTE*)"\xC6\x46\x41\x00\x5F\x5E\xC3", 7), // clear crouch after crouch
				new InPlacePatchVariant(GOG, 0x118E42, (BYTE*)"\x85\xC0\x74\x07\xC6\x86\x91\x02\x00\x00\x00\x5E\x8B\xE5\x5D\xC3", 16), // clear crouch after standing
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"FixedPools",
		new Patch(
			std::vector<PatchVariant*>{ //call 0x47FB49
				new InPlacePatchVariant(SPTEST, 0x112F01, (BYTE*)"\xE8\x43\xCC\xF6\xFF", 5), // Decal
				new InPlacePatchVariant(SPTEST, 0x09DD89, (BYTE*)"\xE8\xBB\x1D\xFE\xFF", 5), // PathRequest
				new InPlacePatchVariant(SPTEST, 0x09DD9A, (BYTE*)"\xE8\xAA\x1D\xFE\xFF", 5), // AIPath
				new InPlacePatchVariant(SPTEST, 0x09DDAB, (BYTE*)"\xE8\x99\x1D\xFE\xFF", 5), // AIPathNode
			},
			/*
			* mov     eax, [esp+4]
			* mov     [ecx+34h], eax
			* mov     eax, [esp+8]
			* mov     [ecx+30h], eax
			*/
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x1D3F28, (BYTE*)"\x8B\x44\x24\x04\x89\x41\x34\x8B\x44\x24\x08\x89\x41\x30\xC2\x08\x00", 17), // create the init function as it doesn't exist
				new InPlacePatchVariant(GOG, 0x0a8016, (BYTE*)"\xE8\x0D\xBF\x12\x00", 5), // Decal
				new InPlacePatchVariant(GOG, 0x1d3f49, (BYTE*)"\xE8\xDA\xFF\xFF\xFF", 5), // PathRequest
				new InPlacePatchVariant(GOG, 0x1d3f5a, (BYTE*)"\xE8\xC9\xFF\xFF\xFF", 5), // AIPath
				new InPlacePatchVariant(GOG, 0x1d3f6b, (BYTE*)"\xE8\xB8\xFF\xFF\xFF", 5), // AIPathNode
			},
			std::vector<PatchVariant*>{ //call :$12292 (0x00C72292)
				new InPlacePatchVariant(STEAM, 0x0c8f5e, (BYTE*)"\xE8\x2F\x93\xF4\xFF", 5), // Decal
				new InPlacePatchVariant(STEAM, 0x1ea8ee, (BYTE*)"\xE8\x9F\x79\xE2\xFF", 5), // PathRequest
				new InPlacePatchVariant(STEAM, 0x1ea8ae, (BYTE*)"\xE8\xDF\x79\xE2\xFF", 5), // AIPath
				new InPlacePatchVariant(STEAM, 0x1ea8ce, (BYTE*)"\xE8\xBF\x79\xE2\xFF", 5), // AIPathNode
			}
		)
	));
	// 2**8 = 256 which would cover 200 units (current max) and up to 56 other entites before overflowing
	// 2**10 = 1024 per latest unlock 11/14
	BYTE trackingPower = 10;
	patches.insert(std::pair<const char*, Patch*>(
		"RayRequest",
		new Patch(
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(SPTEST, 0x887b2 + 1, (BYTE*)"\xA0\x0F\x00\x00", 4), // close units array
				new InPlacePatchVariant(SPTEST, 0x8895f + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // close units array size
				new InPlacePatchVariant(SPTEST, 0x887cd, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue count and first element
				//-----------------------------------
				new AllocatorPatch(SPTEST, 24 /*size of the struct*/ * 8 /*number of teams*/ << trackingPower /*Count of objects per team as power of 2*/, PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					LPVOID offsetPtr = (BYTE*)ptr + 8;
					LPVOID offsetPtr2 = (BYTE*)ptr + 0x14;
					BYTE lclTrackingPower = trackingPower;
					BYTE lclTeamShift = trackingPower + 3; // eight teams 2**3
					DWORD lclTeamSize = 24 << trackingPower; // eight teams 2**3
					return (new InPlacePatchVariant(SPTEST, 0x24f4e + 3, (const BYTE*)&offsetPtr2, 4))->patch()			// getCloseUnits
						&& (new InPlacePatchVariant(SPTEST, 0x25038 + 3, (const BYTE*)&lclTeamSize, 4))->patch()		// getCloseUnits
						&& (new InPlacePatchVariant(SPTEST, 0x24ed3 + 2, (const BYTE*)&offsetPtr, 4))->patch()			// fill collision function
						&& (new InPlacePatchVariant(SPTEST, 0x24ecf + 2, (const BYTE*)&lclTeamShift, 1))->patch()		// fill collision function
						&& (new InPlacePatchVariant(SPTEST, 0x24db2 + 2, (const BYTE*)&lclTrackingPower, 1))->patch()	// GameObject::track
						&& (new InPlacePatchVariant(SPTEST, 0x24dbf + 2, (const BYTE*)&ptr, 4))->patch()				// GameObject::track
						&& (new InPlacePatchVariant(SPTEST, 0x24ddb + 2, (const BYTE*)&offsetPtr, 4))->patch()			// GameObject::track
						&& (new InPlacePatchVariant(SPTEST, 0x24e3e + 2, (const BYTE*)&lclTeamShift, 1))->patch()		// GameObject::__untrack
						&& (new InPlacePatchVariant(SPTEST, 0x24e41 + 2, (const BYTE*)&ptr, 4))->patch()				// GameObject::__untrack
						&& (new InPlacePatchVariant(SPTEST, 0x24e73 + 2, (const BYTE*)&lclTrackingPower, 1))->patch()	// GameObject::__untrack
						&& (new InPlacePatchVariant(SPTEST, 0x24e82 + 2, (const BYTE*)&ptr, 4))->patch()				// GameObject::__untrack
						&& (new InPlacePatchVariant(SPTEST, 0x24e90 + 2, (const BYTE*)&ptr, 4))->patch()				// GameObject::__untrack
						&& (new InPlacePatchVariant(SPTEST, 0x24e9e + 2, (const BYTE*)&offsetPtr, 4))->patch()			// GameObject::__untrack
						&& (new InPlacePatchVariant(SPTEST, 0x24ea7 + 2, (const BYTE*)&offsetPtr, 4))->patch();			// GameObject::__untrack
				}),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x22b161 + 1, (BYTE*)"\xA0\x0F\x00\x00", 4), // close units array
				new InPlacePatchVariant(GOG, 0x22b83d + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // close units array size
				new InPlacePatchVariant(GOG, 0x22b180 + 1, (BYTE*)"\x48\x1F\x00\x00", 4), // rayrequest priority queue 1 size
				new InPlacePatchVariant(GOG, 0x22b194 + 3, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue 1 count
				new InPlacePatchVariant(GOG, 0x22b19b + 2, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue 1 first element
				new InPlacePatchVariant(GOG, 0x22b1c1 + 1, (BYTE*)"\x48\x1F\x00\x00", 4), // rayrequest pool size
				new InPlacePatchVariant(GOG, 0x22b1d5 + 3, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest count
				new InPlacePatchVariant(GOG, 0x22b1dc + 2, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest count
				//-----------------------------------
				new AllocatorPatch(GOG, (24 /*size of the struct*/ * 8 /*number of teams*/ << trackingPower /*Count of objects per team as power of 2*/) + 0x18, PAGE_READWRITE, [=](LPVOID o_ptr) -> bool {
					LPVOID ptr_0 = (BYTE*)o_ptr;
					LPVOID ptr_4 = (BYTE*)o_ptr + 4;
					LPVOID ptr_8 = (BYTE*)o_ptr + 8;
					LPVOID ptr = (BYTE*)o_ptr + 0x18;
					LPVOID offsetPtr = (BYTE*)ptr + 4;
					LPVOID offsetPtr2 = (BYTE*)ptr + 8;
					BYTE lclTrackingPower = trackingPower;
					DWORD lclTeamSize = 24 << trackingPower; // eight teams 2**3
					return (new InPlacePatchVariant(GOG, 0x2128bb + 3, (const BYTE*)&offsetPtr, 4))->patch()			// getCloseUnits
						&& (new InPlacePatchVariant(GOG, 0x2129d9 + 3, (const BYTE*)&lclTeamSize, 4))->patch()			// getCloseUnits
						&& (new InPlacePatchVariant(GOG, 0x2127da + 2, (const BYTE*)&ptr, 4))->patch()					// fill collision function
						&& (new InPlacePatchVariant(GOG, 0x2127d4 + 2, (const BYTE*)&lclTeamSize, 4))->patch()			// fill collision function
						&& (new InPlacePatchVariant(GOG, 0x2121b3 + 2, (const BYTE*)&lclTrackingPower, 1))->patch()		// GameObject::track
						&& (new InPlacePatchVariant(GOG, 0x2121bb + 3, (const BYTE*)&ptr, 4))->patch()					// GameObject::track
						&& (new InPlacePatchVariant(GOG, 0x2121c8 + 3, (const BYTE*)&offsetPtr, 4))->patch()			// GameObject::track
						&& (new InPlacePatchVariant(GOG, 0x2121ec + 4, (const BYTE*)&offsetPtr2, 4))->patch()			// GameObject::track
						&& (new InPlacePatchVariant(GOG, 0x212202 + 4, (const BYTE*)&offsetPtr2, 4))->patch()			// GameObject::track
						&& (new InPlacePatchVariant(GOG, 0x21269b + 2, (const BYTE*)&lclTeamSize, 4))->patch()			// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x2126a1 + 1, (const BYTE*)&ptr, 4))->patch()					// GameObject::__untrack
						//this one crashes, the issue is that those lines access elements before array start, ignoring for now
						// TODO: fix this by accounting for the 2 dwords and the xmmword before the ptr
						&& (new InPlacePatchVariant(GOG, 0x2126e2 + 2, (const BYTE*)&lclTrackingPower, 1))->patch()		// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x2126f1 + 3, (const BYTE*)&ptr_0, 4))->patch()		// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x2126ff + 3, (const BYTE*)&ptr_4, 4))->patch()		// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x212710 + 4, (const BYTE*)&ptr_8, 4))->patch()		// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x2126f8 + 3, (const BYTE*)&ptr, 4))->patch()					// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x212706 + 3, (const BYTE*)&offsetPtr, 4))->patch()			// GameObject::__untrack
						&& (new InPlacePatchVariant(GOG, 0x21271a + 4, (const BYTE*)&offsetPtr2, 4))->patch();			// GameObject::__untrack
				}),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(STEAM, 0x23de70 + 1, (BYTE*)"\xA0\x0F\x00\x00", 4), // close units array
				new InPlacePatchVariant(STEAM, 0x23e4d0 + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // close units array size
				new InPlacePatchVariant(STEAM, 0x23de8f + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue 1
				new InPlacePatchVariant(STEAM, 0x23de94 + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue 1
				new InPlacePatchVariant(STEAM, 0x23deb9 + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue 2
				new InPlacePatchVariant(STEAM, 0x23debe + 1, (BYTE*)"\xe8\x03\x00\x00", 4), // rayrequest priority queue 2
				//-----------------------------------
				new AllocatorPatch(STEAM, 24 /*size of the struct*/ * 8 /*number of teams*/ << trackingPower /*Count of objects per team as power of 2*/, PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					LPVOID offsetPtr = (BYTE*)ptr + 8;
					BYTE lclTrackingPower = trackingPower;
					DWORD lclTeamSize = 24 << trackingPower; // eight teams 2**3
					return (new InPlacePatchVariant(STEAM, 0x226673 + 3, (const BYTE*)&offsetPtr, 4))->patch()			// getCloseUnits
						&& (new InPlacePatchVariant(STEAM, 0x226769 + 3, (const BYTE*)&lclTeamSize, 4))->patch()		// getCloseUnits
						&& (new InPlacePatchVariant(STEAM, 0x2265ea + 2, (const BYTE*)&offsetPtr, 4))->patch()			// fill collision function
						&& (new InPlacePatchVariant(STEAM, 0x2265e4 + 2, (const BYTE*)&lclTeamSize, 4))->patch()		// fill collision function
						&& (new InPlacePatchVariant(STEAM, 0x22619a + 2, (const BYTE*)&lclTrackingPower, 1))->patch()	// GameObject::track
						&& (new InPlacePatchVariant(STEAM, 0x2261a2 + 3, (const BYTE*)&ptr, 4))->patch()				// GameObject::track
						&& (new InPlacePatchVariant(STEAM, 0x2261c3 + 4, (const BYTE*)&offsetPtr, 4))->patch()			// GameObject::track
						&& (new InPlacePatchVariant(STEAM, 0x2264e9 + 2, (const BYTE*)&lclTrackingPower, 1))->patch()	// GameObject::__untrack
						&& (new InPlacePatchVariant(STEAM, 0x2264f2 + 3, (const BYTE*)&ptr, 4))->patch()				// GameObject::__untrack
						&& (new InPlacePatchVariant(STEAM, 0x226536 + 3, (const BYTE*)&ptr, 4))->patch()				// GameObject::__untrack
						&& (new InPlacePatchVariant(STEAM, 0x226546 + 4, (const BYTE*)&offsetPtr, 4))->patch()			// GameObject::__untrack
						&& (new InPlacePatchVariant(STEAM, 0x226554 + 4, (const BYTE*)&offsetPtr, 4))->patch();			// GameObject::__untrack
				}),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"MoreFollowers",
		new Patch(
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(SPTEST, 0x8AE62, (BYTE*)"\x09", 1), // Set Followers to 8
				new InPlacePatchVariant(SPTEST, 0x90B55, (BYTE*)"\x09", 1), // Set Followers to 8
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x13F2DD, (BYTE*)"\x09", 1), // Set Followers to 8
				new InPlacePatchVariant(GOG, 0x222bb4, (BYTE*)"\x09", 1), // Set Followers to 8
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(STEAM, 0x159C86, (BYTE*)"\x09", 1), // Set Followers to 8
				new InPlacePatchVariant(STEAM, 0x235d32, (BYTE*)"\x09", 1), // Set Followers to 8
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"MoreFlyers",
		new Patch(
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(SPTEST, 0x87DDD, (BYTE*)"\x7F", 1),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x1F11A2, (BYTE*)"\xEC\x0B\x00\x00", 4),
				new InPlacePatchVariant(GOG, 0x1f11a9, (BYTE*)"\x7F", 1),
				new InPlacePatchVariant(GOG, 0x1f11be, (BYTE*)"\x7F", 1),
				new InPlacePatchVariant(GOG, 0x1f11c3, (BYTE*)"\x7F", 1),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(STEAM, 0x2073BF, (BYTE*)"\x7F", 1),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"MPBots",
		new Patch(
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(SPTEST, 0x24fad8, (BYTE*)"\x3f", 1),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x42a768, (BYTE*)"\x3f", 1),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(STEAM, 0x4796f8, (BYTE*)"\x3f", 1),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"Foliage",
		new Patch(
			std::vector<PatchVariant*>{
				new AllocatorPatch(SPTEST, 0xE0000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					BYTE* wordArrPtr = (BYTE*)ptr + 0xDAC00;
					BYTE* firstIndexPtr = (BYTE*)ptr + 0x1E;
					DWORD count = 8000;
					//PropObjectMeshArray::setIndex
					return (new InPlacePatchVariant(SPTEST, 0x1040B3 + 1, (const BYTE*)&firstIndexPtr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1040B8 + 1, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1040C9 + 2, (const BYTE*)&count, 2))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1040CF + 7, (const BYTE*)&count, 2))->patch()
						//PropObjectMesh::release
						&& (new InPlacePatchVariant(SPTEST, 0x1040F0 + 4, (const BYTE*)&wordArrPtr, 4))->patch()
						//PropModelMesh::getPropObjectMesh
						&& (new InPlacePatchVariant(SPTEST, 0x1042E7 + 4, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1042F5 + 2, (const BYTE*)&ptr, 4))->patch()
						//PropObjectMeshArray::constructor
						&& (new InPlacePatchVariant(SPTEST, 0x1FB9E0 + 1, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1FB9E5 + 1, (const BYTE*)&count, 4))->patch()
						//PropObjectMeshArray::destructor
						&& (new InPlacePatchVariant(SPTEST, 0x1FF912 + 1, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1FF917 + 1, (const BYTE*)&count, 4))->patch()
						//force 254 clusters
						&& (new InPlacePatchVariant(SPTEST, 0x1033b6, (const BYTE*)&"\x90\x90", 2))->patch();
						// && (new InPlacePatchVariant(SPTEST, 0x1033aa + 1, (const BYTE*)&"\xFE\x03", 2))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(GOG, 0xE0000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					BYTE* wordArrPtr = (BYTE*)ptr + 0xDAC00;
					BYTE* firstIndexPtr = (BYTE*)ptr + 0x1E;
					DWORD count = 8000;
					//PropObjectMeshArray::setIndex
					return (new InPlacePatchVariant(GOG, 0x1ed803 + 1, (const BYTE*)&firstIndexPtr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x1ed808 + 1, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x1ed80d + 1, (const BYTE*)&count, 2))->patch()
						//PropObjectMesh::release
						&& (new InPlacePatchVariant(GOG, 0x1ee82d + 4, (const BYTE*)&wordArrPtr, 4))->patch()
						//PropModelMesh::getPropObjectMesh
						&& (new InPlacePatchVariant(GOG, 0x1ed877 + 4, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x1ed89a + 2, (const BYTE*)&ptr, 4))->patch()
						//PropObjectMeshArray::constructor
						&& (new InPlacePatchVariant(GOG, 0x2b7a2 + 1, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2b7a7 + 1, (const BYTE*)&count, 4))->patch()
						//PropObjectMeshArray::destructor
						&& (new InPlacePatchVariant(GOG, 0x3c0547 + 1, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x3c0542 + 1, (const BYTE*)&count, 4))->patch()
						//force 254 clusters
						&& (new InPlacePatchVariant(GOG, 0x1ebaf2, (const BYTE*)&"\x90\x90", 2))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(STEAM, 0xE0000, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					BYTE* wordArrPtr = (BYTE*)ptr + 0xDAC00;
					BYTE* firstIndexPtr = (BYTE*)ptr + 0x1E;
					DWORD count = 8000;
					//PropObjectMeshArray::setIndex
					return (new InPlacePatchVariant(STEAM, 0x203873 + 1, (const BYTE*)&firstIndexPtr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x203878 + 1, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x20387d + 1, (const BYTE*)&count, 2))->patch()
						//PropObjectMesh::release
						&& (new InPlacePatchVariant(STEAM, 0x204a7d + 4, (const BYTE*)&wordArrPtr, 4))->patch()
						//PropModelMesh::getPropObjectMesh
						&& (new InPlacePatchVariant(STEAM, 0x2038e7 + 4, (const BYTE*)&wordArrPtr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x20390a + 2, (const BYTE*)&ptr, 4))->patch()
						//PropObjectMeshArray::constructor
						&& (new InPlacePatchVariant(STEAM, 0x418bc + 1, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x418b5 + 1, (const BYTE*)&count, 4))->patch()
						//PropObjectMeshArray::destructor
						&& (new InPlacePatchVariant(STEAM, 0x409dfc + 1, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x409df5 + 1, (const BYTE*)&count, 4))->patch()
						//force 254 clusters
						&& (new InPlacePatchVariant(STEAM, 0x201b52, (const BYTE*)&"\x90\x90", 2))->patch();
				}),
			}
		)
	));
	// channelsCount max is 127 since we push one byte size
	//unsigned int soundCount = 1 << 15, channelsCount = 33;
	unsigned int soundCount = 256, channelsCount = 127;
	unsigned int D3DVECTOR_size = 0xC,
		SndSource3D_size_sptest = 0x28,
		SndSource3D_size_steam = 0x2C,
		s_snd_2_size_sptest = 0xC4,
		s_snd_2_size_steam = 0xC8,
		struct_snd_7_size_sptest = 0x530,
		struct_snd_7_size_steam  = 0x548;
	unsigned int struct_snd_19_size = 0x900;//sptest has 0x8E8, rest have 0x8F8
	unsigned int new_struct_snd_19_size = struct_snd_19_size + channelsCount * ( 4 * 2 /*gapCC*/ * 2 /*gap1CC*/ + 8 /*gap2CC*/ + 8 /*gap3CC*/ + 4 /*gap4CC*/);
	unsigned int gapCC_offset = struct_snd_19_size;
	unsigned int gap2CC_offset = gapCC_offset + 4 * 2 * channelsCount * 2;//gap1CC is accessed with gapCC, this is why it caused a crash
	unsigned int gap3CC_offset = gap2CC_offset + 8 * channelsCount;
	unsigned int gap4CC_offset = gap3CC_offset + 8 * channelsCount;
	patches.insert(std::pair<const char*, Patch*>(
		"Sound",
		new Patch(
			std::vector<PatchVariant*>{
				new AllocatorPatch(SPTEST, channelsCount * struct_snd_7_size_sptest + soundCount * (s_snd_2_size_sptest + SndSource3D_size_sptest + D3DVECTOR_size * 2), PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					BYTE* struct_snd_7Arr_ptr	= (BYTE*)ptr;
					BYTE* s_snd_2Arr_ptr		= struct_snd_7Arr_ptr	+ channelsCount * struct_snd_7_size_sptest;
					BYTE* d3dvector1Arr_ptr		= s_snd_2Arr_ptr		+ soundCount * s_snd_2_size_sptest;
					BYTE* d3dvector2Arr_ptr		= d3dvector1Arr_ptr		+ soundCount * D3DVECTOR_size;
					BYTE* SndSource3DArr_ptr	= d3dvector2Arr_ptr		+ soundCount * D3DVECTOR_size;
					BYTE* end_ptr				= SndSource3DArr_ptr	+ soundCount * SndSource3D_size_sptest;
					unsigned int max_struct_snd_7Arr_offset = channelsCount * struct_snd_7_size_sptest;
					unsigned int max_s_snd_2Arr_offset		= soundCount * s_snd_2_size_sptest;
					//struct_snd_7Arr_ptr
					return (new InPlacePatchVariant(SPTEST, 0x165789 + 6, (const BYTE*)&struct_snd_7Arr_ptr, 4))->patch()
						//32 count
						&& (new InPlacePatchVariant(SPTEST, 0x1657c9 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1658cc + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x165a9c + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x166dda + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x167fa7 + 3, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x167fb0 + 1, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x25a770 + 0, (const BYTE*)&channelsCount, 4))->patch()
						//s_snd_2Arr
						&& (new InPlacePatchVariant(SPTEST, 0x16a73a + 6, (const BYTE*)&s_snd_2Arr_ptr, 4))->patch()
						//d3dvector1Arr
						&& (new InPlacePatchVariant(SPTEST, 0x16a78e + 2, (const BYTE*)&d3dvector1Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16ac19 + 2, (const BYTE*)&d3dvector1Arr_ptr, 4))->patch()
						//d3dvector1Arr2
						&& (new InPlacePatchVariant(SPTEST, 0x16a780 + 2, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16ac0a + 2, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16a4cc + 2, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16a4f3 + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						//SndSource3DArr
						&& (new InPlacePatchVariant(SPTEST, 0x16a4d4 + 2, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16a744 + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16ac28 + 3, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1fc4eb + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1ffc64 + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						//TODO count seems missing from the above two functions constructor/destructor
						//64 count
						&& (new InPlacePatchVariant(SPTEST, 0x16a74b + 4, (const BYTE*)&soundCount, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16a236 + 2, (const BYTE*)&max_s_snd_2Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x16afd6 + 2, (const BYTE*)&max_s_snd_2Arr_offset, 4))->patch();
					}),
					// since my code requires an array, the offsets above will not be copiable so I would use a callback till I figure out how to pass the offsets without using static or pointers
				new AllocatorPatch(SPTEST, new_struct_snd_19_size, PAGE_READWRITE, [=](LPVOID ptr) -> bool {
				//new CallbackPatch(SPTEST, [=]() -> bool {
					// handle struct_snd_19 which contains the Snd::StreamMixer
					// we need to adjust offsets in array constructor for gaps
					int gap2CC_relativeOffset = gap2CC_offset - 0x9c;
					int gap3CC_relativeOffset = gap3CC_offset - 0x9c;
					// 1. force dynamic alloc rather than the static copy version
					return (new InPlacePatchVariant(SPTEST, 0x167D6F + 1, (const BYTE*)&ptr, 4))->patch()
					//return (new InPlacePatchVariant(SPTEST, 0x167D6F + 1, (const BYTE*)&"\x00\x00\x00\x00", 4))->patch()
					//return (new InPlacePatchVariant(SPTEST, 0x167D6E, (const BYTE*)&"\x68\x01\x00\x00\x00\x53", 6))->patch()
						// 2. change the size of the object, not needed since I used a virtual alloc rather than their malloc or RedMemory alloc
						//&& (new InPlacePatchVariant(SPTEST, 0x167D74 + 1, (BYTE*)new_struct_snd_19_size, 4))->patch()
						// 3. change offsets and count
						// count
						&& (new InPlacePatchVariant(SPTEST, 0x173D56 + 1, (const BYTE*)&channelsCount, 1))->patch()
						// get free channel
						&& (new InPlacePatchVariant(SPTEST, 0x174141 + 2, (const BYTE*)&channelsCount, 1))->patch()
						// gapCC
						&& (new InPlacePatchVariant(SPTEST, 0x173D67 + 2, (const BYTE*)&gapCC_offset, 4))->patch()
						// gap2CC
						&& (new InPlacePatchVariant(SPTEST, 0x173D58 + 2, (const BYTE*)&gap2CC_offset, 4))->patch()
						// gap3CC
						&& (new InPlacePatchVariant(SPTEST, 0x173D86 + 2, (const BYTE*)&gap3CC_offset, 4))->patch()
						// gap4CC
						&& (new InPlacePatchVariant(SPTEST, 0x173D7F + 2, (const BYTE*)&gap4CC_offset, 4))->patch()
						// 4. struct_snd_19::gaps::constructor
						&& (new InPlacePatchVariant(SPTEST, 0x174251 + 2, (const BYTE*)&gap2CC_relativeOffset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x17424d + 1, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x174266 + 2, (const BYTE*)&gap3CC_relativeOffset, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x174262 + 1, (const BYTE*)&channelsCount, 1))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(GOG, channelsCount * struct_snd_7_size_steam + soundCount * (s_snd_2_size_steam + SndSource3D_size_sptest + D3DVECTOR_size * 2), PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					BYTE* struct_snd_7Arr_ptr	= (BYTE*)ptr;
					BYTE* s_snd_2Arr_ptr		= struct_snd_7Arr_ptr	+ channelsCount * struct_snd_7_size_steam;
					BYTE* d3dvector1Arr_ptr		= s_snd_2Arr_ptr		+ soundCount * s_snd_2_size_steam;
					BYTE* d3dvector2Arr_ptr		= d3dvector1Arr_ptr		+ soundCount * D3DVECTOR_size;
					BYTE* SndSource3DArr_ptr	= d3dvector2Arr_ptr		+ soundCount * D3DVECTOR_size;
					BYTE* end_ptr				= SndSource3DArr_ptr	+ soundCount * SndSource3D_size_sptest;
					unsigned int max_struct_snd_7Arr_offset = channelsCount * struct_snd_7_size_steam;
					unsigned int max_s_snd_2Arr_offset		= soundCount * s_snd_2_size_steam;

					BYTE* unrolled_SndSource3DArr_bitset = SndSource3DArr_ptr + 0x28;
					BYTE* unrolled_SndSource3DArr_pv1 = SndSource3DArr_ptr + 0x10;
					BYTE* unrolled_SndSource3DArr_pv2 = SndSource3DArr_ptr + 0x14;

					//struct_snd_7Arr_ptr
					return (new InPlacePatchVariant(GOG, 0x2f1aa7 + 1, (const BYTE*)&struct_snd_7Arr_ptr, 4))->patch()
						//32 count
						&& (new InPlacePatchVariant(GOG, 0x2f1b32 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f1ce3 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f13c7 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f3b16 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f41a6 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f31dc + 2, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f31e1 + 1, (const BYTE*)&channelsCount, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x428acc + 0, (const BYTE*)&channelsCount, 4))->patch()
						//s_snd_2Arr
						&& (new InPlacePatchVariant(GOG, 0x2fb649 + 6, (const BYTE*)&s_snd_2Arr_ptr, 4))->patch()
						//d3dvector1Arr
						&& (new InPlacePatchVariant(GOG, 0x2fb6ea + 3, (const BYTE*)&d3dvector1Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fc019 + 3, (const BYTE*)&d3dvector1Arr_ptr, 4))->patch()
						//d3dvector1Arr2
						&& (new InPlacePatchVariant(GOG, 0x2fb6d2 + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fc00e + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fccfa + 2, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fcd23 + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						//d3dvector1Arr2 end
						&& (new InPlacePatchVariant(GOG, 0x2fcd02 + 2, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						//SndSource3DArr
						&& (new InPlacePatchVariant(GOG, 0x2fb6f1 + 2, (const BYTE*)&unrolled_SndSource3DArr_bitset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fb6fa + 2, (const BYTE*)&unrolled_SndSource3DArr_pv1, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fb709 + 2, (const BYTE*)&unrolled_SndSource3DArr_pv2, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fc03a + 2, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x02f2ae + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x3c1409 + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						//TODO count seems missing from the above two functions constructor/destructor
						//64 count
						//&& (new InPlacePatchVariant(GOG, 0x2FB712 + 2, (const BYTE*)&soundCount, 1))->patch() // doesn't work due to one byte
						&& (new InPlacePatchVariant(GOG, 0x2FB712 + 0, (const BYTE*)"\xEB\x19\x90", 3))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2FB72D + 0, (const BYTE*)"\x81\xFF\x00\x00\x00\x00\xEB\xE0", 8))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2FB72D + 2, (const BYTE*)&soundCount, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fcb0a + 2, (const BYTE*)&max_s_snd_2Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2fb196 + 2, (const BYTE*)&max_s_snd_2Arr_offset, 4))->patch();
					}),
					// since my code requires an array, the offsets above will not be copiable so I would use a callback till I figure out how to pass the offsets without using static or pointers
				new AllocatorPatch(GOG, new_struct_snd_19_size, PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					// handle struct_snd_19 which contains the Snd::StreamMixer
					// we need to adjust offsets in array constructor for gaps
					int gap2CC_relativeOffset = gap2CC_offset - 0x9c;
					int gap3CC_relativeOffset = gap3CC_offset - 0x9c;
					BYTE* allocationType_ptr = (BYTE *)ptr + 8;
					// 1. force dynamic alloc rather than the static copy version
					return (new InPlacePatchVariant(GOG, 0x2f2f4d + 6, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f2f46 + 2, (const BYTE*)&allocationType_ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x2f2f57 + 1, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x3bc659 + 1, (const BYTE*)&ptr, 4))->patch() //unwind, probably not needed
						// 3. change offsets and count
						// count
						&& (new InPlacePatchVariant(GOG, 0x313f76 + 1, (const BYTE*)&channelsCount, 1))->patch()
						// get free channel
						&& (new InPlacePatchVariant(GOG, 0x313bf4 + 2, (const BYTE*)&channelsCount, 1))->patch()
						// gapCC
						&& (new InPlacePatchVariant(GOG, 0x313f87 + 2, (const BYTE*)&gapCC_offset, 4))->patch()
						// gap2CC
						&& (new InPlacePatchVariant(GOG, 0x313f78 + 2, (const BYTE*)&gap2CC_offset, 4))->patch()
						// gap3CC
						&& (new InPlacePatchVariant(GOG, 0x313fa8 + 2, (const BYTE*)&gap3CC_offset, 4))->patch()
						// gap4CC
						&& (new InPlacePatchVariant(GOG, 0x313fa1 + 2, (const BYTE*)&gap4CC_offset, 4))->patch()
						// 4. struct_snd_19::gaps::constructor
						&& (new InPlacePatchVariant(GOG, 0x313316 + 2, (const BYTE*)&gap2CC_relativeOffset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x313312 + 1, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(GOG, 0x313334 + 2, (const BYTE*)&gap3CC_relativeOffset, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0x313330 + 1, (const BYTE*)&channelsCount, 1))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(STEAM, channelsCount* struct_snd_7_size_steam + soundCount * (s_snd_2_size_steam + SndSource3D_size_steam + D3DVECTOR_size * 2), PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					BYTE* struct_snd_7Arr_ptr = (BYTE*)ptr;
					BYTE* s_snd_2Arr_ptr = struct_snd_7Arr_ptr + channelsCount * struct_snd_7_size_steam;
					BYTE* d3dvector1Arr_ptr = s_snd_2Arr_ptr + soundCount * s_snd_2_size_steam;
					BYTE* d3dvector2Arr_ptr = d3dvector1Arr_ptr + soundCount * D3DVECTOR_size;
					BYTE* SndSource3DArr_ptr = d3dvector2Arr_ptr + soundCount * D3DVECTOR_size;
					BYTE* end_ptr = SndSource3DArr_ptr + soundCount * SndSource3D_size_steam;
					unsigned int max_struct_snd_7Arr_offset = channelsCount * struct_snd_7_size_steam;
					unsigned int max_s_snd_2Arr_offset = soundCount * s_snd_2_size_steam;
					//struct_snd_7Arr_ptr
					return (new InPlacePatchVariant(STEAM, 0x30cc26 + 1, (const BYTE*)&struct_snd_7Arr_ptr, 4))->patch()
						//32 count
						&& (new InPlacePatchVariant(STEAM, 0x30cc86 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x30ce23 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x30c4f1 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x30ecd9 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x30f3b7 + 2, (const BYTE*)&max_struct_snd_7Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x30e33b + 6, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x30e34e + 1, (const BYTE*)&channelsCount, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x47831c + 0, (const BYTE*)&channelsCount, 4))->patch()
						//s_snd_2Arr
						&& (new InPlacePatchVariant(STEAM, 0x3160c8 + 1, (const BYTE*)&s_snd_2Arr_ptr, 4))->patch()
						//d3dvector1Arr
						&& (new InPlacePatchVariant(STEAM, 0x316139 + 3, (const BYTE*)&d3dvector1Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x316916 + 3, (const BYTE*)&d3dvector1Arr_ptr, 4))->patch()
						//d3dvector1Arr2
						&& (new InPlacePatchVariant(STEAM, 0x31612a + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x3168f9 + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x317482 + 2, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x3174ab + 3, (const BYTE*)&d3dvector2Arr_ptr, 4))->patch()
						//d3dvector1Arr2 end
						&& (new InPlacePatchVariant(STEAM, 0x31748a + 2, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						//SndSource3DArr
						&& (new InPlacePatchVariant(STEAM, 0x316124 + 2, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x31692f + 2, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x5025e + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x40a8a9 + 1, (const BYTE*)&SndSource3DArr_ptr, 4))->patch()
						//64 count
						//&& (new InPlacePatchVariant(STEAM, 0x31614c + 2, (const BYTE*)&soundCount, 1))->patch() // doesn't work due to one byte
						&& (new InPlacePatchVariant(STEAM, 0x31614c + 0, (const BYTE*)"\xEB\x1E\x90", 3))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x31616C + 0, (const BYTE*)"\x81\xFB\x00\x00\x00\x00\xEB\xDB", 8))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x31616C + 2, (const BYTE*)&soundCount, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x317441 + 2, (const BYTE*)&max_s_snd_2Arr_offset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x315c9a + 2, (const BYTE*)&max_s_snd_2Arr_offset, 4))->patch();
					}),
					// since my code requires an array, the offsets above will not be copiable so I would use a callback till I figure out how to pass the offsets without using static or pointers
				new AllocatorPatch(STEAM, new_struct_snd_19_size, PAGE_READWRITE, [=](LPVOID ptr) -> bool {
					// handle struct_snd_19 which contains the Snd::StreamMixer
					// we need to adjust offsets in array constructor for gaps
					int gap2CC_relativeOffset = gap2CC_offset - 0x9c;
					int gap3CC_relativeOffset = gap3CC_offset - 0x9c;
					// 1. force dynamic alloc rather than the static copy version
					return (new InPlacePatchVariant(STEAM, 0x30e153 + 1, (const BYTE*)&ptr, 4))->patch()
						// 2. change the size of the object, not needed since I used a virtual alloc rather than their malloc or RedMemory alloc
						//&& (new InPlacePatchVariant(SPTEST, 0x167D74 + 1, (BYTE*)new_struct_snd_19_size, 4))->patch()
						// 3. change offsets and count
						// count
						&& (new InPlacePatchVariant(STEAM, 0x32b9e7 + 1, (const BYTE*)&channelsCount, 1))->patch()
						// get free channel
						&& (new InPlacePatchVariant(STEAM, 0x32b6c4 + 2, (const BYTE*)&channelsCount, 1))->patch()
						// gapCC
						&& (new InPlacePatchVariant(STEAM, 0x32b9f8 + 2, (const BYTE*)&gapCC_offset, 4))->patch()
						// gap2CC
						&& (new InPlacePatchVariant(STEAM, 0x32b9e9 + 2, (const BYTE*)&gap2CC_offset, 4))->patch()
						// gap3CC
						&& (new InPlacePatchVariant(STEAM, 0x32ba17 + 2, (const BYTE*)&gap3CC_offset, 4))->patch()
						// gap4CC
						&& (new InPlacePatchVariant(STEAM, 0x32ba10 + 2, (const BYTE*)&gap4CC_offset, 4))->patch()
						// 4. struct_snd_19::gaps::constructor
						&& (new InPlacePatchVariant(STEAM, 0x32ae1b + 2, (const BYTE*)&gap2CC_relativeOffset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x32ae17 + 1, (const BYTE*)&channelsCount, 1))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x32ae3c + 2, (const BYTE*)&gap3CC_relativeOffset, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0x32ae38 + 1, (const BYTE*)&channelsCount, 1))->patch();
				}),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"Aggro",
		new Patch(
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(SPTEST, 0x88A07, (BYTE*)"\x90\x90", 2),
				new ReadOnlyInPlacePatchVariant(SPTEST, 0x2100a0, (BYTE*)"\x00\x00\x7A\x44", 4),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(GOG, 0x22b8ee, (BYTE*)"\x90\x90", 2),
				new ReadOnlyInPlacePatchVariant(GOG, 0x3dccc4, (BYTE*)"\x00\x00\x7A\x44", 4),
			},
			std::vector<PatchVariant*>{
				new InPlacePatchVariant(STEAM, 0x23e57b, (BYTE*)"\x90\x90", 2),
				new InPlacePatchVariant(STEAM, 0x23e497 + 4, (BYTE*)"\x00\x00\x7A\x44", 4),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"Decal",
		new Patch(
			std::vector<PatchVariant*>{
				// new ReadOnlyInPlacePatchVariant	(SPTEST, 0x21B930, (BYTE*)&maxDecal, 4),
				new AllocatorPatch(SPTEST, 0x40, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					float maxDecal = 1000.0;
					float halfDecal = 500.0;
					return (new InPlacePatchVariant(SPTEST, (BYTE*)ptr - (BYTE*)InstanceSingleton::getBase(), (const BYTE*)&maxDecal, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x113385, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x1133aa, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(SPTEST, 0x11337F, (const BYTE*)&halfDecal, 4))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(GOG, 0x40, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					float maxDecal = 1000.0;
					float halfDecal = 500.0;
					LPVOID halfDecalPtr = (byte*)ptr + 4;
					const BYTE* base = (BYTE*)InstanceSingleton::getBase();
					return (new InPlacePatchVariant(GOG, (BYTE*)ptr - base, (const BYTE*)&maxDecal, 4))->patch()
						&& (new InPlacePatchVariant(GOG, (BYTE*)ptr - base + 4, (const BYTE*)&halfDecal, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0xa856a, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(GOG, 0xa858b, (const BYTE*)&halfDecalPtr, 4))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(STEAM, 0x40, PAGE_READWRITE, [](LPVOID ptr) -> bool {
					float maxDecal = 1000.0;
					float halfDecal = 500.0;
					LPVOID halfDecalPtr = (byte*)ptr + 4;
					const BYTE* base = (BYTE*)InstanceSingleton::getBase();
					return (new InPlacePatchVariant(STEAM, (BYTE*)ptr - base, (const BYTE*)&maxDecal, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, (BYTE*)ptr - base + 4, (const BYTE*)&halfDecal, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0xc9448 + 4, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0xc9484 + 4, (const BYTE*)&ptr, 4))->patch()
						&& (new InPlacePatchVariant(STEAM, 0xc94a7 + 4, (const BYTE*)&halfDecalPtr, 4))->patch();
				}),
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"FixEffects",
		new Patch(
			std::vector<PatchVariant*>{
				// increase RedSceneManager->priorityQueue size from 400 to 1600
				new InPlacePatchVariant(SPTEST, 0x18DAEA, (BYTE*)"\x40\x06\x00\x00", 4),	//0x100*1000
				new InPlacePatchVariant(SPTEST, 0x18DAD7, (BYTE*)"\x08\x32\x00\x00", 4),	//0x100*10000
				/**/
				// fix Water Effect limitation from array of 200 items to 800
				// 1. we increase the size 40 + count * 4
				new InPlacePatchVariant(SPTEST, 0x1193FE, (BYTE*)"\xA8\x0C\x00\x00", 4),
				// 2. we rewrite the function so the size is before the array not after
					// 2.1 change size check
					new InPlacePatchVariant(SPTEST, 0x119603, (BYTE*)"\x81\x7B\x20\x20\x03\x00\x00\x90\x90\x90", 10),
					// 2.2 change array access
					new InPlacePatchVariant(SPTEST, 0x119671, (BYTE*)"\x8B\x43\x20\x90\x90\x90\x8B\x44\x83\x28", 10),
					new InPlacePatchVariant(SPTEST, 0x119689, (BYTE*)"\x8B\x43\x20\x90\x90\x90\x56\x8B\x75\x0C\x57\x8B\x7C\x83\x28", 15),
					new InPlacePatchVariant(SPTEST, 0x11969B, (BYTE*)"\x8B\x43\x20\x90\x90\x90\x8B\x44\x83\x28", 10),
					new InPlacePatchVariant(SPTEST, 0x1196B6, (BYTE*)"\x8B\x43\x20\x90\x90\x90\xFF\x74\x83\x28", 10),
					// 2.3 fix increment
					new InPlacePatchVariant(SPTEST, 0x1196EE, (BYTE*)"\xFF\x43\x20\x90\x90\x90", 6),
				// 3. fix virtual functions
					// 3.1
					new InPlacePatchVariant(SPTEST, 0x11953E, (BYTE*)"\x83\x66\x20\x00\x90\x90\x90\x83\xC6\x28\xBF\x20\x03\x00\x00", 15),
					// 3.2
					new InPlacePatchVariant(SPTEST, 0x119752, (BYTE*)"\x39\x5E\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(SPTEST, 0x11975A, (BYTE*)"\x8D\x7E\x28", 3),
					new InPlacePatchVariant(SPTEST, 0x119780, (BYTE*)"\x3B\x5E\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(SPTEST, 0x119788, (BYTE*)"\x8D\x7E\x28", 3),
					new InPlacePatchVariant(SPTEST, 0x11978B, (BYTE*)"\xBB\x20\x03\x00\x00", 5),
					new InPlacePatchVariant(SPTEST, 0x1197A3, (BYTE*)"\x83\x66\x20\x00\x90\x90\x90", 7),
					// 3.3
					new InPlacePatchVariant(SPTEST, 0x11957A, (BYTE*)"\x39\x5E\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(SPTEST, 0x119583, (BYTE*)"\x8D\x7E\x28", 3),
					new InPlacePatchVariant(SPTEST, 0x119594, (BYTE*)"\x8B\x4E\x20\x90\x90\x90\x8B\x4C\x8E\x24\x8B\x07\x89\x0F\xFF\x4E\x20\x90\x90\x90\x8B\x4E\x20\x90\x90\x90\x89\x44\x8E\x28", 30),
					new InPlacePatchVariant(SPTEST, 0x1195B8, (BYTE*)"\x3B\x5E\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(SPTEST, 0x1195C2, (BYTE*)"\x39\x5E\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(SPTEST, 0x1195CA, (BYTE*)"\x8D\x7E\x28", 3),
					new InPlacePatchVariant(SPTEST, 0x1195EB, (BYTE*)"\x3B\x5E\x20\x90\x90\x90", 6),
				// increase priorityQueue 2 from 400 to same as RedSceneManagerQueue
				new InPlacePatchVariant(SPTEST, 0x1607AD, (BYTE*)"\x68\x40\x06\x00\x00\x6A\x02\xE8\x97\xB5\x02\x00\xE9\xDB\x25\x03\x00", 17),
				new InPlacePatchVariant(SPTEST, 0x192D90, (BYTE*)"\xE9\x18\xDA\xFC\xFF\x90\x90\x90\x90", 9),
				new InPlacePatchVariant(SPTEST, 0x18bb12, (BYTE*)"\xBB\x40\x06\x00\x00", 5),
				/**/

			},
			std::vector<PatchVariant*>{
				// increase RedSceneManager->priorityQueue size from 400 to 1600
				new InPlacePatchVariant(GOG, 0x29F590, (BYTE*)"\x40\x06\x00\x00", 4),	//0x100*1000
				new InPlacePatchVariant(GOG, 0x29F57A, (BYTE*)"\x08\x32\x00\x00", 4),	//0x100*10000
				//TODO rewrite gog function
			},
			std::vector<PatchVariant*>{
				// increase RedSceneManager->priorityQueue size from 400 to 1600, nope 100000
				new InPlacePatchVariant(STEAM, 0x2BC2D3, (BYTE*)"\xA0\x86\x01\x00", 4),
				/**/
				// fix Water Effect limitation from array of 200 items to 800
				// 1. we increase the size 40 + count * 4
				new InPlacePatchVariant(STEAM, 0x240ADA, (BYTE*)"\x28\x32\x04\x00", 4), // is size
				// 2. we rewrite the function so the size is before the array not after
					// 2.1 change size check
					new InPlacePatchVariant(STEAM, 0x240E79, (BYTE*)"\x81\x7E\x20\x80\x0C\x01\x00\x90\x90\x90", 10), // has count
					// 2.2 change array access
					new InPlacePatchVariant(STEAM, 0x240EC0, (BYTE*)"\x8B\x46\x20\x90\x90\x90\x8B\x4C\x86\x28", 10),
					new InPlacePatchVariant(STEAM, 0x240ED2, (BYTE*)"\x8B\x46\x20\x90\x90\x90\x8B\x4C\x86\x28", 10),
					new InPlacePatchVariant(STEAM, 0x240EF5, (BYTE*)"\x8B\x4E\x20\x90\x90\x90\x8B\x4C\x8E\x28", 10),
					new InPlacePatchVariant(STEAM, 0x240F04, (BYTE*)"\x8B\x46\x20\x90\x90\x90\x8D\x4D\xF8\x6A\x01\xFF\x74\x86\x28", 15),
					// 2.3 fix increment
					new InPlacePatchVariant(STEAM, 0x240F42, (BYTE*)"\xFF\x46\x20\x90\x90\x90", 6),
				// 3. fix virtual functions
					// 3.1
					new InPlacePatchVariant(STEAM, 0x240B79, (BYTE*)"\xC7\x46\x20\x00\x00\x00\x00\x90\x90\x90\xBF\x80\x0C\x01\x00\x83\xC6\x28", 18), // has count \x80\x0C
					// 3.2
					new InPlacePatchVariant(STEAM, 0x240A4C, (BYTE*)"\x39\x7B\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(STEAM, 0x240A54, (BYTE*)"\x8D\x73\x28", 3),
					new InPlacePatchVariant(STEAM, 0x240A83, (BYTE*)"\x3B\x7B\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(STEAM, 0x240A8B, (BYTE*)"\x8D\x73\x28", 3),
					new InPlacePatchVariant(STEAM, 0x240A8E, (BYTE*)"\xBF\x80\x0C\x01\x00", 5), // has count
					new InPlacePatchVariant(STEAM, 0x240AA8, (BYTE*)"\x89\x7B\x20\x90\x90\x90", 6),
					// 3.3
					new InPlacePatchVariant(STEAM, 0x240DAB, (BYTE*)"\x8B\x46\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(STEAM, 0x240DB5, (BYTE*)"\x8D\x7E\x28", 3),
					new InPlacePatchVariant(STEAM, 0x240DCE, (BYTE*)"\x8B\x46\x20\x90\x90\x90\x8B\x0F\x8B\x44\x86\x24\x89\x07\xFF\x4E\x20\x90\x90\x90\x8B\x46\x20\x90\x90\x90\x89\x4C\x86\x28", 30),
					new InPlacePatchVariant(STEAM, 0x240DF2, (BYTE*)"\x8B\x46\x20\x90\x90\x90", 6),
					new InPlacePatchVariant(STEAM, 0x240E02, (BYTE*)"\x8D\x7E\x28", 3),
					new InPlacePatchVariant(STEAM, 0x240E30, (BYTE*)"\x3B\x5E\x20\x90\x90\x90", 6),
				// increase priorityQueue 2 from 400 to same as RedSceneManagerQueue
				new InPlacePatchVariant(STEAM, 0x294BC5, (BYTE*)"\x68\xA0\x86\x01\x00\x6A\x02\xE8\x05\x6F\xD8\xFF\xE9\x0A\xFD\xFF\xFF", 17),
				new InPlacePatchVariant(STEAM, 0x2948D7, (BYTE*)"\xE9\xE9\x02\x00\x00\x90\x90\x90\x90", 9),
				new InPlacePatchVariant(STEAM, 0x2F1B82, (BYTE*)"\xBF\xA0\x86\x01\x00", 5),
				/**/
			}
		)
	));
	patches.insert(std::pair<const char*, Patch*>(
		"PostHook",
		new Patch(
			std::vector<PatchVariant*>{
				new AllocatorPatch(SPTEST, 0x100, PAGE_EXECUTE_READ, [](LPVOID ptr) -> bool {
					PVOID base_address = InstanceSingleton::getBase();
					BYTE aMissionName[] = "gMissionName";
					BYTE hookFilePath[] = "Hooks/PostScriptInit.lua";
					DWORD pgMissionName = (DWORD)base_address + 0x32b48c;
					DWORD pLuaScript_Get = (DWORD)base_address + 0x21dcb;
					DWORD pLua_pushString = (DWORD)base_address + 0x12a750;
					DWORD pLua_setGlobal = (DWORD)base_address + 0x12aa70;
					DWORD pLua_dofile = (DWORD)base_address + 0x12b5e0;
					DWORD pLuaScript_CloseScript = (DWORD)base_address + 0x21d38;

					DWORD paMissionName = (DWORD)ptr;
					DWORD phookFilePath = paMissionName + 0x10;
					DWORD pCode = phookFilePath + 0x20;
					DWORD targetAddrRVA = 0xffabf;
					DWORD pReturnAddress = (DWORD)base_address + targetAddrRVA + 5;

					BYTE hookCode []= { 0xE9, 0, 0, 0, 0 };
					*(DWORD*)&hookCode[1] = pCode - (DWORD)base_address - targetAddrRVA - 5;
					// a1 = LuaScript::get();
					// lua::pushString(a1, &gMissionName);
					// lua::setGlobal(a1, "gMissionName")
					// dofile(a1, "Hooks/PostScriptInit.lua")
					// LuaScript::CloseScript()
					BYTE patch[] = {
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, LuaScript::get; call eax
						0x68, 0, 0, 0, 0,					//push &gMissionName
						0x50,								//push eax
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::pushString; call eax
						0xC7, 0x44, 0x24, 0x04, 0, 0, 0, 0,	//mov dword ptr [esp+4], aMissionName
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::setGlobal; call eax
						0xC7, 0x44, 0x24, 0x04, 0, 0, 0, 0,	//mov dword ptr [esp+4], hookFilePath
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::dofile; call eax
						0x83, 0xC4, 0x08,					//add esp,8
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, LuaScript::CloseScript; call eax
						0xB8, 0, 0, 0, 0, 0xFF, 0xE0,		//mov eax, returnAddress; jmp eax
					};
					*(DWORD*)&patch[1] = pLuaScript_Get;
					*(DWORD*)&patch[8] = pgMissionName;
					*(DWORD*)&patch[14] = pLua_pushString;
					*(DWORD*)&patch[24] = paMissionName;
					*(DWORD*)&patch[29] = pLua_setGlobal;
					*(DWORD*)&patch[39] = phookFilePath;
					*(DWORD*)&patch[44] = pLua_dofile;
					*(DWORD*)&patch[54] = pLuaScript_CloseScript;
					*(DWORD*)&patch[61] = pReturnAddress;
					return (new InPlacePatchVariant(SPTEST, (BYTE*)paMissionName - (BYTE*)base_address, (const BYTE*)&aMissionName, 13))->patch()
						&& (new InPlacePatchVariant(SPTEST, (BYTE*)phookFilePath - (BYTE*)base_address, (const BYTE*)&hookFilePath, 25))->patch()
						&& (new InPlacePatchVariant(SPTEST, (BYTE*)pCode         - (BYTE*)base_address, (const BYTE*)&patch, sizeof(patch)/sizeof(BYTE)))->patch()
						&& (new InPlacePatchVariant(SPTEST, targetAddrRVA,                              (const BYTE*)&hookCode, 5))->patch();
				}),
			},
			std::vector<PatchVariant*>{
				new AllocatorPatch(GOG, 0x100, PAGE_EXECUTE_READ, [](LPVOID ptr) -> bool {
					PVOID base_address = InstanceSingleton::getBase();
					BYTE aMissionName[] = "gMissionName";
					BYTE hookFilePath[] = "Hooks/PostScriptInit.lua";
					DWORD pgMissionName = (DWORD)base_address + 0x49ad1c;
					DWORD pLuaScript_Get = (DWORD)base_address + 0xc022;
					DWORD pLua_pushString = (DWORD)base_address + 0xbbd6;
					DWORD pLua_setGlobal = (DWORD)base_address + 0x3382;
					DWORD pLua_dofile = (DWORD)base_address + 0x11c20;
					DWORD pLuaScript_CloseScript = (DWORD)base_address + 0xf7b8;

					DWORD paMissionName = (DWORD)ptr;
					DWORD phookFilePath = paMissionName + 0x10;
					DWORD pCode = phookFilePath + 0x20;
					DWORD targetAddrRVA = 0x126988;
					DWORD pReturnAddress = (DWORD)base_address + targetAddrRVA + 5;

					BYTE hookCode[] = { 0xE9, 0, 0, 0, 0 };
					*(DWORD*)&hookCode[1] = pCode - (DWORD)base_address - targetAddrRVA - 5;
					// a1 = LuaScript::get();
					// lua::pushString(a1, &gMissionName);
					// lua::setGlobal(a1, "gMissionName")
					// dofile(a1, "Hooks/PostScriptInit.lua")
					// LuaScript::CloseScript()
					BYTE patch[] = {
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, LuaScript::get; call eax
						0x68, 0, 0, 0, 0,					//push &gMissionName
						0x50,								//push eax
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::pushString; call eax
						0xC7, 0x44, 0x24, 0x04, 0, 0, 0, 0,	//mov dword ptr [esp+4], aMissionName
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::setGlobal; call eax
						0xC7, 0x44, 0x24, 0x04, 0, 0, 0, 0,	//mov dword ptr [esp+4], hookFilePath
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::dofile; call eax
						0x83, 0xC4, 0x08,					//add esp,8
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, LuaScript::CloseScript; call eax
						0xB8, 0, 0, 0, 0, 0xFF, 0xE0,		//mov eax, returnAddress; jmp eax
					};
					*(DWORD*)&patch[1] = pLuaScript_Get;
					*(DWORD*)&patch[8] = pgMissionName;
					*(DWORD*)&patch[14] = pLua_pushString;
					*(DWORD*)&patch[24] = paMissionName;
					*(DWORD*)&patch[29] = pLua_setGlobal;
					*(DWORD*)&patch[39] = phookFilePath;
					*(DWORD*)&patch[44] = pLua_dofile;
					*(DWORD*)&patch[54] = pLuaScript_CloseScript;
					*(DWORD*)&patch[61] = pReturnAddress;
					return (new InPlacePatchVariant(GOG, (BYTE*)paMissionName - (BYTE*)base_address, (const BYTE*)&aMissionName, 13))->patch()
						&& (new InPlacePatchVariant(GOG, (BYTE*)phookFilePath - (BYTE*)base_address, (const BYTE*)&hookFilePath, 25))->patch()
						&& (new InPlacePatchVariant(GOG, (BYTE*)pCode - (BYTE*)base_address, (const BYTE*)&patch, sizeof(patch) / sizeof(BYTE)))->patch()
						&& (new InPlacePatchVariant(GOG, targetAddrRVA, (const BYTE*)&hookCode, 5))->patch();
					}),
				},
			std::vector<PatchVariant*>{
				new AllocatorPatch(STEAM, 0x100, PAGE_EXECUTE_READ, [](LPVOID ptr) -> bool {
					PVOID base_address = InstanceSingleton::getBase();
					BYTE aMissionName[] = "gMissionName";
					BYTE hookFilePath[] = "Hooks/PostScriptInit.lua";
					DWORD pgMissionName = (DWORD)base_address + 0x4e6d6c;
					DWORD pLuaScript_Get = (DWORD)base_address + 0xbaf0;
					DWORD pLua_pushString = (DWORD)base_address + 0xb465;
					DWORD pLua_setGlobal = (DWORD)base_address + 0x448f;
					DWORD pLua_dofile = (DWORD)base_address + 0x1436c;
					DWORD pLuaScript_CloseScript = (DWORD)base_address + 0x10d2f;

					DWORD paMissionName = (DWORD)ptr;
					DWORD phookFilePath = paMissionName + 0x10;
					DWORD pCode = phookFilePath + 0x20;
					DWORD targetAddrRVA = 0x141e9e;
					DWORD pReturnAddress = (DWORD)base_address + targetAddrRVA + 5;

					BYTE hookCode []= { 0xE9, 0, 0, 0, 0 };
					*(DWORD*)&hookCode[1] = pCode - (DWORD)base_address - targetAddrRVA - 5;
					// a1 = LuaScript::get();
					// lua::pushString(a1, &gMissionName);
					// lua::setGlobal(a1, "gMissionName")
					// dofile(a1, "Hooks/PostScriptInit.lua")
					// LuaScript::CloseScript()
					BYTE patch[] = {
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, LuaScript::get; call eax
						0x68, 0, 0, 0, 0,					//push &gMissionName
						0x50,								//push eax
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::pushString; call eax
						0xC7, 0x44, 0x24, 0x04, 0, 0, 0, 0,	//mov dword ptr [esp+4], aMissionName
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::setGlobal; call eax
						0xC7, 0x44, 0x24, 0x04, 0, 0, 0, 0,	//mov dword ptr [esp+4], hookFilePath
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, lua::dofile; call eax
						0x83, 0xC4, 0x08,					//add esp,8
						0xB8, 0, 0, 0, 0, 0xFF, 0xD0,		//mov eax, LuaScript::CloseScript; call eax
						0xB8, 0, 0, 0, 0, 0xFF, 0xE0,		//mov eax, returnAddress; jmp eax
					};
					*(DWORD*)&patch[1] = pLuaScript_Get;
					*(DWORD*)&patch[8] = pgMissionName;
					*(DWORD*)&patch[14] = pLua_pushString;
					*(DWORD*)&patch[24] = paMissionName;
					*(DWORD*)&patch[29] = pLua_setGlobal;
					*(DWORD*)&patch[39] = phookFilePath;
					*(DWORD*)&patch[44] = pLua_dofile;
					*(DWORD*)&patch[54] = pLuaScript_CloseScript;
					*(DWORD*)&patch[61] = pReturnAddress;
					return (new InPlacePatchVariant(STEAM, (BYTE*)paMissionName - (BYTE*)base_address, (const BYTE*)&aMissionName, 13))->patch()
						&& (new InPlacePatchVariant(STEAM, (BYTE*)phookFilePath - (BYTE*)base_address, (const BYTE*)&hookFilePath, 25))->patch()
						&& (new InPlacePatchVariant(STEAM, (BYTE*)pCode         - (BYTE*)base_address, (const BYTE*)&patch, sizeof(patch)/sizeof(BYTE)))->patch()
						&& (new InPlacePatchVariant(STEAM, targetAddrRVA,                              (const BYTE*)&hookCode, 5))->patch();
				}),
			}
		)
	));
}