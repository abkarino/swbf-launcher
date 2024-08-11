#pragma once
#include "pch.h"
#include "Utils.h"
#include <tlhelp32.h>
#include <fstream>
#include <openssl/evp.h>
#pragma comment (lib, "crypt32")
//#include <winternl.h>
//
//// #define instead of typedef to override
//#define RTL_DRIVE_LETTER_CURDIR struct {\
//  WORD Flags;\
//  WORD Length;\
//  ULONG TimeStamp;\
//  STRING DosPath;\
//}\
//
//// #define instead of typedef to override
//#define RTL_USER_PROCESS_PARAMETERS struct {\
//  ULONG MaximumLength;\
//  ULONG Length;\
//  ULONG Flags;\
//  ULONG DebugFlags;\
//  PVOID ConsoleHandle;\
//  ULONG ConsoleFlags;\
//  PVOID StdInputHandle;\
//  PVOID StdOutputHandle;\
//  PVOID StdErrorHandle;\
//  UNICODE_STRING CurrentDirectoryPath;\
//  PVOID CurrentDirectoryHandle;\
//  UNICODE_STRING DllPath;\
//  UNICODE_STRING ImagePathName;\
//  UNICODE_STRING CommandLine;\
//  PVOID Environment;\
//  ULONG StartingPositionLeft;\
//  ULONG StartingPositionTop;\
//  ULONG Width;\
//  ULONG Height;\
//  ULONG CharWidth;\
//  ULONG CharHeight;\
//  ULONG ConsoleTextAttributes;\
//  ULONG WindowFlags;\
//  ULONG ShowWindowFlags;\
//  UNICODE_STRING WindowTitle;\
//  UNICODE_STRING DesktopName;\
//  UNICODE_STRING ShellInfo;\
//  UNICODE_STRING RuntimeData;\
//  RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[32];\
//  ULONG EnvironmentSize;\
//}\

typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI* NtResumeProcess)(HANDLE ProcessHandle);

using std::string;
using std::wstring;

class InstanceSingleton {
private:
	static PROCESS_INFORMATION     processInformation;
	static bool initialized;
	static PVOID base;
public:
	static LPPROCESS_INFORMATION getInstance() {
		return initialized ? &processInformation : NULL;
	}
	static PVOID getBase() {
		return initialized ? base : nullptr;
	}
	static string sha1HashFile(wstring executable) {
		unsigned char buf[8192];
		EVP_MD_CTX * mdctx = EVP_MD_CTX_new();

		FILE* f;
		if (_wfopen_s(&f, executable.c_str(), L"rb")) {
			return string();
		}
		EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL);
		for (;;) {
			size_t len;

			len = fread(buf, 1, sizeof buf, f);
			if (len == 0)
				break;
			EVP_DigestUpdate(mdctx, buf, len);
		}
		fclose(f);

		unsigned int mdLen = 0;
		unsigned char* digest = (unsigned char*)OPENSSL_malloc(EVP_MD_size(EVP_sha1()));

		EVP_DigestFinal_ex(mdctx, digest, &mdLen);
		EVP_MD_CTX_free(mdctx);

