#include "Sideload.h"

#include <windows.h>

bool SideLoad::Setup()
{
	Logger::Info("Setting up sideload...", nullptr);

	// Get current path
	const std::filesystem::path Path = std::filesystem::current_path();
	// Get current exe name
	char ExeName[MAX_PATH];
	DWORD ExeNameSize = GetModuleFileNameA(nullptr, ExeName, MAX_PATH);
	if (ExeNameSize == 0)
	{
		Logger::Error("Failed to get current exe name.", nullptr);
		return false;
	}

	Logger::Info("Current exe name: %s", ExeName);

	if (!exists(Path))
	{
		Logger::Error("Failed to find current path.", nullptr);
		return false;
	}

	for (const auto& Entry : std::filesystem::directory_iterator(Path))
	{
		if (Entry.is_regular_file() && Entry.path().extension() == ".exe")
		{
			//Logger::Info("Found Exe: %s", Entry.path().string().c_str());
			if (Entry.path().filename().string().find("_ac_") != std::string::npos && Entry.path().filename().string() != Path.string())
			{
				Logger::Info("Found sideloaded exe: %s", Entry.path().string().c_str());

				STARTUPINFO Startupinfo = { sizeof(Startupinfo) };
				PROCESS_INFORMATION  ProcessInformation;
				if (CreateProcessA(Entry.path().string().c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &Startupinfo, &ProcessInformation))
				{
					Logger::Info("Created process.", nullptr);
				}
				else
				{
					Logger::Error("Failed to create process.", nullptr);
				}
			}
		}
	}

	return true;
}
