#pragma once
#include "../Logger/Logging.h"
#include <filesystem>
#include <windows.h>
#include <tlhelp32.h>>

namespace SideLoad
{
	bool IsKnownDll(const char* ExePath, DWORD Signature, )
	bool Setup();
	void Unload();

}