		string hash;
		unsigned char buff[3];
		for (unsigned int i = 0; i < mdLen; i++) {
			sprintf_s((char*)buff, 3, "%02X", digest[i]);
			hash += string((char*)buff, (char*)buff + 2);
		}
		return hash;
	}
	static bool validateFile(GameVersion gameVersion, wstring executable) {
		if (gameVersion == SPTEST) return true;
		else if (gameVersion == GOG) {
			string hash = sha1HashFile(executable);
			return hash == "C2FAB90D529D329ECBDF0C719FF6594250311588" || hash == /*my cff 4gb edit*/"29C3EAC670D4E98E35ADE6A01AC56D6657912F22";
		}
		else if (gameVersion == STEAM) {
			string hash = sha1HashFile(executable);
			return hash == "958B0F01461836D7B94CA869922BA66B0C5EC7BE" || hash == /*infinite loop included*/"E136DE9E14BDAEF87B3B3D85BAB91CA7E57BD364";
		}
		else return false;
	}
	static void createInstance(GameVersion gameVersion, wstring commandLine){
		if (initialized) {
			return;
		}

		TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);

		wstring gameBase = NPath;
		gameBase += L"\\";

		wstring dir;
		wstring executable;

		if (gameVersion == SPTEST) {
			dir = gameBase + L"Debug\\";
			executable = dir + L"SPTest.exe";
		}
		else if (gameVersion == GOG) {
			dir = gameBase + L"GameData\\";
			executable = dir + L"Battlefront.exe";
		}
		else if (gameVersion == STEAM) {
			dir = gameBase + L"GameData\\";
			executable = dir + L"Battlefront.exe";
		}
		else return;

		wchar_t command[MAX_PATH];// = L"SPTest.exe /win /nointro";
		wcscpy_s(command, wstring(L"\"" + executable + L"\" " + commandLine).c_str());


		STARTUPINFO             startupInfo;
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		ZeroMemory(&processInformation, sizeof(processInformation));
		//attach(L"sptest.exe");
		//initialized = true;
		//return;
		bool validFile = validateFile(gameVersion, executable);
		if (!validFile) {
			if (gameVersion == GOG) {
				MessageBox(NULL, wstring(L"Invalid GOG file, make sure you are using v1.3.5 executable and the 4GB applied.").c_str(), L"Error", MB_ICONERROR);
				return;
			}
			else if (gameVersion == STEAM) {
				MessageBox(NULL, wstring(L"Invalid STEAM file, make sure you are using v1.3.7 executable and both steamless and the 4GB applied.").c_str(), L"Error", MB_ICONERROR);
				return;
			}
		}

		if (gameVersion != STEAM) {
			if (!CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, dir.c_str(), &startupInfo, &processInformation))
			{
				MessageBox(NULL, wstring(L"Failed to create process.\n").append(GetLastErrorAsString()).append(gameVersion == STEAM ? L"\nMake sure you removed steamStub using Steamless tool and the new file is named \"Battlefront.exe.unpacked.exe\"." : L"").c_str(), L"Error", MB_ICONERROR);
				return;
			}


			CONTEXT context;
			ZeroMemory(&context, sizeof(context));
			context.ContextFlags = CONTEXT_INTEGER;
			if (!GetThreadContext(processInformation.hThread, &context)) {
				MessageBox(NULL, wstring(L"Failed to read thread context.").c_str(), L"Error", MB_ICONERROR);
				return;
			}

			ZeroMemory(&base, sizeof(base));
			if (!ReadProcessMemory(processInformation.hProcess, (LPCVOID)(context.Ebx + 8), &base, sizeof(PVOID), NULL)) {
				MessageBox(NULL, wstring(L"Failed to read process memory.").c_str(), L"Error", MB_ICONERROR);
				return;
			}

			if (gameVersion == STEAM) {
				DWORD entryPoint = 0x334638E9;
				WriteProcessMemory(processInformation.hProcess, (byte*)base + 0x110e5, &entryPoint, sizeof(entryPoint), nullptr);
			}
		}
		else {
			std::fstream gameBinary(executable, std::ios::binary | std::ios::out | std::ios::in);
			gameBinary.seekp(0x104E5, std::ios::beg);
			WORD inf = 0xFEEB;
			gameBinary.write(reinterpret_cast<char*>(&inf), sizeof(inf));
			gameBinary.close();

			//make sure the file has infinite loop
			/*
			WORD inf = 0xFEEB;
			if (!entryPoint || !WriteProcessMemory(processInformation.hProcess, (byte*)base + 0x110e5, &inf, sizeof(inf), nullptr)) {
				MessageBox(NULL, wstring(L"Failed to inject inifinite loop.").c_str(), L"Error", MB_ICONERROR);
				return;
			}
			*/

			wstring steamURL = L"steam://rungameid/1058020//";
			steamURL.append(UrlEncode(commandLine));
			ShellExecuteW(NULL, L"open", steamURL.c_str(), NULL, NULL, SW_SHOW);

			bool found = false;
			int maxTries = 500;
			while (!found && maxTries) {
				maxTries--;
				Sleep(50);
				PROCESSENTRY32 entry;
				entry.dwSize = sizeof(PROCESSENTRY32);

				HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

				if (Process32First(snapshot, &entry) == TRUE)
				{
					do
					{
						if (_wcsicmp(entry.szExeFile, L"Battlefront.exe") == 0)
						{
							found = true;
							processInformation.dwProcessId = entry.th32ProcessID;
							processInformation.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
							if (!processInformation.hProcess) {
								MessageBox(NULL, wstring(L"Failed to open process, quit the game and try running the launcher as administrator.").c_str(), L"Error", MB_ICONERROR);
								return;
							}
							break;
						}
					} while (Process32Next(snapshot, &entry) == TRUE);
				}

				CloseHandle(snapshot);
			}
			if (!found) {
				MessageBox(NULL, wstring(L"Failed to find process.").c_str(), L"Error", MB_ICONERROR);
				return;
			}

			//find base
			MODULEENTRY32W entry;
			entry.dwSize = sizeof(MODULEENTRY32W);
			found = false;
			maxTries = 500;
			HANDLE snapshot;
			wstring modules = L"";
			do {
				snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processInformation.dwProcessId);
				modules = L"";

				if (Module32First(snapshot, &entry) == TRUE)
				{
					do
					{
						modules.append(entry.szModule).append(L" ");
						if (_wcsicmp(entry.szModule, L"Battlefront.exe") == 0)
						{
							found = true;
							base = (PVOID)entry.modBaseAddr;
							break;
						}
					} while (Module32Next(snapshot, &entry) == TRUE);
				}
				maxTries--;
				Sleep(1);
			} while (!found && maxTries);

			CloseHandle(snapshot);
			if (!found) {
				MessageBox(NULL, wstring(L"Failed to find module base.\nFound modules: ").append(modules).c_str(), L"Error", MB_ICONERROR);
				return;
			}
		}
		initialized = true;
	}
	static void resume() {
		if (processInformation.hThread) ResumeThread(processInformation.hThread);
		else {
			NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
				GetModuleHandle(L"ntdll"), "NtSuspendProcess");
			pfnNtSuspendProcess(processInformation.hProcess);

			DWORD entryPoint = 0x334638E9;
			WriteProcessMemory(processInformation.hProcess, (byte*)base + 0x110e5, &entryPoint, sizeof(entryPoint), nullptr);

			NtResumeProcess pfnNtResumeProcess = (NtSuspendProcess)GetProcAddress(
				GetModuleHandle(L"ntdll"), "NtResumeProcess");
			pfnNtResumeProcess(InstanceSingleton::getInstance()->hProcess);
		}
	}
	static void closeInstance() {
		if (!initialized) {
			return;
		}

		CloseHandle(processInformation.hProcess);
		ZeroMemory(&processInformation, sizeof(processInformation));

		initialized = false;
	}
	static void attach(const wchar_t* gameName) {

		bool found = false;
		int maxTries = 500;
		while (!found && maxTries) {
			maxTries--;
			Sleep(50);
			PROCESSENTRY32 entry;
			entry.dwSize = sizeof(PROCESSENTRY32);

			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

			if (Process32First(snapshot, &entry) == TRUE)
			{
				do
				{
					if (_wcsicmp(entry.szExeFile, gameName) == 0)
					{
						found = true;
						processInformation.dwProcessId = entry.th32ProcessID;
						processInformation.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
						if (!processInformation.hProcess) {
							MessageBox(NULL, wstring(L"Failed to open process, quit the game and try running the launcher as administrator.").c_str(), L"Error", MB_ICONERROR);
							return;
						}
						break;
					}
				} while (Process32Next(snapshot, &entry) == TRUE);
			}

			CloseHandle(snapshot);
		}
		if (!found) {
			MessageBox(NULL, wstring(L"Failed to find process.").c_str(), L"Error", MB_ICONERROR);
			return;
		}

		//find base
		MODULEENTRY32W entry;
		entry.dwSize = sizeof(MODULEENTRY32W);
		found = false;
		maxTries = 500;
		HANDLE snapshot;
		wstring modules = L"";
		do {
			snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processInformation.dwProcessId);
			modules = L"";

			if (Module32First(snapshot, &entry) == TRUE)
			{
				do
				{
					modules.append(entry.szModule).append(L" ");
					if (_wcsicmp(entry.szModule, gameName) == 0)
					{
						found = true;
						base = (PVOID)entry.modBaseAddr;
						break;
					}
				} while (Module32Next(snapshot, &entry) == TRUE);
			}
			maxTries--;
			Sleep(1);
		} while (!found && maxTries);

		CloseHandle(snapshot);
		if (!found) {
			MessageBox(NULL, wstring(L"Failed to find module base.\nFound modules: ").append(modules).c_str(), L"Error", MB_ICONERROR);
			return;
		}
	}
};