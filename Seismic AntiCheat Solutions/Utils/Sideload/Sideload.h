#pragma once
#include "../Logger/Logging.h"
#include <filesystem>
#include <windows.h>
#include <tlhelp32.h>>

namespace SideLoad
{
	bool Setup();
	void Unload();

}