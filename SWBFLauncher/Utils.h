#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>

enum GameVersion
{
	NOTDEFINED,
	SPTEST,
	GOG,
	STEAM
};

std::wstring GetLastErrorAsString();
std::wstring UrlEncode(const std::wstring& url);
#endif // !UTILS