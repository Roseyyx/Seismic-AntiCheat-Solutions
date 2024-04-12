#include "Logging.h"

bool Logging::Attach()
{
	if (!AllocConsole())
		return false;

	AttachConsole(ATTACH_PARENT_PROCESS);

	if (freopen_s(&PStream, "CONOUT$", "w", stdout) != 0)
		return false;

	if (!SetConsoleTitleA("Seismic Anti Cheat Solutions Logger"))
		return false;

	return true;
}


bool Logging::Detach()
{
	if (!fclose(PStream))
		return false;

	if (!FreeConsole())
		return false;

	if (const auto ConsoleWindowHandle = GetConsoleWindow(); ConsoleWindowHandle != nullptr)
		PostMessageW(ConsoleWindowHandle, WM_CLOSE, 0U, 0L);

	return true;
}
